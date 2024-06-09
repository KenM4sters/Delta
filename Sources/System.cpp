#include "System.hpp"
#include "Registry.hpp"
#include <tuple>

namespace slv 
{

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
    return SortTargets({Component<Cs>::GetTypeId()...});
}

template<class... Cs>
void System<Cs...>::Action(ActionDef action) 
{
    mAction = action;
    mActionSet = true;
}

template<class... Cs>
template<std::size_t Index1, typename T, typename... Ts>
std::enable_if_t<Index1==sizeof...(Cs)> System<Cs...>::DoAction(const float elapsedMilliseconds,
    const ArchetypeID& archetypes,
    const std::vector<EntityID>& entities,
    T& t,
    Ts... ts
) 
{
    mAction(elapsedMilliseconds, entities, ts...);
}

template<class...Cs>
template<std::size_t Index, typename T, typename... Ts>
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
void System<Cs...>::DoAction(const float elapsedMilliseconds, Archetype* archetype) const  
{
    if(mActionSet) 
    {
        DoAction<0>(elapsedMilliseconds, archetype->typeId, 
            archetype->entities, archetype->componentData);
    }
}
}