#ifndef ECS_ENTITY_HPP
#define ECS_ENTITY_HPP
#include <iostream>
#include <vector>
#include "TypeId.hpp"
#include "ECS.hpp"

namespace dt 
{

class Entity 
{
public:
    explicit Entity(ECS* ecs)
        : mID(ecs->GetNewID()), mECS(ecs)
    {
        mECS->RegisterEntity(mID);
    }

    template<class T, typename... Args>
    T* AddComponent(Args&&... args) 
    {
        return mECS->AddComponent(mID, std::forward<Args>(args)...);
    }

    template<class T>
    T* AddComponent(T&& t) 
    {
        return mECS->AddComponent(mID, std::forward<T>(t));
    }

private:
    EntityID mID;
    ECS* mECS = nullptr;
};
}

#endif