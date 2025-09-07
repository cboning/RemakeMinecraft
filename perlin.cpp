#include "perlin.h"
#include <random>

inline double Perlin::fade(double x) {
    return x * x * x * (x * (6 * x - 15) + 10);
}

inline double Perlin::dot(pos2d a, pos2d b)
{
    return a.x * b.x + a.y * b.y;
}
inline double Perlin::dot3(pos3d a, pos3d b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline double Perlin::mix(double x, double y, double z)
{
    return x + (y - x) * z;
}

pos2d Perlin::random2d(pos2 cpos)
{
    pos2d st = pos2d(dot(cpos, pos2d(127.1, 311.7)),
                     dot(cpos, pos2d(269.5, 183.3)));
    st.x = -1.0 + 2.0 * fract(sin(st.x) * 43758.5453123);
    st.y = -1.0 + 2.0 * fract(sin(st.y) * 48742.9032093);
    return st;
}
pos3d Perlin::random3d(pos3 cpos)
{
    pos3d st = pos3d(dot3(cpos, pos3d(127.1, 311.7, 247.9)),
                     dot3(cpos, pos3d(269.5, 183.3, 347.2)),
                     dot3(cpos, pos3d(238.7, 184.87, 257.83)));
    st.x = -1.0 + 2.0 * fract(sin(st.x) * 13948.7438478);
    st.y = -1.0 + 2.0 * fract(sin(st.y) * 95837.8859829);
    st.z = -1.0 + 2.0 * fract(sin(st.z) * 12984.7288427);
    return st;
}

double Perlin::get_perlin_noise(int offset, pos2d opos)
{
    double left_bottom, right_bottom, left_top, right_top;
    pos2d fade_val = {fade(opos.x), fade(opos.y)};

    return mix(
        mix(
            dot(random2dvalue[0][offset], {opos.x, opos.y}),
            dot(random2dvalue[1][offset], {opos.x - 1.0, opos.y}),
            fade_val.x
        ),
        mix(
            dot(random2dvalue[2][offset], {opos.x, opos.y - 1.0}),
            dot(random2dvalue[3][offset], {opos.x - 1.0, opos.y - 1.0}),
            fade_val.x
        ),
        fade_val.y
    );
}
double Perlin::get_perlin_noise_3d(int offset, pos3d opos)
{
    pos3d fade_val = {fade(opos.x), fade(opos.y), fade(opos.z)};
    pos3d grad_indices = {floor(opos.x), floor(opos.y), floor(opos.z)};
    
    pos3d diff1 = opos;
    pos3d diff2 = diff1 - 1.0;

    double x0y0z0 = dot3(random3dvalue[0][offset], diff1);
    double x1y0z0 = dot3(random3dvalue[1][offset], {diff2.x, diff1.y, diff1.z});
    double x0y0z1 = dot3(random3dvalue[2][offset], {diff1.x, diff1.y, diff2.z});
    double x1y0z1 = dot3(random3dvalue[3][offset], {diff2.x, diff1.y, diff2.z});

    double x0y1z0 = dot3(random3dvalue[4][offset], {diff1.x, diff2.y, diff1.z});
    double x1y1z0 = dot3(random3dvalue[5][offset], {diff2.x, diff2.y, diff1.z});
    double x0y1z1 = dot3(random3dvalue[6][offset], {diff1.x, diff2.y, diff2.z});
    double x1y1z1 = dot3(random3dvalue[7][offset], diff2);

    double mix_x0 = mix(x0y0z0, x1y0z0, fade_val.x);
    double mix_x1 = mix(x0y0z1, x1y0z1, fade_val.x);
    double mix_x2 = mix(x0y1z0, x1y1z0, fade_val.x);
    double mix_x3 = mix(x0y1z1, x1y1z1, fade_val.x);

    double mix_y0 = mix(mix_x0, mix_x1, fade_val.z);
    double mix_y1 = mix(mix_x2, mix_x3, fade_val.z);

    return mix(mix_y0, mix_y1, fade_val.y);
}

void Perlin::set_seed(int seed_val)
{
    seed = seed_val;
}



void Perlin::chunk2dinit(pos2 cpos, int offset)
{
    double n = offset;
    cpos = {static_cast<int>(floor(cpos.x / n)), static_cast<int>(floor(cpos.y / n))};

    random2dvalue[0][offset] = random2d({cpos.x, cpos.y});
    random2dvalue[1][offset] = random2d({cpos.x + 1, cpos.y});
    random2dvalue[2][offset] = random2d({cpos.x, cpos.y + 1});
    random2dvalue[3][offset] = random2d({cpos.x + 1, cpos.y + 1});

}

void Perlin::chunk3dinit(pos3 cpos, int offset)
{
double n = offset;
    cpos = {static_cast<int>(floor(cpos.x / n)), static_cast<int>(floor(cpos.y / n)), static_cast<int>(floor(cpos.z / n))};

    random3dvalue[0][offset] = random3d({cpos.x, cpos.y, cpos.z});
    random3dvalue[1][offset] = random3d({cpos.x + 1, cpos.y, cpos.z});
    random3dvalue[2][offset] = random3d({cpos.x, cpos.y, cpos.z + 1});
    random3dvalue[3][offset] = random3d({cpos.x + 1, cpos.y, cpos.z + 1});
    random3dvalue[4][offset] = random3d({cpos.x, cpos.y + 1, cpos.z});
    random3dvalue[5][offset] = random3d({cpos.x + 1, cpos.y + 1, cpos.z});
    random3dvalue[6][offset] = random3d({cpos.x, cpos.y + 1, cpos.z + 1});
    random3dvalue[7][offset] = random3d({cpos.x + 1, cpos.y + 1, cpos.z + 1});
}