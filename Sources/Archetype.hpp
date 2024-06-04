#pragma once
#include "TypeId.hpp"

namespace dt 
{

typedef unsigned char* ComponentData;

struct Archetype 
{
    ArchetypeID typeId;
    std::vector<EntityID> entities;
    std::vector<ComponentData> componentData;
    std::vector<size_t> componentDataSize;
};



}