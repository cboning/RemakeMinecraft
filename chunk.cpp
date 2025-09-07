#include "chunk.h"
#include "world.h"
#include <iostream>
#include <GLFW/glfw3.h>

extern block BlockID;
void Chunk::set_pblock(int x, int y, int z, blockdata *blockid)
{
    ablock[y << 8 | z << 4 | x].BlockNID = blockid;
    _light->light[y << 8 | z << 4 | x] = (*blockid).light;
    if ((*blockid).light > 0)
    {
        _light->lights[(*blockid).light - 1].insert(y << 8 | z << 4 | x);
    }
}

blockdata *Chunk::get_block_data(int x, int y, int z)
{
    return ablock[y << 8 | z << 4 | x].BlockNID;
}

Block Chunk::get_block(int x, int y, int z)
{
    return ablock[y << 8 | z << 4 | x];
}

void Chunk::check()
{
    pos3 ocpos = {ChunkPos.x << 4, ChunkPos.y << 4, ChunkPos.z << 4};
    bool dirface[6];
    int j, t;

    for (std::uint16_t i = 0; i < 4096; i++)
    {
        if (*ablock[i].BlockNID == BlockID.Air)
        {
            continue;
        }
        Block &sablock = ablock[i];
        std::uint8_t x = i & 15, y = i >> 8, z = i >> 4 & 15;
        pos3 sablock_wpos = {ocpos.x + x,
                             ocpos.y + y,
                             ocpos.z + z};
        if (ablock[i].BlockNID->x)
        {
            checkface(sablock_wpos, _world->get_light_level_from_light({ChunkPos, {x, y, z}}), _world->get_light_level_from_light({ChunkPos, {x, y, z}}), BlockTexCoord(sablock.BlockNID, 6), xFace1);
            checkface(sablock_wpos, _world->get_light_level_from_light({ChunkPos, {x, y, z}}), _world->get_light_level_from_light({ChunkPos, {x, y, z}}), BlockTexCoord(sablock.BlockNID, 7), xFace2);
            checkface(sablock_wpos, _world->get_light_level_from_light({ChunkPos, {x, y, z}}), _world->get_light_level_from_light({ChunkPos, {x, y, z}}), BlockTexCoord(sablock.BlockNID, 6), xFace3);
            checkface(sablock_wpos, _world->get_light_level_from_light({ChunkPos, {x, y, z}}), _world->get_light_level_from_light({ChunkPos, {x, y, z}}), BlockTexCoord(sablock.BlockNID, 7), xFace4);
        }
        else
        {
            _world->blockface.update(x, y, z, ChunkPos.x, ChunkPos.y, ChunkPos.z);
            t = 6;
            t -= dirface[0] = _world->get_block_from_blockface_front().BlockNID->opaque;
            t -= dirface[1] = _world->get_block_from_blockface_back().BlockNID->opaque;
            t -= dirface[2] = _world->get_block_from_blockface_left().BlockNID->opaque;
            t -= dirface[3] = _world->get_block_from_blockface_right().BlockNID->opaque;
            t -= dirface[4] = _world->get_block_from_blockface_down().BlockNID->opaque;
            t -= dirface[5] = _world->get_block_from_blockface_up().BlockNID->opaque;

            if (t > 0)
            {
                _world->lightface.update(sablock_wpos.x, sablock_wpos.y, sablock_wpos.z);
                _world->light.update(x, y, z, ChunkPos.x, ChunkPos.y, ChunkPos.z);

                if (!dirface[0])
                    checkface(sablock_wpos, _world->get_ao_from_blockface_front(), _world->get_light_from_lightface_front(), BlockTexCoord(sablock.BlockNID, 0), frontFace);
                if (!dirface[1])
                    checkface(sablock_wpos, _world->get_ao_from_blockface_back(), _world->get_light_from_lightface_back(), BlockTexCoord(sablock.BlockNID, 1), backFace);

                if (!dirface[2])
                    checkface(sablock_wpos, _world->get_ao_from_blockface_left(), _world->get_light_from_lightface_left(), BlockTexCoord(sablock.BlockNID, 2), leftFace);

                if (!dirface[3])
                    checkface(sablock_wpos, _world->get_ao_from_blockface_right(), _world->get_light_from_lightface_right(), BlockTexCoord(sablock.BlockNID, 3), rightFace);

                if (!dirface[4])
                    checkface(sablock_wpos, _world->get_ao_from_blockface_down(), _world->get_light_from_lightface_down(), BlockTexCoord(sablock.BlockNID, 4), bottomFace);

                if (!dirface[5])
                    checkface(sablock_wpos, _world->get_ao_from_blockface_up(), _world->get_light_from_lightface_up(), BlockTexCoord(sablock.BlockNID, 5), topFace);
            }
        }
    }
    ischeck = true;
}

void Chunk::checkface(pos3 BlockWPos, pos4 light, int facelight, const std::array<float, 8> &textureCoords, const std::array<float, 12> &blockFace)
{

    display_data.textureCoords.insert(display_data.textureCoords.end(), textureCoords.begin(), textureCoords.end());

    pos4f tempos;
    for (int i = 0, index = 0; i < 4; ++i)
    {
        display_data.verticies.push_back(blockFace[index++] + BlockWPos.x);
        display_data.verticies.push_back(blockFace[index++] + BlockWPos.y);
        display_data.verticies.push_back(blockFace[index++] + BlockWPos.z);
        tempos = otf(light[i]);
        display_data.light.insert(display_data.light.end(), {tempos.x, tempos.y, tempos.z, tempos.w});
    }

    // if (true)
    // {
    // }
    // else
    // {
    //     for (int i = 0, index = 0; i < 4; ++i)
    //     {
    //         display_data.light.push_back(facelight / 15.0);
    //         display_data.light.push_back(facelight / 15.0);
    //         display_data.light.push_back(facelight / 15.0);
    //         display_data.light.push_back(1.0);
    //     }
    // }

    display_data.indices.insert(display_data.indices.end(),
                                {indexIndex, indexIndex + 1, indexIndex + 2,

                                 indexIndex + 2, indexIndex + 3, indexIndex});
    indexIndex += 4;
}

void Chunk::veboinit()
{
    if (ischeck)
    {
        if (!isdataed)
        {
            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);
            glGenBuffers(1, &vbo1);
            glBindBuffer(GL_ARRAY_BUFFER, vbo1);
            glBufferData(GL_ARRAY_BUFFER, display_data.verticies.size() * sizeof(float), display_data.verticies.data(), GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

            glEnableVertexAttribArray(0);

            glGenBuffers(1, &vbo2);
            glBindBuffer(GL_ARRAY_BUFFER, vbo2);
            glBufferData(GL_ARRAY_BUFFER, display_data.light.size() * sizeof(float), display_data.light.data(), GL_STATIC_DRAW);

            glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void *)0);

            glEnableVertexAttribArray(1);

            glGenBuffers(2, &vbo3);
            glBindBuffer(GL_ARRAY_BUFFER, vbo3);
            glBufferData(GL_ARRAY_BUFFER, display_data.textureCoords.size() * sizeof(float), display_data.textureCoords.data(), GL_STATIC_DRAW);

            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);

            glEnableVertexAttribArray(2);

            indicesCount = static_cast<int>(display_data.indices.size());
            glGenBuffers(1, &ebo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, display_data.indices.size() * sizeof(int),
                         display_data.indices.data(), GL_STATIC_DRAW);
            isdataed = true;
            display_data.verticies.clear();
            display_data.textureCoords.clear();
            display_data.light.clear();
            display_data.indices.clear();
            display_data.verticies.shrink_to_fit();
            display_data.textureCoords.shrink_to_fit();
            display_data.light.shrink_to_fit();
            display_data.indices.shrink_to_fit();
        }
        else
        {
            // std::cout << vao << std::endl;
            glBindVertexArray(vao);
            glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, nullptr);
        }
    }
}

const std::array<float, 8> Chunk::BlockTexCoord(blockdata *Blocktexdata, int face)
{

    float minx = face / 8.0f;
    float miny = (texture_long - Blocktexdata->texcoord - 1) / texture_long;
    float maxx = (face + 1) / 8.0f;
    float maxy = (texture_long - Blocktexdata->texcoord) / texture_long;

    return {minx, miny, maxx, miny, maxx, maxy, minx, maxy};
}

pos4f Chunk::otf(int light)
{
    float tlight = light / 15.0;

    return {tlight, tlight, tlight, 1.0};
}

void Chunk::update_chunk()
{

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo1);
    glBufferData(GL_ARRAY_BUFFER, display_data.verticies.size() * sizeof(float), display_data.verticies.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vbo2);
    glBufferData(GL_ARRAY_BUFFER, display_data.light.size() * sizeof(float), display_data.light.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vbo3);
    glBufferData(GL_ARRAY_BUFFER, display_data.textureCoords.size() * sizeof(float), display_data.textureCoords.data(), GL_STATIC_DRAW);

    indicesCount = static_cast<int>(display_data.indices.size());

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, display_data.indices.size() * sizeof(int),
                 display_data.indices.data(), GL_STATIC_DRAW);
    display_data.verticies.clear();
    display_data.textureCoords.clear();
    display_data.light.clear();
    display_data.indices.clear();
    display_data.verticies.shrink_to_fit();
    display_data.textureCoords.shrink_to_fit();
    display_data.light.shrink_to_fit();
    display_data.indices.shrink_to_fit();
}
void Chunk::delete_chunk()
{
    if (isdataed)
    {
        ischeck = false;
        isdataed = false;

        indexIndex = 0;

        display_data.verticies.clear();
        display_data.textureCoords.clear();
        display_data.light.clear();
        display_data.indices.clear();
        display_data.verticies.shrink_to_fit();
        display_data.textureCoords.shrink_to_fit();
        display_data.light.shrink_to_fit();
        display_data.indices.shrink_to_fit();

        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo1);
        glDeleteBuffers(1, &vbo2);
        glDeleteBuffers(1, &vbo3);
        glDeleteBuffers(1, &ebo);
    }
}