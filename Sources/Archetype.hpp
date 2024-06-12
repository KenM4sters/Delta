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
 * along with the entities (just UUIDs) that they belong to. 
 * The archetype also needs to hold the size of each component in componentData since
 * we're managing memory ourselves, as well as the id of the archetype itself,
 * which is just a vector of all the IDs that belong to the components in 
 * componentData.
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