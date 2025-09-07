#include "chunk.h"

class ChunkXZ
{
    public:
        Chunk chunkxz[24];
        LightsLevel lightchunks[26];
        int highmap[256];
        bool ishighestcheck[256] = {false};
        bool loading = false;
        bool isload = false;
        bool ischecksky = false;
        bool ischeck = false;
        bool isflood = false;
        pos2 PosXZ;
};
