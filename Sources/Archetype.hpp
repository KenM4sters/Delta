#ifndef SILVERBACK_ARCHETYPE_HPP
#define SILVERBACK_ARCHETYPE_HPP

#include "TypeId.hpp"

namespace slv 
{

typedef unsigned char* ComponentData;

/**
 * @brief Archetype definition.
*/
struct Archetype 
{
    ArchetypeID typeId;
    std::vector<EntityID> entities;
    std::vector<ComponentData> componentData;
    std::vector<size_t> componentDataSize;
};
}

#endif