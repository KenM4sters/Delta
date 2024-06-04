#include "ECS.hpp"
#include <stdexcept>

namespace dt 
{

ECS::ECS()
    : mNumEntities{1}
{

}

ECS::~ECS() 
{

}


const EntityID ECS::GetNewID() 
{
    return mNumEntities++;
}


template<class T>
void ECS::RegisterComponent() 
{
    ComponentTypeID componentTypeID = Component<T>::GetTypeID();

    if(mComponentBaseMap.count(componentTypeID)) 
    {
        return;
    }

    mComponentBaseMap.emplace(componentTypeID, new Component<T>);
}


template<class T>
bool ECS::IsComponentRegistered() 
{

}


void ECS::RegisterSystem(const uint8_t& layer, ISystemBase* system) 
{
    mSystemsMap[layer].push_back(system);
}


void ECS::RegisterEntity(const EntityID& entity) 
{
    Record pch{};
    pch.archetype = nullptr;
    pch.index = 0;
    mEntiyArchetypeMap[entity] = pch;
}


void ECS::RunSystems(const uint8_t layer, const float ts) 
{
    for(const auto& system : mSystemsMap[layer]) 
    {
        const ArchetypeID& key = system.GetKey();

        for(const auto& archetype : mArchetypes) 
        {
            if(std::includes(archetype.type.begin(), archetype.type.end(),
                key.begin(), key.end()))
            {
                // this archetype has all the types required by the system
                // so we can pull it's relevant data, reinterpret them as
                // their correct types, and call the Func in the system
                system.DoAction(ts, archetype);
            }
        }
    }
}


const Archetype* ECS::GetArchetype(const ArchetypeID& id) const 
{
    for(const auto& archetype : mArchetypes) 
    {
        if(archetype->typeId == id) 
        {
            return archetype;
        } 
        else {
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
    } 
}


template<class T, typename... Args>
T* ECS::AddComponent(const EntityID& entity, Args&&... args) 
{
    if(!IsComponentRegistered<T>()) 
    {
        throw std::runtime_error("Attempting to AddComponent with an unregistered component!");
    }

    ComponentTypeID componentId = Component<T>::GetTypeID();

    const size_t compDataSize = mComponentBaseMap[id]->GetSize();

    Record& record = mEntiyArchetypeMap(entity);
    Archetype* oldArchetype = record.archetype; // nullptr in the case of the dummy record.
    
    T* newComponent = nullptr; // Return value.

    Archetype* newArchetype = nullptr;

    if(oldArchetype) 
    {
        if(std::find(oldArchetype.begin(), oldArchetype.end(), componentId)) != oldArchetype.end() 
        {
            throw std::warn("[WARNING] Attempting to AddComponent to an entity which already has the same component!");
            return nullptr; // The entity alread has this component, and can't have more than one of the same component.
        }

        ArchetypeID newArchetypeId = oldArchetype->typeId;
        newArchetypeId.push_back(componentId);
        std::sort(newArchetypeId.begin(), newArchetypeId.end());

        newArchetype = GetArchetype(newArchetypeId);

        for(size_t i = 0; i < newArchetypeId.size(); i++) 
        {
            const ComponentTypeID& newCompId = newArchetypeId[i];
            const IComponentBase* const newComp = mComponentBaseMap[newCompId];
            const size_t& newCompDataSize = newComp->GetSize();

            size_t currentSize = newArchetype->entities.size()*newCompDataSize;
            size_t newSize = currentSize + newCompDataSize;
            if(newSize > newArchetype->componentDataSize[0]) 
            {
                newArchetype->componentDataSize[0] *= 2;
                newArchetype->componentDataSize[0] += componentDataSize;
                
                unsigned char* newData = new unsigned char[newArchetype->componentDataSize[0]];
                
                for(size_t i = 0; i < newArchetype->entities.size(); i++) 
                {
                    newComp->MoveData(&newArchetype->componentData[0][e*newCompDataSize],
                            &newData[e*newCompDataSize]);
                    newComp->DestroyData(&newArchetype->componentData[0][e*newCompDataSize]);
                }

                delete[](newArchetype->componentData[0]);

                newArchetype->componentData[0] = newData;
            }
            ArchetypeID oldArchetype = oldArchetype->typeId;

            for(std::size_t i = 0; i < oldArchetype->type.size(); ++i)
		    {
                const ComponentTypeID& oldCompId = oldArchetype->type[i];
                if(oldCompId == newCompId)
                {
                    const ComponentBase* const oldComp = m_componentMap[oldCompId];

                    const std::size_t& oldCompDataSize = oldComp->GetSize();

                    oldComp->MoveData(&oldArchetype->componentData[i][record.index*oldCompDataSize],
                                    &newArchetype->componentData[j][currentSize]);
                    oldComp->DestroyData(&oldArchetype->componentData[i][record.index*oldCompDataSize]);

                    goto cnt;
                }
		    }

            newComponent = new (&newArchetype->componentData[j][currentSize])T(std::forward<Args>(args)...);

            cnt:;
        }

        if(!oldArchetype->entities.empty()) 
        {
            for(size_t i = 0; i < oldArchetype->entities.size(); i++) 
            {
                const ComponentTypeID& oldCompTypeID = oldArchetype->typeId[i];

                if(oldCompTypeID == newCompId)
                {
                    ComponentBase* removeWrapper = m_componentMap[newCompId];
                    removeWrapper->DestroyData(
                        &oldArchetype->componentData[i][record.index*sizeof(T)]);
                }

                const ComponentBase* const oldComp = m_componentMap[oldCompTypeID];

                const std::size_t& oldCompDataSize = oldComp->GetSize();

                std::size_t currentSize = oldArchetype->entityIds.size() * oldCompDataSize;
                std::size_t newSize = currentSize - oldCompDataSize;
                unsigned char* newData = new unsigned char[oldArchetype->componentDataSize[i]-oldCompDataSize];
                oldArchetype->componentDataSize[i] -= oldCompDataSize;
                for(std::size_t e = 0, ei = 0; e < oldArchetype->entityIds.size(); ++e)
                {
                    if(e == record.index)
                        continue;

                    oldComp->MoveData(&oldArchetype->componentData[i][e*oldCompDataSize],
                                    &newData[ei*oldCompDataSize]);
                    oldComp->DestroyData(&oldArchetype->componentData[i][e*oldCompDataSize]);

                    ++ei;
                }

                delete [] oldArchetype->componentData[i];

                oldArchetype->componentData[i] = newData;
            }
        }

        std::vector<EntityID>::iterator willBeRemoved = std::find(
            oldArchetype->entityIds.begin(),
            oldArchetype->entityIds.end(),
            entityId);

        std::for_each(willBeRemoved, oldArchetype->entityIds.end(),
                    [this,&oldArchetype](const EntityID& entityId)
        {
            Record& r = m_entityArchetypeMap[enittyId];
            r.index--;
        });

        oldArchetype->entityIds.erase(willBeRemoved);
    }
    else 
    {
        ArchetypeID newArchetypeId(1, componentId);

        const IComponentBase* const newComponent = mComponentBaseMap[componentId];

        newArchetype = GetArchetype(newArchetypeId);

        size_t currentSize = newArchetype->entities.size()*compDataSize;
        size_t newSize = currentSize + compDataSize;
        if(newSize > newArchetype->componentDataSize[0]) 
        {
            newArchetype->componentDataSize[0] *= 2;
            newArchetype->componentDataSize[0] += componentDataSize;
            
            unsigned char* newData = new unsigned char[newArchetype->componentDataSize[0]];
            
            for(size_t i = 0; i < newArchetype->entities.size(); i++) 
            {
                newComp->MoveData(&newArchetype->componentData[0][e*compDataSize],
						&newData[e*compDataSize]);
			    newComp->DestroyData(&newArchetype->componentData[0][e*compDataSize]);
            }

            delete[](newArchetype->componentData[0]);

            newArchetype->componentData[0] = newData;
        }
        newComponent = new (&newArchetype->componentData[0][currentSize])T(std::forward<Args>(args)...);
    }

    newArchetype->entities.push_back(entity);
    record.index = newArchetype->entities.size() - 1;
    record.archetype = newArchetype;

    return newComponent;
}


template<class T>
void ECS::RemoveComponent(const EntityID& entity) 
{
    if(!IsComponentRegistered<T>()) 
    {
        throw std::runtime_error("Attempting to AddComponent with an unregistered component!");
    }

    ComponentTypeID componentId = Component<T>::GetTypeID();

    if(!mEntiyArchetypeMap.contains(entity)) 
    {
        return; // It doesn't exist.
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
        const ComponentTypeID& newCompId = newArchetypeId[j];

        const IComponentBase* const newComp = mComponentBaseMap[newCompId];

        const size_t& newCompDataSize = newComp->GetSize();

        size_t currentSize = newArchetype->entities.size() * newCompDataSize;
        size_t newSize = currentSize + newCompDataSize;
        if(newSize > newArchetype->componentDataSize[j])
        {
            newArchetype->componentDataSize[j] *= 2;
            newArchetype->componentDataSize[j] += newCompDataSize;

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
}


template<class T>
T* ECS::GetComponent(const EntityID& entity) 
{

}


template<class T>
bool ECS::HasComponent(const EntityID& entity) 
{

}


void ECS::RemoveEntity(const EntityID& entity) 
{

}


template<class... Ts>
std::vector<EntityID> ECS::GetAllEnittiesWith() 
{

}


}