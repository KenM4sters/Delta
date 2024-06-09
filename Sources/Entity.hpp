#ifndef SILVERBACK_ENTITY_HPP
#define SILVERBACK_ENTITY_HPP

#include "TypeId.hpp"
#include "Registry.hpp"

#include <iostream>
#include <vector>

namespace slv 
{

/**
 * @brief Entity Class.
*/
class Entity 
{
public:
    explicit Entity(Registry* registry)
        : mID{registry->GetNewID()}, mRegistry{registry}
    {
        mRegistry->RegisterEntity(mID);
    }

    template<class T, typename... Args>
    T* AddComponent(Args&&... args) 
    {
        return mRegistry->AddComponent(mID, std::forward<Args>(args)...);
    }

    template<class T>
    T* AddComponent(T&& t) 
    {
        return mRegistry->AddComponent(mID, std::forward<T>(t));
    }

private:
    EntityID mID;
    Registry* mRegistry = nullptr;
};
}

#endif