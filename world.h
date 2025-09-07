#ifndef WORLD_H
#define WORLD_H

#include "chunkxz.h"
#include "perlin.h"
#include "FastNoiseLite.h"
#include <unordered_map>
#include <unordered_set>
#include <thread>
#include <mutex>

struct ChunkPos3Hash
{
      std::size_t operator()(const pos3 &cp) const
      {
            // FNV-1a 哈希算法
            const std::size_t fnv_prime = 1099511628211u;
            std::size_t hash = 14695981039346656037u;

            hash ^= static_cast<std::size_t>(cp.x);
            hash *= fnv_prime;
            hash ^= static_cast<std::size_t>(cp.y);
            hash *= fnv_prime;
            hash ^= static_cast<std::size_t>(cp.z);
            hash *= fnv_prime;

            return hash;
      }
};
struct ChunkPos2Hash
{
      std::size_t operator()(const pos2 &cp) const
      {
            // FNV-1a 哈希算法
            const std::size_t fnv_prime = 1099511628211u;
            std::size_t hash = 14695981039346656037u;

            hash ^= static_cast<std::size_t>(cp.x);
            hash *= fnv_prime;
            hash ^= static_cast<std::size_t>(cp.y);
            hash *= fnv_prime;

            return hash;
      }
};

struct checkblockpos
{
      inline void update(std::uint8_t x, std::uint8_t y, std::uint8_t z, int cx, int cy, int cz)
      {
            up = {{cx, cy + ((y + 1) >> 4), cz},
                  {x, (y + 1) & 15, z}};

            down = {{cx, cy - 1 + ((y + 15) >> 4), cz},
                    {x, (y + 15) & 15, z}};

            left = {{cx - 1 + ((x + 15) >> 4), cy, cz},
                    {(x + 15) & 15, y, z}};

            right = {{cx + ((x + 1) >> 4), cy, cz},
                     {(x + 1) & 15, y, z}};

            front = {{cx, cy, cz + ((z + 1) >> 4)},
                     {x, y, (z + 1) & 15}};

            back = {{cx, cy, cz - 1 + ((z + 15) >> 4)},
                    {x, y, (z + 15) & 15}};
      }

      copos up;
      copos down;
      copos left;
      copos right;
      copos front;
      copos back;
};

struct floodblockpos
{
      inline void update(int x, int y, int z)
      {
            up = {x, y + 1, z};

            down = {x, y - 1, z};

            left = {x - 1, y, z};

            right = {x + 1, y, z};

            front = {x, y, z + 1};

            back = {x, y, z - 1};
      }

      pos3 up;
      pos3 down;
      pos3 left;
      pos3 right;
      pos3 front;
      pos3 back;
};

struct aoblockpos
{
      /*u:up
        d:down
        l:left
        r:right
        f:front
        b:back
        */
      inline void update(std::uint8_t x, std::uint8_t y, std::uint8_t z, int cx, int cy, int cz)
      {

            u = {{cx, cy + ((y + 1) >> 4), cz},
                 {x, (y + 1) & 15, z}};

            d = {{cx, cy - 1 + ((y + 15) >> 4), cz},
                 {x, (y + 15) & 15, z}};

            l = {{cx - 1 + ((x + 15) >> 4), cy, cz},
                 {(x + 15) & 15, y, z}};

            r = {{cx + ((x + 1) >> 4), cy, cz},
                 {(x + 1) & 15, y, z}};

            f = {{cx, cy, cz + ((z + 1) >> 4)},
                 {x, y, (z + 1) & 15}};

            b = {{cx, cy, cz - 1 + ((z + 15) >> 4)},
                 {x, y, (z + 15) & 15}};

            uf = {{cx, u.ChunkPos.y, f.ChunkPos.z},
                  {x, u.OPos.y, f.OPos.z}};

            ub = {{cx, u.ChunkPos.y, b.ChunkPos.z},
                  {x, u.OPos.y, b.OPos.z}};

            ul = {{l.ChunkPos.x, u.ChunkPos.y, cz},
                  {l.OPos.x, u.OPos.y, z}};

            ur = {{r.ChunkPos.x, u.ChunkPos.y, cz},
                  {r.OPos.x, u.OPos.y, z}};

            df = {{cx, d.ChunkPos.y, f.ChunkPos.z},
                  {x, d.OPos.y, f.OPos.z}};

            db = {{cx, d.ChunkPos.y, b.ChunkPos.z},
                  {x, d.OPos.y, b.OPos.z}};

            dl = {{l.ChunkPos.x, d.ChunkPos.y, cz},
                  {l.OPos.x, d.OPos.y, z}};

            dr = {{r.ChunkPos.x, d.ChunkPos.y, cz},
                  {r.OPos.x, d.OPos.y, z}};

            fl = {{l.ChunkPos.x, cy, f.ChunkPos.z},
                  {l.OPos.x, y, f.OPos.z}};

            fr = {{r.ChunkPos.x, cy, f.ChunkPos.z},
                  {r.OPos.x, y, f.OPos.z}};

            bl = {{l.ChunkPos.x, cy, b.ChunkPos.z},
                  {l.OPos.x, y, b.OPos.z}};

            br = {{r.ChunkPos.x, cy, b.ChunkPos.z},
                  {r.OPos.x, y, b.OPos.z}};

            ufl = {{l.ChunkPos.x, u.ChunkPos.y, f.ChunkPos.z},
                   {l.OPos.x, u.OPos.y, f.OPos.z}};

            ufr = {{r.ChunkPos.x, u.ChunkPos.y, f.ChunkPos.z},
                   {r.OPos.x, u.OPos.y, f.OPos.z}};

            ubl = {{l.ChunkPos.x, u.ChunkPos.y, b.ChunkPos.z},
                   {l.OPos.x, u.OPos.y, b.OPos.z}};

            ubr = {{r.ChunkPos.x, u.ChunkPos.y, b.ChunkPos.z},
                   {r.OPos.x, u.OPos.y, b.OPos.z}};

            dfl = {{l.ChunkPos.x, d.ChunkPos.y, f.ChunkPos.z},
                   {l.OPos.x, d.OPos.y, f.OPos.z}};

            dfr = {{r.ChunkPos.x, d.ChunkPos.y, f.ChunkPos.z},
                   {r.OPos.x, d.OPos.y, f.OPos.z}};

            dbl = {{l.ChunkPos.x, d.ChunkPos.y, b.ChunkPos.z},
                   {l.OPos.x, d.OPos.y, b.OPos.z}};

            dbr = {{r.ChunkPos.x, d.ChunkPos.y, b.ChunkPos.z},
                   {r.OPos.x, d.OPos.y, b.OPos.z}};
      }

      copos u;
      copos d;
      copos l;
      copos r;
      copos f;
      copos b;

      copos uf;
      copos ub;
      copos ul;
      copos ur;
      copos df;
      copos db;
      copos dl;
      copos dr;
      copos fl;
      copos fr;
      copos bl;
      copos br;

      copos ufl;
      copos ufr;
      copos ubl;
      copos ubr;
      copos dfl;
      copos dfr;
      copos dbl;
      copos dbr;
};

class World
{
public:
      std::mutex muex;
      std::mutex runningMutex;
      std::unordered_map<pos2, ChunkXZ, ChunkPos2Hash> world;
      int linetmap[529];                      // 用于给从中心区块到外围区块排序
      std::vector<pos3> checklist;
      std::unordered_set<pos3, ChunkPos3Hash> check_list_for_main_thread;
      bool gamerunning = true;
      int chunkr = 12;
      int chunkl = chunkr * 2 + 1;
      int chunka = chunkl * chunkl;
      int chunkdr = chunkr - 1;
      int chunkdl = chunkdr * 2 + 1;
      int chunkda = chunkdl * chunkdl;
      // std::int8_t lightcheckdir[18] = {
      //     1, 1, 2,
      //     1, 1, 0,
      //     1, 2, 1,
      //     1, 0, 1,
      //     2, 1, 1,
      //     0, 1, 1};

      std::int8_t lightcheckdir[18] = {
          0, 0, 1,
          0, 0, -1,
          0, 1, 0,
          0, -1, 0,
          1, 0, 0,
          -1, 0, 0};

      Perlin terrain;

      int fps = 0;
      int fps2 = 0;

      checkblockpos blockface;
      floodblockpos lightface;
      aoblockpos light;

      pos2 load_offset = {-chunkr, -chunkr}, toffset = {-chunkr, -chunkr}, light_flood_offset = {-chunkr, -chunkr};
      Block Air;

      Block get_block(int x, int y, int z);
      Block get_block_from_blockface_up();
      Block get_block_from_blockface_down();
      Block get_block_from_blockface_left();
      Block get_block_from_blockface_right();
      Block get_block_from_blockface_front();
      Block get_block_from_blockface_back();
      int get_light_from_lightface_up();
      int get_light_from_lightface_down();
      int get_light_from_lightface_left();
      int get_light_from_lightface_right();
      int get_light_from_lightface_front();
      int get_light_from_lightface_back();

      Block get_block_from_blockface_data(copos pos);
      int get_light_level_from_light(copos pos);

      pos4 get_ao_from_blockface_up();
      pos4 get_ao_from_blockface_down();
      pos4 get_ao_from_blockface_left();
      pos4 get_ao_from_blockface_right();
      pos4 get_ao_from_blockface_front();
      pos4 get_ao_from_blockface_back();

      int vertexAO(bool side1, bool side2, bool corner)
      {
            if (side1 && side2)
            {
                  return 0;
            }
            return 6 - 2 * (side1 + side2 + corner);
      }
      Chunk &get_chunk(int x, int y, int z);
      ChunkXZ &get_chunkxz(int x, int z);
      LightsLevel &get_light_chunk(pos3 cpos);
      void loadthread();
      bool updata(int x, int z);
      void cltm(); // 给从中心区块到外围区块排序
      bool loadchunk(int x, int z);
      void loadnewchunk(int x, int z);
      void loading();
      void light_flooding();
      bool lightflood(int x, int z);
      bool flood(int x, int z);
      void set_light(int x, int y, int z, int light);
      void set_skylight(int x, int y, int z, int light);
      int get_light(int x, int y, int z);
      int get_skylight(int x, int y, int z);
      std::unordered_set<pos3, ChunkPos3Hash> delete_lights(std::unordered_map<int, std::vector<pos3>> dlightPos);
      void delete_block(std::vector<pos3> dblockPos);
      void check_skylights(int x, int z);
      void set_skylights(int x, int y, int z);
      int get_height_highestmap(int x, int z);
      void endingThread();
      void endSignal();
};

#endif