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
    /**
     * @brief A vector of the component Ids that the
     * archetype is interested in. 
     */
    ArchetypeID typeId;
    /**
     * @brief A vector of all the entities that share this archetype (ie have a superset
     * of the components in typeId).
     */
    std::vector<EntityID> entities;
    /**
     * @brief A vector of pointers to the data that belongs to each component found
     * in typeId.
     */
    std::vector<ComponentData> componentData;
    /**
     * @brief Since we're heap allocating the data for each component ourselves and only holding
     * pointers to them, we need to hold a respective vector of sizes that correspond to the 
     * size of the data for each component, so that we can properly navigate through the 
     * componentData with pointer arithmetic.
     */
    std::vector<size_t> componentDataSize;
};
}

#endif