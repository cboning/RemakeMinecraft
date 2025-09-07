#ifndef BLOCK_H
#define BLOCK_H
#include <bitset>
#include <memory>
#include <string>
#include <iostream>
#include "pos.h"



struct blockdata
{
    int id;
    int childId;
    std::string name;
    std::uint8_t light;
    int texcoord = -1;
    bool opaque;
    bool x;
    
};



inline bool operator==(const blockdata &lhs, const blockdata &rhs)
{
    return lhs.id == rhs.id && lhs.childId == rhs.childId;
}

inline bool operator!=(const blockdata &lhs, const blockdata &rhs)
{
    return lhs.id != rhs.id || lhs.childId != rhs.childId;
}

struct block
{
    blockdata Air = {0,0,"air",0,-1,false,false};
    blockdata Stone = {1,0,"stone",0,0,true,false};
    blockdata Granite = {1,1,"granite",0,1,true,false};
    blockdata Polished_Granite = {1,2,"polished_granite",0,2,true,false};
    blockdata Diorite = {1,3,"diorite",0,3,true,false};
    blockdata Polished_Diorite = {1,4,"polished_diorite",0,4,true,false};
    blockdata Andesite = {1,5,"andesite",0,5,true,false};
    blockdata Polished_Andesite = {1,6," polished_andesite",0,6,true,false};
    blockdata Grass_Block = {2,0,"grass_block",0,7,true,false};
    blockdata Dirt = {3,0,"dirt",0,8,true,false};
    blockdata Coarse_Dirt = {3,0,"coarse_dirt",0,9,true,false};
    blockdata Podzol = {3,0,"podzol",0,10,true,false};
    blockdata Cobblestone = {4,0,"cobblestone",0,11,true,false};
    blockdata Oak_Planks = {5,0,"oak_planks",0,12,true,false};
    blockdata Spruce_Planks = {5,1,"spruce_planks",0,13,true,false};
    blockdata Birch_Planks = {5,2,"birch_planks",0,14,true,false};
    blockdata Jungle_Planks = {5,3,"jungle_planks",0,15,true,false};
    blockdata Acacia_Planks = {5,4,"acacia_planks",0,16,true,false};
    blockdata Dark_Oak_Planks = {5,5,"dark_oak_planks",0,17,true,false};
    blockdata Oak_Sapling = {6,0,"oak_sapling",0,18,false,true};
    blockdata Spruce_Sapling = {6,1,"spruce_sapling",0,19,false,true};
    blockdata Birch_Sapling = {6,2,"birch_sapling",0,20,false,true};
    blockdata Jungle_Sapling = {6,3,"jungle_sapling",0,21,false,true};
    blockdata Acacia_Sapling = {6,4,"acacia_sapling",0,22,false,true};
    blockdata Dark_Oak_Sapling = {6,5,"dark_oak_sapling",0,23,false,true};
    blockdata Bedrock = {6,5,"bedrock",0,24,true,false};
    blockdata GlowStone = {8,0,"glowstone",15,25,true,false};

};
extern struct block BlockID; 

struct copos
{
    pos3 ChunkPos;
    pos3 OPos;
};

class Block
{
    public:
        Block() : BlockNID(&BlockID.Air) {}
        blockdata *BlockNID;


};


#endif