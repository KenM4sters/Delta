#ifndef SILVERBACK_REGISTRY_HPP
#define SILVERBACK_REGISTRY_HPP

#include "Archetype.hpp"
#include "Component.hpp"

#include <unordered_map>
#include <functional>

namespace slv 
{

class Entity;
class ISystemBase;

template<class... Cs>
class System;

/**
 * @brief 
*/
class Registry 
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
    inline Registry();

    inline ~Registry();

    template<class T>
    void RegisterComponent();

    template<class T>
    bool IsComponentRegistered();

    template<class T, typename... Args>
    T* AddComponent(const EntityID& entity, Args&&... args);

    template<class T>
    void RemoveComponent(const EntityID& entity);

    template<class T>
    T* GetComponent(const EntityID& entity);

    template<class T>
    bool HasComponent(const EntityID& entity);

    template<class... Ts>
    std::vector<EntityID> GetAllEnittiesWith();

    inline void RegisterSystem(const uint8_t& layer, ISystemBase* system);

    inline void RegisterEntity(const EntityID& entity);

    inline void RemoveEntity(const EntityID& entity);

    inline void RunSystems(const uint8_t layer, const float elapsedTime);

    inline Archetype* GetArchetype(const ArchetypeID& id);

    inline const EntityID GetNewID();

    template<class T, class...Ts>
    inline void ProcessComponents(std::vector<ComponentTypeID>* components);

private:
    std::vector<Archetype*> mArchetypes{};

    EntityArchetypeMap mEntiyArchetypeMap{};
    
    SystemsMap mSystemsMap{};
    
    ComponentBaseMap mComponentBaseMap{};

    EntityID mNumEntities;
};


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
        return mRegistry->AddComponent<T>(mID, std::forward<Args>(args)...);
    }

    template<class T>
    T* AddComponent(T&& t) 
    {
        return mRegistry->AddComponent<T>(mID, std::forward<T>(t));
    }

    inline const EntityID& GetID() const { return mID; }

private:
    EntityID mID;
    Registry* mRegistry = nullptr;
};


/**
 * @brief Base System interface.
*/
class ISystemBase 
{
public:
    virtual ~ISystemBase() {}

    virtual ArchetypeID GetArchetypeTarget() const = 0;
    
    virtual void DoAction(float elapsedTime, Archetype* archetype) = 0;

};


/**
 * @brief Derived System class.
*/
template<class... Cs>
class System : public ISystemBase 
{
public:
	typedef std::function<void(const float, const std::vector<EntityID>&, Cs*...)> ActionDef;
	
	System(Registry& registry, const std::uint8_t& layer);

    ~System();

	virtual ArchetypeID GetArchetypeTarget() const override;
	
	void Action(ActionDef action);

    template<std::size_t Index1, typename T, typename... Ts>
	std::enable_if_t<Index1==sizeof...(Cs)> DoAction(const float elapsedMilliseconds,
		const ArchetypeID& archetypes,
		const std::vector<EntityID>& entities,
		T& t,
		Ts... ts
    );

	template<std::size_t Index1, typename T, typename... Ts>
	std::enable_if_t<Index1 != sizeof...(Cs)> DoAction(const float elapsedMilliseconds,
		const ArchetypeID& archetypes,
		const std::vector<EntityID>& entities,
		T& t,
		Ts... ts
    );

    virtual void DoAction(const float elapsedMilliseconds, Archetype* archetype) override;

private:
	Registry& mRegistry;

	ActionDef mAction;
	
	bool mActionSet = false;

};


/**
 * 
 * 
 *  Implementation for Registry class.
 * 
 * 
*/


inline Registry::Registry()
    : mNumEntities{1}
{

}

inline Registry::~Registry() 
{
    for(Archetype* archetype : mArchetypes)
    {
        for(size_t i = 0; i < archetype->typeId.size(); ++i)
        {
            IComponentBase* const comp = mComponentBaseMap[archetype->typeId[i]];
            const size_t& dataSize = comp->GetSize();
            for(size_t e = 0; e < archetype->entities.size(); ++e)
            {
                comp->DestroyData(&archetype->componentData[i][e*dataSize]);
            }
            delete[] archetype->componentData[i];
        }

        delete archetype;
    }

    for(auto& p : mComponentBaseMap) 
    {
        delete p.second;
    }
}


inline const EntityID Registry::GetNewID() 
{
    return mNumEntities++;
}


template<class T>
void Registry::RegisterComponent() 
{
    ComponentTypeID componentTypeID = Component<T>::GetTypeID();

    if(mComponentBaseMap.count(componentTypeID)) 
    {
        return;
    }

    mComponentBaseMap.emplace(componentTypeID, new Component<T>);
}


template<class T>
bool Registry::IsComponentRegistered() 
{
    if(mComponentBaseMap.count(Component<T>::GetTypeID())) 
    {
        return true;
    }
    return false;
}


inline void Registry::RegisterSystem(const uint8_t& layer, ISystemBase* system) 
{
    mSystemsMap[layer].push_back(system);
}


inline void Registry::RegisterEntity(const EntityID& entity) 
{
    Record pch{};
    pch.archetype = nullptr;
    pch.index = 0;
    mEntiyArchetypeMap[entity] = pch;
}


inline void Registry::RunSystems(const uint8_t layer, const float elapsedTime) 
{
    for(auto system : mSystemsMap[layer]) 
    {
        const ArchetypeID& target = system->GetArchetypeTarget();

        for(auto archetype : mArchetypes) 
        {
            if(std::includes(archetype->typeId.begin(), archetype->typeId.end(),
                target.begin(), target.end()))
            {
                // this archetype has all the types required by the system
                // so we can pull it's relevant data, reinterpret them as
                // their correct types, and call the Action in the system.
                system->DoAction(elapsedTime, archetype);
            }
        }
    }
}


inline Archetype* Registry::GetArchetype(const ArchetypeID& id) 
{
    // Archetype already exists...
    //
    for(const auto& archetype : mArchetypes) 
    {
        if(archetype->typeId == id) 
        {
            return archetype;
        }
    } 

    // Archetype doesn't exist, so we'll create one and return it.
    //
    Archetype* newArchetype = new Archetype;
    newArchetype->typeId = id;
    mArchetypes.push_back(newArchetype);

    for(size_t i = 0; i < id.size(); i++) 
    {
        newArchetype->componentData.push_back(new unsigned char[0]);
        newArchetype->componentDataSize.push_back(0);
    }

    return newArchetype;
}


template<class T, typename... Args>
T* Registry::AddComponent(const EntityID& entity, Args&&... args) 
{
    if(!IsComponentRegistered<T>()) 
    {
        throw std::runtime_error("Attempting to AddComponent with an unregistered component!");
    }

    ComponentTypeID componentId = Component<T>::GetTypeID();

    const size_t& compDataSize = mComponentBaseMap[componentId]->GetSize();

    Record& record = mEntiyArchetypeMap[entity];
    Archetype* oldArchetype = record.archetype; // nullptr in the case of the dummy record.
    
    T* newComponent = nullptr; // Return value.

    Archetype* newArchetype = nullptr;

    if(oldArchetype) 
    { 
        if(std::find(oldArchetype->typeId.begin(), oldArchetype->typeId.end(), componentId) != oldArchetype->typeId.end()) 
        {
            throw std::runtime_error("[WARNING] Attempting to AddComponent to an entity which already has the same component!");
            return nullptr;
        };
 
        ArchetypeID newArchetypeId = oldArchetype->typeId;
        newArchetypeId.push_back(componentId);
        std::sort(newArchetypeId.begin(), newArchetypeId.end());

        newArchetype = GetArchetype(newArchetypeId);

        for(size_t i = 0; i < newArchetypeId.size(); i++) 
        {
            const ComponentTypeID& newCompId = newArchetypeId[i];

            IComponentBase* newComp = mComponentBaseMap[newCompId];
            
            const size_t& newCompDataSize = newComp->GetSize();

            size_t currentSize = newArchetype->entities.size() * newCompDataSize;
            size_t newSize = currentSize + newCompDataSize; 

            if(newSize > newArchetype->componentDataSize[i]) 
            {
                newArchetype->componentDataSize[i] *= 2;
                newArchetype->componentDataSize[i] += newCompDataSize;
                
                unsigned char* newData = new unsigned char[newArchetype->componentDataSize[i]];
                
                for(size_t j = 0; j < newArchetype->entities.size(); j++) 
                {
                    newComp->MoveData(&newArchetype->componentData[i][j * newCompDataSize],
                            &newData[j * newCompDataSize]);
                    newComp->DestroyData(&newArchetype->componentData[i][j * newCompDataSize]);
                }

                delete[] newArchetype->componentData[i];

                newArchetype->componentData[i] = newData;
            }

            ArchetypeID oldArchetypeId = oldArchetype->typeId;

            for(size_t j = 0; j < oldArchetype->typeId.size(); j++)
		    {
                const ComponentTypeID& oldCompId = oldArchetype->typeId[j];
                if(oldCompId == newCompId)
                {
                    IComponentBase* oldComp = mComponentBaseMap[oldCompId];

                    const size_t& oldCompDataSize = oldComp->GetSize();

                    oldComp->MoveData(&oldArchetype->componentData[j][record.index * oldCompDataSize],
                                    &newArchetype->componentData[i][currentSize]);
                    oldComp->DestroyData(&oldArchetype->componentData[j][record.index * oldCompDataSize]);

                    goto cnt;
                }
		    }
 
            newComponent = new(&newArchetype->componentData[i][currentSize])T(std::forward<Args>(args)...);

            cnt:;
        }

        if(!oldArchetype->entities.empty()) 
        {
            for(size_t i = 0; i < oldArchetype->typeId.size(); i++) 
            {
                const ComponentTypeID& oldCompTypeID = oldArchetype->typeId[i];

                if(oldCompTypeID == componentId)
                {
                    IComponentBase* removeWrapper = mComponentBaseMap[componentId];
                    removeWrapper->DestroyData(
                        &oldArchetype->componentData[i][record.index * sizeof(T)]);
                }
                
                IComponentBase* oldComp = mComponentBaseMap[oldCompTypeID];

                const size_t& oldCompDataSize = oldComp->GetSize();

                size_t currentSize = oldArchetype->entities.size() * oldCompDataSize;
                size_t newSize = currentSize - oldCompDataSize;

                unsigned char* newData = new unsigned char[oldArchetype->componentDataSize[i] - oldCompDataSize];
                
                oldArchetype->componentDataSize[i] -= oldCompDataSize;
                
                for(size_t j = 0, k = 0; j < oldArchetype->entities.size(); j++)
                {
                    if(j == record.index) 
                    {
                        continue;
                    }

                    oldComp->MoveData(&oldArchetype->componentData[i][j * oldCompDataSize],
                                    &newData[k * oldCompDataSize]);
                    oldComp->DestroyData(&oldArchetype->componentData[i][j * oldCompDataSize]);

                    k++;
                }

                delete[] oldArchetype->componentData[i];

                oldArchetype->componentData[i] = newData;
            }
        }

        std::vector<EntityID>::iterator willBeRemoved = std::find(
            oldArchetype->entities.begin(),
            oldArchetype->entities.end(),
            entity);

        std::for_each(willBeRemoved, oldArchetype->entities.end(),
                    [this, &oldArchetype](const EntityID& ent)
        {
            Record& r = mEntiyArchetypeMap[ent];
            r.index--;
        });

        oldArchetype->entities.erase(willBeRemoved);
    }
    else 
    {
        ArchetypeID newArchetypeId(1, componentId);

        IComponentBase* newComp = mComponentBaseMap[componentId];

        newArchetype = GetArchetype(newArchetypeId);

        size_t currentSize = newArchetype->entities.size() * compDataSize;
        size_t newSize = currentSize + compDataSize;

        if(newSize > newArchetype->componentDataSize[0]) 
        {
            newArchetype->componentDataSize[0] *= 2;
            newArchetype->componentDataSize[0] += compDataSize;
            
            unsigned char* newData = new unsigned char[newArchetype->componentDataSize[0]];
            
            for(size_t i = 0; i < newArchetype->entities.size(); i++) 
            {
                newComp->MoveData(&newArchetype->componentData[0][i * compDataSize],
						&newData[i * compDataSize]);
			    newComp->DestroyData(&newArchetype->componentData[0][i * compDataSize]);
            }

            delete[] newArchetype->componentData[0];

            newArchetype->componentData[0] = newData;
        }

        newComponent = new(&newArchetype->componentData[0][currentSize])T(std::forward<Args>(args)...);
    }

    newArchetype->entities.push_back(entity);
    record.index = newArchetype->entities.size() - 1;
    record.archetype = newArchetype;

    return newComponent;
}


template<class T>
void Registry::RemoveComponent(const EntityID& entity) 
{
    if(!IsComponentRegistered<T>()) 
    {
        throw std::runtime_error("Attempting to RemoveComponent with an unregistered component!");
    }

    ComponentTypeID componentId = Component<T>::GetTypeID();

    if(!mEntiyArchetypeMap.count(entity))   
    {
        return; // Entity doesn't exist.
    }

    Record& record = mEntiyArchetypeMap[entity];

    Archetype* oldArchetype = record.archetype;

    if(!oldArchetype)
        return; // There's no components anyway.

    if(std::find(oldArchetype->typeId.begin(), oldArchetype->typeId.end(), componentId) 
        == oldArchetype->typeId.end())
    {
        return; // This entity doesn't have this component.
    }

    // Find the new archetypeId by removing the old ComponentTypeId.
    ArchetypeID newArchetypeId = oldArchetype->typeId;

    newArchetypeId.erase(
        std::remove(newArchetypeId.begin(), newArchetypeId.end(), componentId),
        newArchetypeId.end()
    );

    std::sort(newArchetypeId.begin(), newArchetypeId.end());

    Archetype* newArchetype = GetArchetype(newArchetypeId);

    for(size_t i = 0; i < newArchetypeId.size(); i++) 
    {
        const ComponentTypeID& newCompId = newArchetypeId[i];

        IComponentBase* newComp = mComponentBaseMap[newCompId];

        const size_t& newCompDataSize = newComp->GetSize();

        size_t currentSize = newArchetype->entities.size() * newCompDataSize;
        size_t newSize = currentSize + newCompDataSize;
        if(newSize > newArchetype->componentDataSize[i])
        {
            newArchetype->componentDataSize[i] *= 2;
            newArchetype->componentDataSize[i] += newCompDataSize;

            unsigned char* newData = new unsigned char[newSize];

            for(size_t j = 0; j < newArchetype->entities.size(); j++)
            {
                newComp->MoveData(&newArchetype->componentData[i][j * newCompDataSize],
                                  &newData[j * newCompDataSize]);
                newComp->DestroyData(&newArchetype->componentData[i][j * newCompDataSize]);
            }

            delete[] newArchetype->componentData[i];

            newArchetype->componentData[i] = newData;
        }

        newComp->ConstructData(&newArchetype->componentData[i][currentSize]);

        ArchetypeID oldArchetypeId = oldArchetype->typeId;

        for(size_t j = 0; j < oldArchetype->typeId.size(); j++) 
        {
            const ComponentTypeID& oldCompId = oldArchetype->typeId[i];

            if(oldCompId == newCompId)
            {
                const std::size_t& oldCompDataSize
                    = mComponentBaseMap[oldCompId]->GetSize();

                IComponentBase* removeWrapper = mComponentBaseMap[oldCompId];
                removeWrapper->MoveData(&oldArchetype->componentData[i][record.index*oldCompDataSize],
                                        &newArchetype->componentData[j][currentSize]);

                removeWrapper->DestroyData(&oldArchetype->componentData[i][record.index*oldCompDataSize]);

                break;
            }
        }
    }


    for(std::size_t i = 0; i < oldArchetype->typeId.size(); ++i)
    {
        const ComponentTypeID& oldCompTypeID = oldArchetype->typeId[i];

        // if this is the component being removed, we should also destruct it
        if(oldCompTypeID == componentId)
        {
            IComponentBase* removeWrapper = mComponentBaseMap[componentId];
            removeWrapper->DestroyData(
                &oldArchetype->componentData[i][record.index*sizeof(T)]);
        }

        IComponentBase* oldComp = mComponentBaseMap[oldCompTypeID];

        const size_t& oldCompDataSize = oldComp->GetSize();

        size_t currentSize = oldArchetype->entities.size() * oldCompDataSize;
        size_t newSize = currentSize - oldCompDataSize;
        unsigned char* newData = new unsigned char[oldArchetype->componentDataSize[i] - oldCompDataSize];
        oldArchetype->componentDataSize[i] -= oldCompDataSize;
        for(size_t j = 0, k = 0; j < oldArchetype->entities.size(); j++)
        {
            if(j == record.index)
                continue;

            oldComp->MoveData(&oldArchetype->componentData[i][j*oldCompDataSize],
                              &newData[k*oldCompDataSize]);
            oldComp->DestroyData(&oldArchetype->componentData[i][j*oldCompDataSize]);

            k++;
        }

        delete[] oldArchetype->componentData[i];

        oldArchetype->componentData[i] = newData;
    }

    std::vector<EntityID>::iterator willBeRemoved = std::find(
        oldArchetype->entities.begin(),
        oldArchetype->entities.end(),
        entity
    );

    std::for_each(willBeRemoved, oldArchetype->entities.end(), [this, &oldArchetype](const EntityID& entityId)
    {
        Record& r = mEntiyArchetypeMap[entityId];
        r.index--; 
    });

    oldArchetype->entities.erase(
        std::remove(oldArchetype->entities.begin(), oldArchetype->entities.end(), entity)
            , oldArchetype->entities.end());

    newArchetype->entities.push_back(entity);
    record.index = newArchetype->entities.size() - 1;
    record.archetype = newArchetype;
}


template<class T>
T* Registry::GetComponent(const EntityID& entity) 
{
    if(!IsComponentRegistered<T>()) 
    {
        throw std::runtime_error("Attempting to RemoveComponent with an unregistered component!");
    }

    ComponentTypeID componentId = Component<T>::GetTypeID();

    if(!mEntiyArchetypeMap.count(entity))   
    {
        return nullptr; // Entity doesn't exist.
    }

    Record& record = mEntiyArchetypeMap[entity];

    Archetype* archetype = record.archetype;

    if(!archetype) 
    {
        return nullptr; // There's no components anyway.
    }

    auto compItr = std::find(archetype->typeId.begin(), 
        archetype->typeId.end(), componentId);

    if(compItr == archetype->typeId.end()) 
    {
        throw std::runtime_error("Attempting to GetComponent from an Entity without that component!");
    } 

    size_t compIndex = std::distance(archetype->typeId.begin(), compItr);;

    return (T*)archetype->componentData[compIndex];
}


template<class T>
bool Registry::HasComponent(const EntityID& entity) 
{
    if(!IsComponentRegistered<T>()) 
    {
        throw std::runtime_error("Attempting to RemoveComponent with an unregistered component!");
    }

    ComponentTypeID componentId = Component<T>::GetTypeID();

    if(!mEntiyArchetypeMap.count(entity))   
    {
        return false; // Entity doesn't exist.
    }

    Record& record = mEntiyArchetypeMap[entity];

    Archetype* oldArchetype = record.archetype;

    if(!oldArchetype) 
    {
        return false; // There's no components anyway.
    }

    if(std::find(oldArchetype->typeId.begin(), oldArchetype->typeId.end(), componentId) 
        != oldArchetype->typeId.end())
    {
        return true; // This entity doesn't have this component.
    }

    return false;
}


void Registry::RemoveEntity(const EntityID& entity) 
{
    if(!mEntiyArchetypeMap.count(entity))
        return; // It doesn't exist.

    Record& record = mEntiyArchetypeMap[entity];

    Archetype* oldArchetype = record.archetype;

    if(!oldArchetype)
    {
        mEntiyArchetypeMap.erase(entity);
        return; // We wouldn't know where to delete.
    }

    for(std::size_t i = 0; i < oldArchetype->typeId.size(); ++i)
    {
        const ComponentTypeID& oldCompId = oldArchetype->typeId[i];

        IComponentBase* comp = mComponentBaseMap[oldCompId];

        const std::size_t& compSize = comp->GetSize();

        comp->DestroyData(&oldArchetype->componentData[i][record.index*compSize]);
    }

    for(std::size_t i = 0; i < oldArchetype->typeId.size(); ++i)
    {
        const ComponentTypeID& oldCompID = oldArchetype->typeId[i];

        IComponentBase* oldComp = mComponentBaseMap[oldCompID];

        const std::size_t& oldCompDataSize = oldComp->GetSize();

        std::size_t currentSize = oldArchetype->entities.size() * oldCompDataSize;
        std::size_t newSize = currentSize - oldCompDataSize;
        unsigned char* newData = new unsigned char[oldArchetype->componentDataSize[i]-oldCompDataSize];
        oldArchetype->componentDataSize[i] -= oldCompDataSize;
        for(std::size_t j = 0, k = 0; j < oldArchetype->entities.size(); j++)
        {
            if(j == record.index)
                continue;

            oldComp->MoveData(&oldArchetype->componentData[i][j*oldCompDataSize],
                              &newData[k*oldCompDataSize]);

            oldComp->DestroyData(&oldArchetype->componentData[i][j*oldCompDataSize]);

            k++;
        }

        delete [] oldArchetype->componentData[i];

        oldArchetype->componentData[i] = newData;
    }

    mEntiyArchetypeMap.erase(entity);

    std::vector<EntityID>::iterator willBeRemoved = std::find(
        oldArchetype->entities.begin(),
        oldArchetype->entities.end(),
        entity);

    std::for_each(willBeRemoved, oldArchetype->entities.end(),
                  [this, &oldArchetype, &entity](const EntityID& entityId)
    {
        if(entityId == entity) 
        {
            return; // No need to adjust our removing one.
        }

        Record& mover = mEntiyArchetypeMap[entityId];
        mover.index -= 1;
    });

    oldArchetype->entities.erase(willBeRemoved);
}


template<class... Ts>
std::vector<EntityID> Registry::GetAllEnittiesWith() 
{

    constexpr size_t argCount = sizeof...(Ts);

    std::vector<ComponentTypeID> components{};

    ProcessComponents<Ts...>(&components);

}


template<class T, class...Ts>
void Registry::ProcessComponents(std::vector<ComponentTypeID>* components) 
{
    if(!IsComponentRegistered<T>()) 
    {
        throw std::runtime_error("Can't get all entities with an unregistered component!");
    }

    components->push_back(Component<T>::GetTypeID());

    ProcessComponents<Ts...>();
}


/**
 * 
 * 
 *  Implementation for System class.
 * 
 * 
*/


/**
 * @brief Simple helper function to sort archetype targets.
*/
template<class... Ts>
ArchetypeID SortTargets(ArchetypeID types)
{
    std::sort(types.begin(), types.end());
    return types;
}


template<class...Cs>
System<Cs...>::System(Registry& registry, const std::uint8_t& layer)
    : mRegistry{registry}, mActionSet{false}
{
    mRegistry.RegisterSystem(layer, this);
}

template<class...Cs>
System<Cs...>::~System() 
{

}

template<class... Cs>
ArchetypeID System<Cs...>::GetArchetypeTarget() const  
{
    return SortTargets({{Component<Cs>::GetTypeID()...}});
}

template<class... Cs>
void System<Cs...>::Action(ActionDef action) 
{
    mAction = action;
    mActionSet = true;
}

template<class... Cs>
template<std::size_t Index1, typename T, typename... Ts>
std::enable_if_t<Index1 == sizeof...(Cs)> System<Cs...>::DoAction(const float elapsedMilliseconds,
    const ArchetypeID& archetypes,
    const std::vector<EntityID>& entities,
    T& t,
    Ts... ts
) 
{
    mAction(elapsedMilliseconds, entities, ts...);
} 

template<class...Cs>
template<size_t Index, typename T, typename... Ts>
std::enable_if_t<Index != sizeof...(Cs)> System<Cs...>::DoAction(const float elapsedMilliseconds,
    const ArchetypeID& archetypes,
    const std::vector<EntityID>& entities,
    T& t,
    Ts... ts
) 
{
    using tuple = std::tuple_element<Index, std::tuple<Cs...>>::type;
    
    std::size_t index2 = 0;
    ComponentTypeID thisTypeCS = Component<tuple>::GetTypeID();
    ComponentTypeID thisArchetypeID = archetypes[index2];

    while(thisTypeCS != thisArchetypeID && index2 < archetypes.size())
    {
        index2++;
        thisArchetypeID = archetypes[index2];
    }

    if(index2 == archetypes.size())
    {
        throw std::runtime_error("System was executed against an incorrect Archetype");
    }

    DoAction<Index+1>(elapsedMilliseconds, archetypes, entities, 
        t, ts..., reinterpret_cast<tuple*>(&t[index2][0]));
}

template<class... Cs>
void System<Cs...>::DoAction(const float elapsedMilliseconds, Archetype* archetype)  
{
    if(mActionSet) 
    {
        DoAction<0>(elapsedMilliseconds, archetype->typeId, 
            archetype->entities, archetype->componentData);
    }
}

}

#endif
