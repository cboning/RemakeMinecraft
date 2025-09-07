#ifndef CHUNK_H
#define CHUNK_H

#include "block.h"
#include "lightslevel.h"
#include <bitset>
#include <glad/glad.h>

struct vbeodata
{
    std::vector<float> verticies;
    std::vector<float> textureCoords;
    std::vector<float> light;
    std::vector<int> indices;
};

class World;

class Chunk
{
  public:
    float texture_long = 26;
    Block ablock[4096];
    pos3 ChunkPos;
    vbeodata display_data;
    World *_world;
    LightsLevel *_light;
    bool isdataed = false;
    bool ischeck = false;
    int indexIndex = 0;

    const std::array<float, 12> frontFace{
        0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1,
    };

    const std::array<float, 12> backFace{
        1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0,
    };

    const std::array<float, 12> leftFace{
        0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0,
    };

    const std::array<float, 12> rightFace{
        1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1,
    };

    const std::array<float, 12> topFace{
        0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0,
    };

    const std::array<float, 12> bottomFace{0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1};

    const std::array<float, 12> xFace1{0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0};

    const std::array<float, 12> xFace2{0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1};

    const std::array<float, 12> xFace3{0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1};

    const std::array<float, 12> xFace4{0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0};

    unsigned int vao, vbo1, vbo2, vbo3, ebo;
    int indicesCount = 0;

    void set_pblock(int x, int y, int z, blockdata *blockid);
    blockdata *get_block_data(int x, int y, int z);
    Block get_block(int x, int y, int z);
    void check();
    void checkface(pos3 BlockOPos, pos4 light, int facelight, const std::array<float, 8> &textureCoords, const std::array<float, 12> &blockFace);
    void update_chunk();
    void veboinit();
    const std::array<float, 8> BlockTexCoord(blockdata *Blocktexdata, int face);
    pos4f otf(int light);
    void delete_chunk();
};

#endif