#ifndef PERLIN_H
#define PERLIN_H

#include "pos.h"
#include <cmath>
#include <random>
#include <unordered_map>


class Perlin
{
    public:
        int seed;
        int chunksize = 16;
        std::unordered_map<int, pos2d> random2dvalue[4];
        std::unordered_map<int, pos3d> random3dvalue[8];
        inline double fract(double num) {
            return num - std::floor(num);
        }
        void set_seed(int seed_val);
        double get_perlin_noise(int offset, pos2d opos);
        double get_perlin_noise_3d(int offset, pos3d opos);
        void chunk2dinit(pos2 cpos, int offset);
        void chunk3dinit(pos3 cpos, int offset);
        double fade(double x);
        double dot(pos2d a, pos2d b);
        double dot3(pos3d a, pos3d b);
        double mix(double x, double y, double z);
        pos2d random2d(pos2 cpos);
        pos3d random3d(pos3 cpos);
};



#endif