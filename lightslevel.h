#ifndef LIGHT_LEVEL_H
#define LIGHT_LEVEL_H
#include <vector>
#include <unordered_set>
#include <memory>







class LightsLevel
{
    public:
        std::unordered_set<int> lights[15];
        std::unordered_set<int> skylights;
        std::uint8_t light[4096];
        std::uint8_t skylight[4096];
        void set_light_level(int x, int y, int z, int light_level);
        int get_light_level(int x, int y, int z);
        void set_skylight_level(int x, int y, int z, int light_level);
        int get_skylight_level(int x, int y, int z);
};




inline void LightsLevel::set_light_level(int x, int y, int z, int light_level)
{
    light[y << 8 | z << 4 | x] = light_level;

    
}

inline int LightsLevel::get_light_level(int x, int y, int z)
{
    return light[y << 8 | z << 4 | x];
    
}

inline void LightsLevel::set_skylight_level(int x, int y, int z, int light_level)
{
    skylight[y << 8 | z << 4 | x] = light_level;

    
}

inline int LightsLevel::get_skylight_level(int x, int y, int z)
{
    return skylight[y << 8 | z << 4 | x];
    
}
#endif