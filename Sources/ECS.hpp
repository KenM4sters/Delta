#pragma once
#include "TypeId.hpp"
#include "Archetype.hpp"
#include <unordered_map>
#include "Component.hpp"

namespace dt 
{

class ISystemBase;

class ECS 
{
private: 
    struct Record 
    {
        Archetype* archetype;
        size_t index;
    };

    typedef std::unordered_map<ComponentTypeID, IComponentBase*> ComponentBaseMap;
    typedef std::unordered_map<EntityID, Record> EntityArchetypeMap;
    typedef std::unordered_map<uint8_t, std::vector<ISystemBase*>> SystemsMap;

public:
    ECS();
    ~ECS();

    const EntityID GetNewID();

    template<class T>
    void RegisterComponent();

    template<class T>
    bool IsComponentRegistered();

    void RegisterSystem(const uint8_t& layer, ISystemBase* system);

    void RegisterEntity(const EntityID& entity);

    void RunSystems(const uint8_t layer, const float ts);

    const Archetype* GetArchetype(const ArchetypeID& id) const;

    template<class T, typename... Args>
    T* AddComponent(const EntityID& entity, Args&&... args);

    template<class T>
    void RemoveComponent(const EntityID& entity);

    template<class T>
    T* GetComponent(const EntityID& entity);

    template<class T>
    bool HasComponent(const EntityID& entity);

    void RemoveEntity(const EntityID& entity);

    template<class... Ts>
    std::vector<EntityID> GetAllEnittiesWith();

private:
    std::vector<Archetype*> mArchetypes{};
    EntityArchetypeMap mEntiyArchetypeMap{};
    SystemsMap mSystemsMap{};
    ComponentBaseMap mComponentBaseMap{};

    EntityID mNumEntities;

};
}
