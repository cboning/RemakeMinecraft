#include "world.h"
#include <GLFW/glfw3.h>
#include <cstdint>
#include <iostream>
extern struct block BlockID;

void World::cltm()
{
    std::unordered_map<int, int> linetomap;
    for (int i = 0; i < chunkda; i++)
    {

        linetomap[i] = (i % chunkdl - chunkdr) * (i % chunkdl - chunkdr) + (i / chunkdl - chunkdr) * (i / chunkdl - chunkdr);
    }
    int t = 0;
    for (int i = 0; i < chunkda; i++)
    {
        auto minPair = linetomap.begin();
        for (auto it = linetomap.begin(); it != linetomap.end(); ++it)
        {
            if (it->second < minPair->second)
            {
                minPair = it;
            }
        }

        linetmap[t] = minPair->first;
        linetomap.erase(minPair);
        t++;
    }
}

void World::set_skylights(int x, int y, int z) { get_light_chunk({x >> 4, y >> 4, z >> 4}).skylights.insert((y & 15) << 8 | (z & 15) << 4 | (x & 15)); }

int World::get_height_highestmap(int x, int z) { return get_chunkxz(x >> 4, z >> 4).highmap[(z & 15) << 4 | (x & 15)]; }

Chunk &World::get_chunk(int x, int y, int z) { return get_chunkxz(x, z).chunkxz[y + 4]; }

ChunkXZ &World::get_chunkxz(int x, int z)
{

    // std::unique_lock<std::mutex> lock(muex);
    // muex.lock();
    ChunkXZ &tem = world[{x, z}];
    // muex.unlock();
    return tem;
}

LightsLevel &World::get_light_chunk(pos3 cpos) { return get_chunkxz(cpos.x, cpos.z).lightchunks[cpos.y + 5]; }

Block World::get_block(int x, int y, int z)
{
    if (y > 319 || y < -64)
        return Air;
    return get_chunk(x >> 4, y >> 4, z >> 4).get_block(x & 15, y & 15, z & 15);
}

void World::set_light(int x, int y, int z, int light) { get_light_chunk({x >> 4, y >> 4, z >> 4}).set_light_level(x & 15, y & 15, z & 15, light); }
void World::set_skylight(int x, int y, int z, int light) { get_light_chunk({x >> 4, y >> 4, z >> 4}).set_skylight_level(x & 15, y & 15, z & 15, light); }

int World::get_light(int x, int y, int z) { return get_light_chunk({x >> 4, y >> 4, z >> 4}).get_light_level(x & 15, y & 15, z & 15); }

int World::get_skylight(int x, int y, int z) { return get_light_chunk({x >> 4, y >> 4, z >> 4}).get_skylight_level(x & 15, y & 15, z & 15); }
std::unordered_set<pos3, ChunkPos3Hash> World::delete_lights(std::unordered_map<int, std::vector<pos3>> dlightPos)
{

    int tem_light, other_light = 0, i;
    pos3 tem_pos;

    std::vector<pos3> light_list_remove, temp_light_list_remove, light_list_other[15], light_list, temp_light_list, tem;
    std::unordered_set<pos3, ChunkPos3Hash> tempCheckList;

    for (int n = 15; n > 1; n--)
    {
        temp_light_list_remove.clear();
        for (auto it : dlightPos[n - 1])
        {
            light_list_remove.push_back(it);
            set_light(it.x, it.y, it.z, 0);
            tem.push_back(it);
            tempCheckList.insert({(it.x >> 4), (it.y >> 4), (it.z >> 4)});
        }

        for (auto it : light_list_remove)
        {
            for (i = 0; i < 18;)
            {

                tem_pos = {it.x + lightcheckdir[i++], it.y + lightcheckdir[i++], it.z + lightcheckdir[i++]};
                tem_light = get_light(tem_pos.x, tem_pos.y, tem_pos.z);
                if (tem_light != 0)
                {
                    if (tem_light < n)
                    {
                        set_light(tem_pos.x, tem_pos.y, tem_pos.z, 0);
                        temp_light_list_remove.push_back(tem_pos);
                    }
                    else
                    {
                        light_list_other[tem_light - 1].push_back(tem_pos);
                        if (tem_light > other_light)
                        {
                            other_light = tem_light;
                        }
                    }
                }
                for (int nx = -1; nx < 1; nx++)
                {
                    for (int ny = -1; ny < 1; ny++)
                    {
                        for (int nz = -1; nz < 1; nz++)
                        {
                            if ((get_block(tem_pos.x + nx, tem_pos.y + ny, tem_pos.z + nz).BlockNID->opaque))
                            {
                                tempCheckList.insert({(tem_pos.x >> 4), (tem_pos.y >> 4), (tem_pos.z >> 4)});
                            }
                        }
                    }
                }
            }
        }
        light_list_remove = temp_light_list_remove;
    }
    for (auto it : light_list_remove)
    {
        for (i = 0; i < 18;)
        {

            tem_pos = {it.x + lightcheckdir[i++], it.y + lightcheckdir[i++], it.z + lightcheckdir[i++]};

            if ((get_block(tem_pos.x, tem_pos.y, tem_pos.z).BlockNID->opaque))
            {
                tempCheckList.insert({(tem_pos.x >> 4), (tem_pos.y >> 4), (tem_pos.z >> 4)});
            }
        }
    }

    for (int n = other_light - 1; n > 0; n--)
    {

        temp_light_list.clear();
        light_list.insert(light_list.end(), light_list_other[n].begin(), light_list_other[n].end());
        light_list_other[n].clear();
        Block tem_block;

        for (auto it : light_list)
        {

            for (i = 0; i < 18;)
            {

                tem_pos = {it.x + lightcheckdir[i++], it.y + lightcheckdir[i++], it.z + lightcheckdir[i++]};
                tem_block = get_block(tem_pos.x, tem_pos.y, tem_pos.z);
                if (!tem_block.BlockNID->opaque && get_light(tem_pos.x, tem_pos.y, tem_pos.z) < n)
                {
                    set_light(tem_pos.x, tem_pos.y, tem_pos.z, n);
                    temp_light_list.push_back({tem_pos.x, tem_pos.y, tem_pos.z});
                }
                if (!(*tem_block.BlockNID == BlockID.Air))
                {
                    tempCheckList.insert({(tem_pos.x >> 4), (tem_pos.y >> 4), (tem_pos.z >> 4)});
                }
            }
        }
        light_list = temp_light_list;
    }

    for (int n = 15; n > 1; n--)
    {
        temp_light_list_remove.clear();
        for (auto it : dlightPos[n - 1])
        {
            light_list_remove.push_back(it);
            set_skylight(it.x, it.y, it.z, 0);
            tem.push_back(it);
            tempCheckList.insert({(it.x >> 4), (it.y >> 4), (it.z >> 4)});
        }

        for (auto it : light_list_remove)
        {
            for (i = 0; i < 18;)
            {

                tem_pos = {it.x + lightcheckdir[i++], it.y + lightcheckdir[i++], it.z + lightcheckdir[i++]};
                tem_light = get_skylight(tem_pos.x, tem_pos.y, tem_pos.z);
                if (tem_light != 0)
                {
                    if (tem_light < n)
                    {
                        set_skylight(tem_pos.x, tem_pos.y, tem_pos.z, 0);
                        temp_light_list_remove.push_back(tem_pos);
                    }
                    else
                    {
                        light_list_other[tem_light - 1].push_back(tem_pos);
                        if (tem_light > other_light)
                        {
                            other_light = tem_light;
                        }
                    }
                }

                if ((get_block(tem_pos.x, tem_pos.y, tem_pos.z).BlockNID->opaque))
                {
                    tempCheckList.insert({(tem_pos.x >> 4), (tem_pos.y >> 4), (tem_pos.z >> 4)});
                }
            }
        }
        light_list_remove = temp_light_list_remove;
    }
    for (auto it : light_list_remove)
    {
        for (i = 0; i < 18;)
        {

            tem_pos = {it.x + lightcheckdir[i++], it.y + lightcheckdir[i++], it.z + lightcheckdir[i++]};

            if ((get_block(tem_pos.x, tem_pos.y, tem_pos.z).BlockNID->opaque))
            {
                tempCheckList.insert({(tem_pos.x >> 4), (tem_pos.y >> 4), (tem_pos.z >> 4)});
            }
        }
    }

    for (int n = other_light - 1; n > 0; n--)
    {

        temp_light_list.clear();
        light_list.insert(light_list.end(), light_list_other[n].begin(), light_list_other[n].end());
        Block tem_block;

        for (auto it : light_list)
        {

            for (i = 0; i < 18;)
            {

                tem_pos = {it.x + lightcheckdir[i++], it.y + lightcheckdir[i++], it.z + lightcheckdir[i++]};
                tem_block = get_block(tem_pos.x, tem_pos.y, tem_pos.z);
                // while (!get_block(tem_pos.x, tem_pos.y, tem_pos.z).BlockNID->opaque && get_skylight(tem_pos.x, tem_pos.y, tem_pos.z) < n &&
                // get_skylight(tem_pos.x, tem_pos.y, tem_pos.z) == 15)
                if (!tem_block.BlockNID->opaque && get_skylight(tem_pos.x, tem_pos.y, tem_pos.z) < n)
                {
                    set_skylight(tem_pos.x, tem_pos.y, tem_pos.z, n);
                    temp_light_list.push_back({tem_pos.x, tem_pos.y, tem_pos.z});
                }
                if (!(*tem_block.BlockNID == BlockID.Air))
                {
                    tempCheckList.insert({(tem_pos.x >> 4), (tem_pos.y >> 4), (tem_pos.z >> 4)});
                }
            }
        }
        light_list = temp_light_list;
    }

    return tempCheckList;
}

void World::delete_block(std::vector<pos3> dblockPos)
{

    std::unordered_set<pos3, ChunkPos3Hash> tempCheckList;
    std::unordered_map<int, std::vector<pos3>> templist;
    int x, y, z;
    for (auto it : dblockPos)
    {
        x = it.x, y = it.y, z = it.z;
        blockdata *temblock = get_block(x, y, z).BlockNID;

        get_chunk(x >> 4, y >> 4, z >> 4).set_pblock(x & 15, y & 15, z & 15, &BlockID.Air);
        if (temblock->light != 0)
        {
            templist[temblock->light - 1].push_back(it);
            get_light_chunk({x >> 4, y >> 4, z >> 4}).lights[temblock->light - 1].erase(it.y << 8 | it.z << 4 | it.x);
        }
        else
        {
            templist[1].push_back(it);
        }
    }
    std::unordered_set<pos3, ChunkPos3Hash> tem_delete_lights_clist = delete_lights(templist);
    tem_delete_lights_clist.insert(tempCheckList.begin(), tempCheckList.end());

    std::unordered_set<pos3, ChunkPos3Hash> tem_recheck_list;
    for (auto it : tem_delete_lights_clist)
    {
        if (it.y >= -4 && it.y < 20)
        {
            Chunk &temchunk = get_chunk(it.x, it.y, it.z);
            if (temchunk.ischeck)
            {
                temchunk.indexIndex = 0;
                temchunk.check(); // 插队
                tem_recheck_list.insert(it);
            }
        }
    }
    check_list_for_main_thread.insert(tem_recheck_list.begin(), tem_recheck_list.end());
}

Block World::get_block_from_blockface_up()
{
    if ((blockface.up.ChunkPos.y) > 19 || (blockface.up.ChunkPos.y) < -4)
        return Air;
    return get_chunk(blockface.up.ChunkPos.x, blockface.up.ChunkPos.y, blockface.up.ChunkPos.z)
        .get_block(blockface.up.OPos.x, blockface.up.OPos.y, blockface.up.OPos.z);
}

Block World::get_block_from_blockface_down()
{
    if ((blockface.down.ChunkPos.y) > 19 || (blockface.down.ChunkPos.y) < -4)
        return Air;
    return get_chunk(blockface.down.ChunkPos.x, blockface.down.ChunkPos.y, blockface.down.ChunkPos.z)
        .get_block(blockface.down.OPos.x, blockface.down.OPos.y, blockface.down.OPos.z);
}

Block World::get_block_from_blockface_left()
{
    if ((blockface.left.ChunkPos.y) > 19 || (blockface.left.ChunkPos.y) < -4)
        return Air;
    return get_chunk(blockface.left.ChunkPos.x, blockface.left.ChunkPos.y, blockface.left.ChunkPos.z)
        .get_block(blockface.left.OPos.x, blockface.left.OPos.y, blockface.left.OPos.z);
}

Block World::get_block_from_blockface_right()
{
    if ((blockface.right.ChunkPos.y) > 19 || (blockface.right.ChunkPos.y) < -4)
        return Air;
    return get_chunk(blockface.right.ChunkPos.x, blockface.right.ChunkPos.y, blockface.right.ChunkPos.z)
        .get_block(blockface.right.OPos.x, blockface.right.OPos.y, blockface.right.OPos.z);
}

Block World::get_block_from_blockface_front()
{
    if ((blockface.front.ChunkPos.y) > 19 || (blockface.front.ChunkPos.y) < -4)
        return Air;
    return get_chunk(blockface.front.ChunkPos.x, blockface.front.ChunkPos.y, blockface.front.ChunkPos.z)
        .get_block(blockface.front.OPos.x, blockface.front.OPos.y, blockface.front.OPos.z);
}

Block World::get_block_from_blockface_back()
{
    if ((blockface.back.ChunkPos.y) > 19 || (blockface.back.ChunkPos.y) < -4)
        return Air;
    return get_chunk(blockface.back.ChunkPos.x, blockface.back.ChunkPos.y, blockface.back.ChunkPos.z)
        .get_block(blockface.back.OPos.x, blockface.back.OPos.y, blockface.back.OPos.z);
}

int World::get_light_from_lightface_up()
{
    pos3 cpos = {lightface.up.x >> 4, lightface.up.y >> 4, lightface.up.z >> 4};
    pos3 opos = {lightface.up.x & 15, lightface.up.y & 15, lightface.up.z & 15};
    return std::max(get_light_chunk(cpos).get_light_level(opos.x, opos.y, opos.z), get_light_chunk(cpos).get_skylight_level(opos.x, opos.y, opos.z));
}

int World::get_light_from_lightface_down()
{
    pos3 cpos = {lightface.down.x >> 4, lightface.down.y >> 4, lightface.down.z >> 4};
    pos3 opos = {lightface.down.x & 15, lightface.down.y & 15, lightface.down.z & 15};
    return std::max(get_light_chunk(cpos).get_light_level(opos.x, opos.y, opos.z), get_light_chunk(cpos).get_skylight_level(opos.x, opos.y, opos.z));
}

int World::get_light_from_lightface_left()
{
    pos3 cpos = {lightface.left.x >> 4, lightface.left.y >> 4, lightface.left.z >> 4};
    pos3 opos = {lightface.left.x & 15, lightface.left.y & 15, lightface.left.z & 15};
    return std::max(get_light_chunk(cpos).get_light_level(opos.x, opos.y, opos.z), get_light_chunk(cpos).get_skylight_level(opos.x, opos.y, opos.z));
}

int World::get_light_from_lightface_right()
{
    pos3 cpos = {lightface.right.x >> 4, lightface.right.y >> 4, lightface.right.z >> 4};
    pos3 opos = {lightface.right.x & 15, lightface.right.y & 15, lightface.right.z & 15};
    return std::max(get_light_chunk(cpos).get_light_level(opos.x, opos.y, opos.z), get_light_chunk(cpos).get_skylight_level(opos.x, opos.y, opos.z));
}

int World::get_light_from_lightface_front()
{
    pos3 cpos = {lightface.front.x >> 4, lightface.front.y >> 4, lightface.front.z >> 4};
    pos3 opos = {lightface.front.x & 15, lightface.front.y & 15, lightface.front.z & 15};
    return std::max(get_light_chunk(cpos).get_light_level(opos.x, opos.y, opos.z), get_light_chunk(cpos).get_skylight_level(opos.x, opos.y, opos.z));
}

int World::get_light_from_lightface_back()
{
    pos3 cpos = {lightface.back.x >> 4, lightface.back.y >> 4, lightface.back.z >> 4};
    pos3 opos = {lightface.back.x & 15, lightface.back.y & 15, lightface.back.z & 15};
    return std::max(get_light_chunk(cpos).get_light_level(opos.x, opos.y, opos.z), get_light_chunk(cpos).get_skylight_level(opos.x, opos.y, opos.z));
}

Block World::get_block_from_blockface_data(copos pos)
{
    if ((pos.ChunkPos.y) > 19 || (pos.ChunkPos.y) < -4)
        return Air;
    return get_chunk(pos.ChunkPos.x, pos.ChunkPos.y, pos.ChunkPos.z).get_block(pos.OPos.x, pos.OPos.y, pos.OPos.z);
}

int World::get_light_level_from_light(copos pos)
{
    return std::max(get_light_chunk(pos.ChunkPos).get_light_level(pos.OPos.x, pos.OPos.y, pos.OPos.z),
                    get_light_chunk(pos.ChunkPos).get_skylight_level(pos.OPos.x, pos.OPos.y, pos.OPos.z));
}

pos4 World::get_ao_from_blockface_up()
{
    pos4 color;
    int tem;
    bool o_ufl = get_block_from_blockface_data(light.ufl).BlockNID->opaque;
    bool o_ufr = get_block_from_blockface_data(light.ufr).BlockNID->opaque;
    bool o_ubr = get_block_from_blockface_data(light.ubr).BlockNID->opaque;
    bool o_ubl = get_block_from_blockface_data(light.ubl).BlockNID->opaque;
    bool o_uf = get_block_from_blockface_data(light.uf).BlockNID->opaque;
    bool o_ul = get_block_from_blockface_data(light.ul).BlockNID->opaque;
    bool o_ur = get_block_from_blockface_data(light.ur).BlockNID->opaque;
    bool o_ub = get_block_from_blockface_data(light.ub).BlockNID->opaque;
    int l_uf = get_light_level_from_light(light.uf);
    int l_ul = get_light_level_from_light(light.ul);
    int l_ur = get_light_level_from_light(light.ur);
    int l_ub = get_light_level_from_light(light.ub);

    int clight = get_light_level_from_light(light.u);

    if (!o_ufl && (!o_uf || !o_ul)) // 检查斜角是否为空气
    {
        tem = (clight + get_light_level_from_light(light.ufl)) / 2; // 该面的这个角的亮度为该面的表面亮度与斜角的亮度的平均值
    }
    else if (!o_uf && !o_ul) // 检查边是否都为空气
    {
        tem = (l_uf + l_ul) / 2; // 该面的这个角的两边的位置亮度的平均值
    }
    else
    {
        tem = ((!o_uf ? l_uf : clight) + (!o_ul ? l_ul : clight)) / 2; // 哪边有方块那杯就不使用平滑
    }
    color.x = tem + vertexAO(o_uf, o_ul, o_ufl);

    if (!o_ufr && (!o_uf || !o_ur)) // 检查斜角是否为空气
    {
        tem = (clight + get_light_level_from_light(light.ufr)) / 2; // 该面的这个角的亮度为该面的表面亮度与斜角的亮度的平均值
    }
    else if (!o_uf && !o_ur) // 检查边是否都为空气
    {
        tem = (l_uf + l_ur) / 2; // 该面的这个角的两边的位置亮度的平均值
    }
    else
    {
        tem = ((!o_uf ? l_uf : clight) + (!o_ur ? l_ur : clight)) / 2; // 哪边有方块那杯就不使用平滑
    }
    color.y = tem + vertexAO(o_uf, o_ur, o_ufr);

    if (!o_ubr && (!o_ub || !o_ur)) // 检查斜角是否为空气
    {
        tem = (clight + get_light_level_from_light(light.ubr)) / 2; // 该面的这个角的亮度为该面的表面亮度与斜角的亮度的平均值
    }
    else if (!o_ub && !o_ur) // 检查边是否都为空气
    {
        tem = (l_ub + l_ur) / 2; // 该面的这个角的两边的位置亮度的平均值
    }
    else
    {
        tem = ((!o_ub ? l_ub : clight) + (!o_ur ? l_ur : clight)) / 2; // 哪边有方块那杯就不使用平滑
    }
    color.z = tem + vertexAO(o_ub, o_ur, o_ubr);

    if (!o_ubl && (!o_ub || !o_ul)) // 检查斜角是否为空气
    {
        tem = (clight + get_light_level_from_light(light.ubl)) / 2; // 该面的这个角的亮度为该面的表面亮度与斜角的亮度的平均值
    }
    else if (!o_ub && !o_ul) // 检查边是否都为空气
    {
        tem = (l_ub + l_ul) / 2; // 该面的这个角的两边的位置亮度的平均值
    }
    else
    {
        tem = ((!o_ub ? l_ub : clight) + (!o_ul ? l_ul : clight)) / 2; // 哪边有方块那杯就不使用平滑
    }
    color.w = tem + vertexAO(o_ub, o_ul, o_ubl);

    return color;
}

pos4 World::get_ao_from_blockface_down()
{
    pos4 color;
    int tem;
    bool o_dbl = get_block_from_blockface_data(light.dbl).BlockNID->opaque;
    bool o_dbr = get_block_from_blockface_data(light.dbr).BlockNID->opaque;
    bool o_dfr = get_block_from_blockface_data(light.dfr).BlockNID->opaque;
    bool o_dfl = get_block_from_blockface_data(light.dfl).BlockNID->opaque;
    bool o_db = get_block_from_blockface_data(light.db).BlockNID->opaque;
    bool o_dl = get_block_from_blockface_data(light.dl).BlockNID->opaque;
    bool o_dr = get_block_from_blockface_data(light.dr).BlockNID->opaque;
    bool o_df = get_block_from_blockface_data(light.df).BlockNID->opaque;
    int l_db = get_light_level_from_light(light.db);
    int l_dl = get_light_level_from_light(light.dl);
    int l_dr = get_light_level_from_light(light.dr);
    int l_df = get_light_level_from_light(light.df);

    int clight = get_light_level_from_light(light.d);

    if (!o_dbl && (!o_db || !o_dl)) // 检查斜角是否为空气
    {
        tem = (clight + get_light_level_from_light(light.dbl)) / 2; // 该面的这个角的亮度为该面的表面亮度与斜角的亮度的平均值
    }
    else if (!o_db && !o_dl) // 检查边是否都为空气
    {
        tem = (l_db + l_dl) / 2; // 该面的这个角的两边的位置亮度的平均值
    }
    else
    {
        tem = ((!o_db ? l_db : clight) + (!o_dl ? l_dl : clight)) / 2; // 哪边有方块那杯就不使用平滑
    }
    color.x = tem + vertexAO(o_db, o_dl, o_dbl);

    if (!o_dbr && (!o_db || !o_dr)) // 检查斜角是否为空气
    {
        tem = (clight + get_light_level_from_light(light.dbr)) / 2; // 该面的这个角的亮度为该面的表面亮度与斜角的亮度的平均值
    }
    else if (!o_db && !o_dr) // 检查边是否都为空气
    {
        tem = (l_db + l_dr) / 2; // 该面的这个角的两边的位置亮度的平均值
    }
    else
    {
        tem = ((!o_db ? l_db : clight) + (!o_dr ? l_dr : clight)) / 2; // 哪边有方块那杯就不使用平滑
    }
    color.y = tem + vertexAO(o_db, o_dr, o_dbr);

    if (!o_dfr && (!o_df || !o_dr)) // 检查斜角是否为空气
    {
        tem = (clight + get_light_level_from_light(light.dfr)) / 2; // 该面的这个角的亮度为该面的表面亮度与斜角的亮度的平均值
    }
    else if (!o_df && !o_dr) // 检查边是否都为空气
    {
        tem = (l_df + l_dr) / 2; // 该面的这个角的两边的位置亮度的平均值
    }
    else
    {
        tem = ((!o_df ? l_df : clight) + (!o_dr ? l_dr : clight)) / 2; // 哪边有方块那杯就不使用平滑
    }
    color.z = tem + vertexAO(o_df, o_dr, o_dfr);

    if (!o_dfl && (!o_df || !o_dl)) // 检查斜角是否为空气
    {
        tem = (clight + get_light_level_from_light(light.dfl)) / 2; // 该面的这个角的亮度为该面的表面亮度与斜角的亮度的平均值
    }
    else if (!o_df && !o_dl) // 检查边是否都为空气
    {
        tem = (l_df + l_dl) / 2; // 该面的这个角的两边的位置亮度的平均值
    }
    else
    {
        tem = ((!o_df ? l_df : clight) + (!o_dl ? l_dl : clight)) / 2; // 哪边有方块那杯就不使用平滑
    }
    color.w = tem + vertexAO(o_df, o_dl, o_dfl);

    return color;
}

pos4 World::get_ao_from_blockface_left()
{

    pos4 color;
    int tem;
    bool o_dbl = get_block_from_blockface_data(light.dbl).BlockNID->opaque;
    bool o_dfl = get_block_from_blockface_data(light.dfl).BlockNID->opaque;
    bool o_ufl = get_block_from_blockface_data(light.ufl).BlockNID->opaque;
    bool o_ubl = get_block_from_blockface_data(light.ubl).BlockNID->opaque;
    bool o_bl = get_block_from_blockface_data(light.bl).BlockNID->opaque;
    bool o_dl = get_block_from_blockface_data(light.dl).BlockNID->opaque;
    bool o_fl = get_block_from_blockface_data(light.fl).BlockNID->opaque;
    bool o_ul = get_block_from_blockface_data(light.ul).BlockNID->opaque;
    int l_bl = get_light_level_from_light(light.bl);
    int l_dl = get_light_level_from_light(light.dl);
    int l_fl = get_light_level_from_light(light.fl);
    int l_ul = get_light_level_from_light(light.ul);

    int clight = get_light_level_from_light(light.l);

    if (!o_dbl && (!o_bl || !o_dl)) // 检查斜角是否为空气
    {
        tem = (clight + get_light_level_from_light(light.dbl)) / 2; // 该面的这个角的亮度为该面的表面亮度与斜角的亮度的平均值
    }
    else if (!o_bl && !o_dl) // 检查边是否都为空气
    {
        tem = (l_bl + l_dl) / 2; // 该面的这个角的两边的位置亮度的平均值
    }
    else
    {
        tem = ((!o_bl ? l_bl : clight) + (!o_dl ? l_dl : clight)) / 2; // 哪边有方块那杯就不使用平滑
    }
    color.x = tem + vertexAO(o_bl, o_dl, o_dbl);

    if (!o_dfl && (!o_fl || !o_dl)) // 检查斜角是否为空气
    {
        tem = (clight + get_light_level_from_light(light.dfl)) / 2; // 该面的这个角的亮度为该面的表面亮度与斜角的亮度的平均值
    }
    else if (!o_fl && !o_dl) // 检查边是否都为空气
    {
        tem = (l_fl + l_dl) / 2; // 该面的这个角的两边的位置亮度的平均值
    }
    else
    {
        tem = ((!o_fl ? l_fl : clight) + (!o_dl ? l_dl : clight)) / 2; // 哪边有方块那杯就不使用平滑
    }
    color.y = tem + vertexAO(o_fl, o_dl, o_dfl);

    if (!o_ufl && (!o_fl || !o_ul)) // 检查斜角是否为空气
    {
        tem = (clight + get_light_level_from_light(light.ufl)) / 2; // 该面的这个角的亮度为该面的表面亮度与斜角的亮度的平均值
    }
    else if (!o_fl && !o_ul) // 检查边是否都为空气
    {
        tem = (l_fl + l_ul) / 2; // 该面的这个角的两边的位置亮度的平均值
    }
    else
    {
        tem = ((!o_fl ? l_fl : clight) + (!o_ul ? l_ul : clight)) / 2; // 哪边有方块那杯就不使用平滑
    }
    color.z = tem + vertexAO(o_fl, o_ul, o_ufl);

    if (!o_ubl && (!o_bl || !o_ul)) // 检查斜角是否为空气
    {
        tem = (clight + get_light_level_from_light(light.ubl)) / 2; // 该面的这个角的亮度为该面的表面亮度与斜角的亮度的平均值
    }
    else if (!o_bl && !o_ul) // 检查边是否都为空气
    {
        tem = (l_bl + l_ul) / 2; // 该面的这个角的两边的位置亮度的平均值
    }
    else
    {
        tem = ((!o_bl ? l_bl : clight) + (!o_ul ? l_ul : clight)) / 2; // 哪边有方块那杯就不使用平滑
    }
    color.w = tem + vertexAO(o_bl, o_ul, o_ubl);

    return color;
}

pos4 World::get_ao_from_blockface_right()
{

    pos4 color;
    int tem;
    bool o_drf = get_block_from_blockface_data(light.dfr).BlockNID->opaque;
    bool o_dbr = get_block_from_blockface_data(light.dbr).BlockNID->opaque;
    bool o_ubr = get_block_from_blockface_data(light.ubr).BlockNID->opaque;
    bool o_ufr = get_block_from_blockface_data(light.ufr).BlockNID->opaque;
    bool o_fr = get_block_from_blockface_data(light.fr).BlockNID->opaque;
    bool o_dr = get_block_from_blockface_data(light.dr).BlockNID->opaque;
    bool o_br = get_block_from_blockface_data(light.br).BlockNID->opaque;
    bool o_ur = get_block_from_blockface_data(light.ur).BlockNID->opaque;
    int l_fr = get_light_level_from_light(light.fr);
    int l_dr = get_light_level_from_light(light.dr);
    int l_br = get_light_level_from_light(light.br);
    int l_ur = get_light_level_from_light(light.ur);

    int clight = get_light_level_from_light(light.r);

    if (!o_drf && (!o_fr || !o_dr)) // 检查斜角是否为空气
    {
        tem = (clight + get_light_level_from_light(light.dfr)) / 2; // 该面的这个角的亮度为该面的表面亮度与斜角的亮度的平均值
    }
    else if (!o_fr && !o_dr) // 检查边是否都为空气
    {
        tem = (l_fr + l_dr) / 2; // 该面的这个角的两边的位置亮度的平均值
    }
    else
    {
        tem = ((!o_fr ? l_fr : clight) + (!o_dr ? l_dr : clight)) / 2; // 哪边有方块那杯就不使用平滑
    }
    color.x = tem + vertexAO(o_fr, o_dr, o_drf);

    if (!o_dbr && (!o_br || !o_dr)) // 检查斜角是否为空气
    {
        tem = (clight + get_light_level_from_light(light.dbr)) / 2; // 该面的这个角的亮度为该面的表面亮度与斜角的亮度的平均值
    }
    else if (!o_br && !o_dr) // 检查边是否都为空气
    {
        tem = (l_br + l_dr) / 2; // 该面的这个角的两边的位置亮度的平均值
    }
    else
    {
        tem = ((!o_br ? l_br : clight) + (!o_dr ? l_dr : clight)) / 2; // 哪边有方块那杯就不使用平滑
    }
    color.y = tem + vertexAO(o_br, o_dr, o_dbr);

    if (!o_ubr && (!o_br || !o_ur)) // 检查斜角是否为空气
    {
        tem = (clight + get_light_level_from_light(light.ubr)) / 2; // 该面的这个角的亮度为该面的表面亮度与斜角的亮度的平均值
    }
    else if (!o_br && !o_ur) // 检查边是否都为空气
    {
        tem = (l_br + l_ur) / 2; // 该面的这个角的两边的位置亮度的平均值
    }
    else
    {
        tem = ((!o_br ? l_br : clight) + (!o_ur ? l_ur : clight)) / 2; // 哪边有方块那杯就不使用平滑
    }
    color.z = tem + vertexAO(o_br, o_ur, o_ubr);

    if (!o_ufr && (!o_fr || !o_ur)) // 检查斜角是否为空气
    {
        tem = (clight + get_light_level_from_light(light.ufr)) / 2; // 该面的这个角的亮度为该面的表面亮度与斜角的亮度的平均值
    }
    else if (!o_fr && !o_ur) // 检查边是否都为空气
    {
        tem = (l_fr + l_ur) / 2; // 该面的这个角的两边的位置亮度的平均值
    }
    else
    {
        tem = ((!o_fr ? l_fr : clight) + (!o_ur ? l_ur : clight)) / 2; // 哪边有方块那杯就不使用平滑
    }
    color.w = tem + vertexAO(o_fr, o_ur, o_ufr);

    return color;
}

pos4 World::get_ao_from_blockface_front()
{

    pos4 color;
    int tem;
    bool o_dfl = get_block_from_blockface_data(light.dfl).BlockNID->opaque;
    bool o_dfr = get_block_from_blockface_data(light.dfr).BlockNID->opaque;
    bool o_ufr = get_block_from_blockface_data(light.ufr).BlockNID->opaque;
    bool o_ufl = get_block_from_blockface_data(light.ufl).BlockNID->opaque;
    bool o_fl = get_block_from_blockface_data(light.fl).BlockNID->opaque;
    bool o_df = get_block_from_blockface_data(light.df).BlockNID->opaque;
    bool o_fr = get_block_from_blockface_data(light.fr).BlockNID->opaque;
    bool o_uf = get_block_from_blockface_data(light.uf).BlockNID->opaque;
    int l_fl = get_light_level_from_light(light.fl);
    int l_df = get_light_level_from_light(light.df);
    int l_fr = get_light_level_from_light(light.fr);
    int l_uf = get_light_level_from_light(light.uf);

    int clight = get_light_level_from_light(light.f);

    if (!o_dfl && (!o_fl || !o_df)) // 检查斜角是否为空气
    {
        tem = (clight + get_light_level_from_light(light.dfl)) / 2; // 该面的这个角的亮度为该面的表面亮度与斜角的亮度的平均值
    }
    else if (!o_fl && !o_df) // 检查边是否都为空气
    {
        tem = (l_fl + l_df) / 2; // 该面的这个角的两边的位置亮度的平均值
    }
    else
    {
        tem = ((!o_fl ? l_fl : clight) + (!o_df ? l_df : clight)) / 2; // 哪边有方块那杯就不使用平滑
    }
    color.x = tem + vertexAO(o_fl, o_df, o_dfl);

    if (!o_dfr && (!o_fr || !o_df)) // 检查斜角是否为空气
    {
        tem = (clight + get_light_level_from_light(light.dfr)) / 2; // 该面的这个角的亮度为该面的表面亮度与斜角的亮度的平均值
    }
    else if (!o_fr && !o_df) // 检查边是否都为空气
    {
        tem = (l_fr + l_df) / 2; // 该面的这个角的两边的位置亮度的平均值
    }
    else
    {
        tem = ((!o_fr ? l_fr : clight) + (!o_df ? l_df : clight)) / 2; // 哪边有方块那杯就不使用平滑
    }
    color.y = tem + vertexAO(o_fr, o_df, o_dfr);

    if (!o_ufr && (!o_fr || !o_uf)) // 检查斜角是否为空气
    {
        tem = (clight + get_light_level_from_light(light.ufr)) / 2; // 该面的这个角的亮度为该面的表面亮度与斜角的亮度的平均值
    }
    else if (!o_fr && !o_uf) // 检查边是否都为空气
    {
        tem = (l_fr + l_uf) / 2; // 该面的这个角的两边的位置亮度的平均值
    }
    else
    {
        tem = ((!o_fr ? l_fr : clight) + (!o_uf ? l_uf : clight)) / 2; // 哪边有方块那杯就不使用平滑
    }
    color.z = tem + vertexAO(o_fr, o_uf, o_ufr);

    if (!o_ufl && (!o_fl || !o_uf)) // 检查斜角是否为空气
    {
        tem = (clight + get_light_level_from_light(light.ufl)) / 2; // 该面的这个角的亮度为该面的表面亮度与斜角的亮度的平均值
    }
    else if (!o_fl && !o_uf) // 检查边是否都为空气
    {
        tem = (l_fl + l_uf) / 2; // 该面的这个角的两边的位置亮度的平均值
    }
    else
    {
        tem = ((!o_fl ? l_fl : clight) + (!o_uf ? l_uf : clight)) / 2; // 哪边有方块那杯就不使用平滑
    }
    color.w = tem + vertexAO(o_fl, o_uf, o_ufl);
    return color;
}

pos4 World::get_ao_from_blockface_back()
{

    pos4 color;
    int tem;
    bool o_dbr = get_block_from_blockface_data(light.dbr).BlockNID->opaque;
    bool o_dbl = get_block_from_blockface_data(light.dbl).BlockNID->opaque;
    bool o_ubl = get_block_from_blockface_data(light.ubl).BlockNID->opaque;
    bool o_ubr = get_block_from_blockface_data(light.ubr).BlockNID->opaque;
    bool o_db = get_block_from_blockface_data(light.db).BlockNID->opaque;
    bool o_br = get_block_from_blockface_data(light.br).BlockNID->opaque;
    bool o_bl = get_block_from_blockface_data(light.bl).BlockNID->opaque;
    bool o_ub = get_block_from_blockface_data(light.ub).BlockNID->opaque;
    int l_db = get_light_level_from_light(light.db);
    int l_br = get_light_level_from_light(light.br);
    int l_bl = get_light_level_from_light(light.bl);
    int l_ub = get_light_level_from_light(light.ub);

    int clight = get_light_level_from_light(light.b);

    if (!o_dbr && (!o_db || !o_br)) // 检查斜角是否为空气
    {
        tem = (clight + get_light_level_from_light(light.dbr)) / 2; // 该面的这个角的亮度为该面的表面亮度与斜角的亮度的平均值
    }
    else if (!o_db && !o_br) // 检查边是否都为空气
    {
        tem = (l_db + l_br) / 2; // 该面的这个角的两边的位置亮度的平均值
    }
    else
    {
        tem = ((!o_db ? l_db : clight) + (!o_br ? l_br : clight)) / 2; // 哪边有方块那杯就不使用平滑
    }
    color.x = tem + vertexAO(o_db, o_br, o_dbr);

    if (!o_dbl && (!o_bl || !o_db)) // 检查斜角是否为空气
    {
        tem = (clight + get_light_level_from_light(light.dbl)) / 2; // 该面的这个角的亮度为该面的表面亮度与斜角的亮度的平均值
    }
    else if (!o_bl && !o_db) // 检查边是否都为空气
    {
        tem = (l_bl + l_db) / 2; // 该面的这个角的两边的位置亮度的平均值
    }
    else
    {
        tem = ((!o_bl ? l_bl : clight) + (!o_db ? l_db : clight)) / 2; // 哪边有方块那杯就不使用平滑
    }
    color.y = tem + vertexAO(o_bl, o_db, o_dbl);

    if (!o_ubl && (!o_bl || !o_ub)) // 检查斜角是否为空气
    {
        tem = (clight + get_light_level_from_light(light.ubl)) / 2; // 该面的这个角的亮度为该面的表面亮度与斜角的亮度的平均值
    }
    else if (!o_bl && !o_ub) // 检查边是否都为空气
    {
        tem = (l_bl + l_ub) / 2; // 该面的这个角的两边的位置亮度的平均值
    }
    else
    {
        tem = ((!o_bl ? l_bl : clight) + (!o_ub ? l_ub : clight)) / 2; // 哪边有方块那杯就不使用平滑
    }
    color.z = tem + vertexAO(o_bl, o_ub, o_ubl);

    if (!o_ubr && (!o_br || !o_ub)) // 检查斜角是否为空气
    {
        tem = (clight + get_light_level_from_light(light.ubr)) / 2; // 该面的这个角的亮度为该面的表面亮度与斜角的亮度的平均值
    }
    else if (!o_br && !o_ub) // 检查边是否都为空气
    {
        tem = (l_br + l_ub) / 2; // 该面的这个角的两边的位置亮度的平均值
    }
    else
    {
        tem = ((!o_br ? l_br : clight) + (!o_ub ? l_ub : clight)) / 2; // 哪边有方块那杯就不使用平滑
    }
    color.w = tem + vertexAO(o_br, o_ub, o_ubr);
    return color;
}

void World::loadthread()
{
    cltm();
    long seed = 584903239;
    // std::cin >> seed;
    terrain.set_seed(seed);

    std::thread load_world_thread(&World::loading, this);
    load_world_thread.detach();
    std::thread Flood_light_thread(&World::light_flooding, this);
    Flood_light_thread.detach();
}

void World::light_flooding()
{
    int j;
    while (gamerunning)
    {
        // std::this_thread::sleep_for(std::chrono::milliseconds(10));

        for (int i = 0; i < chunkda; i++)
        {

            if (light_flood_offset != toffset)
            {
                light_flood_offset = toffset;
                break;
            }
            j = linetmap[i];
            std::unique_lock<std::mutex> lock(muex);
            if (lightflood(j % chunkdl + light_flood_offset.x, j / chunkdl + light_flood_offset.y))
            {
            }
        }
    }
}

bool World::lightflood(int x, int z)
{
    int n = 0, nz, nx;
    for (nx = -1; nx <= 1; nx++)
    {
        for (nz = -1; nz <= 1; nz++)
        {

            if (!checklist.empty())
            {

                delete_block(checklist);
                checklist.clear();
            }

            if (flood(x + nx, z + nz)) // 加载世界所有亮度
            {
                n++;
            }
        }
    }
    if (n == 9)
    {
        return updata(x, z);
    }
    else
    {
        return false;
    }
}

bool World::flood(int x, int z)
{

    ChunkXZ &tempchunkxz = get_chunkxz(x, z);
    if (!tempchunkxz.isflood && tempchunkxz.ischecksky)
    {
        pos3 wpos, tem_pos;
        int i, y, n;
        for (y = 0; y < 24; y++)
        {
            LightsLevel &temchunk = tempchunkxz.lightchunks[y + 1];
            std::vector<pos3> light_list, temp_light_list;
            for (n = 14; n > 0; n--)
            {

                temp_light_list.clear();
                for (auto l : temchunk.lights[n])
                {
                    light_list.push_back({(l & 15) + (x << 4), (l >> 8) + ((y - 4) << 4), (l >> 4 & 15) + (z << 4)});
                }

                for (auto it : light_list)
                {
                    for (i = 0; i < 18;)
                    {
                        tem_pos = {it.x + lightcheckdir[i++], it.y + lightcheckdir[i++], it.z + lightcheckdir[i++]};
                        if (!get_block(tem_pos.x, tem_pos.y, tem_pos.z).BlockNID->opaque && get_light(tem_pos.x, tem_pos.y, tem_pos.z) < n)
                        {
                            set_light(tem_pos.x, tem_pos.y, tem_pos.z, n);
                            temp_light_list.push_back({tem_pos.x, tem_pos.y, tem_pos.z});
                        }
                    }
                }
                light_list = temp_light_list;
            }
            light_list.clear();
            for (auto l : temchunk.skylights)
            {
                light_list.push_back({(l & 15) + (x << 4), (l >> 8) + ((y - 4) << 4), (l >> 4 & 15) + (z << 4)});
            }
            for (n = 13; n > 0; n--)
            {

                temp_light_list.clear();

                for (auto it : light_list)
                {
                    for (i = 0; i < 18;)
                    {
                        tem_pos = {it.x + lightcheckdir[i++], it.y + lightcheckdir[i++], it.z + lightcheckdir[i++]};
                        if (!get_block(tem_pos.x, tem_pos.y, tem_pos.z).BlockNID->opaque && get_skylight(tem_pos.x, tem_pos.y, tem_pos.z) < n)
                        {
                            set_skylight(tem_pos.x, tem_pos.y, tem_pos.z, n);
                            temp_light_list.push_back({tem_pos.x, tem_pos.y, tem_pos.z});
                        }
                    }
                }
                light_list = temp_light_list;
            }
        }
        tempchunkxz.isflood = true;
    }
    return tempchunkxz.isflood;
}

void World::loading()
{
    runningMutex.lock();
    int j;
    while (gamerunning)
    {
        for (int i = 0; i < chunkda; i++)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));

            if (load_offset != toffset)
            {
                load_offset = toffset;
                break;
            }

            j = linetmap[i];
            std::unique_lock<std::mutex> lock(muex);
            if (loadchunk(j % chunkdl + load_offset.x, j / chunkdl + load_offset.y))
            {
            }
        }
    }
    runningMutex.unlock();
}

void World::endingThread()
{
    gamerunning = false;
}

void World::endSignal()
{
    runningMutex.lock();
    runningMutex.unlock();

}

bool World::loadchunk(int x, int z)
{
    int nx, nz;
    for (nx = -1; nx <= 1; nx++)
    {
        for (nz = -1; nz <= 1; nz++)
        {

            loadnewchunk(x + nx, z + nz); // 加载世界所有方块
        }
    }

    check_skylights(x, z);
    return false;
}

void World::loadnewchunk(int x, int z)
{
    ChunkXZ &tempchunkxz = get_chunkxz(x, z);

    if (!tempchunkxz.isload)
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        FastNoiseLite noise(7843921);
        noise.SetFractalOctaves(5);
        noise.SetFractalLacunarity(1.75);
        noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
        noise.SetFractalType(FastNoiseLite::FractalType_FBm);

        pos3 wpos;
        double max16, perlin1 = 0, perlin4 = 0, perlin16 = 0, perlin3d = 0;
        int ni, nx, nz;
        tempchunkxz.PosXZ = {x, z};
        memset(tempchunkxz.highmap, -80, sizeof(tempchunkxz.highmap));

        terrain.chunk2dinit({x, z}, 1);
        terrain.chunk2dinit({x, z}, 4);
        terrain.chunk2dinit({x, z}, 16);
        for (int i = 0; i < 24; i++)
        {

            Chunk &temchunk = tempchunkxz.chunkxz[i];
            LightsLevel &temlightchunk = tempchunkxz.lightchunks[i + 1];
            temchunk.ChunkPos = {x, i - 4, z};
            temchunk._world = this;
            temchunk._light = &tempchunkxz.lightchunks[i + 1];
            terrain.chunk3dinit({x, i, z}, 4);
            terrain.chunk3dinit({x, i, z}, 2);
            for (ni = 0; ni < 256; ni++)
            {

                nx = ni & 15;
                nz = ni >> 4;

                int t = nz << 4 | nx;

                perlin1 = noise.GetNoise((float)(nx + (x << 4)), (float)(nz + (z << 4)));

                // perlin1 = terrain.get_perlin_noise(1, {nx / 16.0, nz / 16.0});
                // perlin4 = terrain.get_perlin_noise(4, {nx / 64.0 + terrain.fract(x / 4.0), nz / 64.0 + terrain.fract(z / 4.0)});
                // perlin16 = terrain.get_perlin_noise(16, {nx / 256.0 + terrain.fract(x / 16.0), nz / 256.0 + terrain.fract(z / 16.0)});
                // max16 = perlin1 > max16 ? perlin1 : max16;

                if (perlin4 < 1.0 && perlin4 >= 0.35)
                {
                    perlin4 = (perlin4 - 0.35) * 2 + 0.8;
                }
                if (perlin4 > 0.3 && perlin4 < 0.35)
                {
                    perlin4 = (perlin4 - 0.3) * 10 + 0.3;
                }
                int th = static_cast<int>(10 * perlin16 + perlin1 * 100 + perlin4 * 20);
                for (int ny = 0; ny < 16; ny++)
                {

                    wpos = {nx + (x << 4), ny + (i << 4), nz + (z << 4)};
                    if ((wpos.x == 13 && wpos.z == 13) || (wpos.x == 17 && wpos.z == 17))
                    {
                        temchunk.set_pblock(nx, ny, nz, &BlockID.GlowStone);
                    }
                    else if (wpos.y == 0)
                    {
                        temchunk.set_pblock(nx, ny, nz, &BlockID.Bedrock);
                    }
                    else if (wpos.y < 125 + th)
                    {
                        // perlin3d = terrain.get_perlin_noise_3d(2, {nx / 32.0 + terrain.fract(x / 2.0), ny / 32.0 + terrain.fract((i) / 2.0), nz / 32.0 +
                        // terrain.fract(z / 2.0)});
                        perlin3d = noise.GetNoise((float)(nx + (x << 4)), (float)(ny + (i << 4)), (float)(nz + (z << 4)));
                        // perlin3d = perlin3d * 0.3 + terrain.get_perlin_noise_3d(4, {nx / 64.0 + terrain.fract(x / 4.0), ny / 64.0 + terrain.fract((i) / 4.0),
                        // nz / 64.0 + terrain.fract(z / 4.0)}) * 0.7;
                        if (perlin3d < -0.3)
                        {
                            temchunk.set_pblock(nx, ny, nz, &BlockID.Air);
                        }
                        else
                        {
                            temchunk.set_pblock(nx, ny, nz, &BlockID.Stone);
                        }
                    }
                    else if (wpos.y < 128 + th)
                    {
                        // perlin3d = terrain.get_perlin_noise_3d(2, {nx / 32.0 + terrain.fract(x / 2.0), ny / 32.0 + terrain.fract((i) / 2.0), nz / 32.0 +
                        // terrain.fract(z / 2.0)});
                        perlin3d = noise.GetNoise((float)(nx + (x << 4)), (float)(ny + (i << 4)), (float)(nz + (z << 4)));
                        // perlin3d = perlin3d * 0.3 + terrain.get_perlin_noise_3d(4, {nx / 64.0 + terrain.fract(x / 4.0), ny / 64.0 + terrain.fract((i) / 4.0),
                        // nz / 64.0 + terrain.fract(z / 4.0)}) * 0.7;
                        if (perlin3d < -0.3)
                        {
                            temchunk.set_pblock(nx, ny, nz, &BlockID.Air);
                        }
                        else
                        {
                            temchunk.set_pblock(nx, ny, nz, &BlockID.Dirt);
                        }
                    }
                    else if (wpos.y == 128 + th)
                    {
                        // perlin3d = terrain.get_perlin_noise_3d(2, {nx / 32.0 + terrain.fract(x / 2.0), ny / 32.0 + terrain.fract((i) / 2.0), nz / 32.0 +
                        // terrain.fract(z / 2.0)});
                        perlin3d = noise.GetNoise((float)(nx + (x << 4)), (float)(ny + (i << 4)), (float)(nz + (z << 4)));
                        // perlin3d = perlin3d * 0.3 + terrain.get_perlin_noise_3d(4, {nx / 64.0 + terrain.fract(x / 4.0), ny / 64.0 + terrain.fract((i) / 4.0),
                        // nz / 64.0 + terrain.fract(z / 4.0)}) * 0.7;
                        if (perlin3d < -0.3)
                        {
                            temchunk.set_pblock(nx, ny, nz, &BlockID.Air);
                        }
                        else
                        {
                            temchunk.set_pblock(nx, ny, nz, &BlockID.Grass_Block);
                        }
                    }
                    else
                    {
                        temchunk.set_pblock(nx, ny, nz, &BlockID.Air);
                    }
                    if (temchunk.get_block(nx, ny, nz).BlockNID->opaque)
                    {
                        tempchunkxz.highmap[t] = std::max(tempchunkxz.highmap[t], ((i - 4) << 4) + ny);
                    }
                }
            }
        }
        // std::cout << max16 << std::endl;
        tempchunkxz.isload = true;

        fps = static_cast<int>(1 / (static_cast<float>(glfwGetTime()) - currentFrame));
    }
}

bool World::updata(int x, int z)
{
    ChunkXZ &tempchunkxz = get_chunkxz(x, z);
    if (!tempchunkxz.ischeck)
    {
        for (int i = 0; i < 24; i++)
        {
            tempchunkxz.chunkxz[i].check();
        }
        tempchunkxz.ischeck = true;
        return true;
    }
    return false;
}

void World::check_skylights(int x, int z)
{

    ChunkXZ &tempchunkxz = get_chunkxz(x, z);
    if (!tempchunkxz.ischecksky)
    {
        for (int nx = -1; nx < 17; nx++)
        {
            for (int nz = -1; nz < 17; nz++)
            {

                int tx = (x << 4) + nx;
                int tz = (z << 4) + nz;
                int h = get_height_highestmap(tx, tz);
                int right_side_h = get_height_highestmap(tx + 1, tz);
                int left_side_h = get_height_highestmap(tx - 1, tz);
                int front_side_h = get_height_highestmap(tx, tz + 1);
                int back_side_h = get_height_highestmap(tx, tz - 1);

                for (int ny = 335; ny > h; ny--)
                {
                    set_skylight(tx, ny, tz, 15);
                    if (ny < right_side_h && (!get_block(tx + 1, ny, tz).BlockNID->opaque))
                    {
                        set_skylights(tx + 1, ny, tz);
                        set_skylight(tx + 1, ny, tz, 14);
                    }
                    if (ny < left_side_h && (!get_block(tx - 1, ny, tz).BlockNID->opaque))
                    {
                        set_skylights(tx - 1, ny, tz);
                        set_skylight(tx - 1, ny, tz, 14);
                    }
                    if (ny < front_side_h && (!get_block(tx, ny, tz + 1).BlockNID->opaque))
                    {
                        set_skylights(tx, ny, tz + 1);
                        set_skylight(tx, ny, tz + 1, 14);
                    }
                    if (ny < back_side_h && (!get_block(tx, ny, tz - 1).BlockNID->opaque))
                    {
                        set_skylights(tx, ny, tz - 1);
                        set_skylight(tx, ny, tz - 1, 14);
                    }
                }
            }
        }
        tempchunkxz.ischecksky = true;
    }
}