#ifndef SILVERBACK_ARCHETYPE_HPP
#define SILVERBACK_ARCHETYPE_HPP

#include "TypeId.hpp"

namespace slv 
{

typedef unsigned char* ComponentData;

/**
 * @brief The Archetype class could in many ways be described as the crux of 
 * our ECS framework, and many alternatives to using an archetype/archetype-like structure exist. 
 * An instance of an archetype stores all *registered* components of the same type
 * (Component<T>::GetTypeId() returns a different number but from the same number pool),
 * along with the entities (just UUIDs) that they belong to, as well as the id of the 
 * archetype itself and the size of each component. 
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