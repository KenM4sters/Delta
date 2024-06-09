#ifndef SILVERBACK_SYSTEM_HPP
#define SILVERBACK_SYSTEM_HPP

#include "TypeId.hpp"
#include "Archetype.hpp"

#include <functional>

namespace slv 
{

class Registry;

/**
 * @brief Base System interface.
*/
class ISystemBase 
{
public:
    virtual ~ISystemBase() {}

    virtual ArchetypeID GetArchetypeTarget() const = 0;
    
    virtual void DoAction(float elapsedTime, Archetype* archetype) const = 0;

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

	virtual void DoAction(const float elapsedMilliseconds, Archetype* archetype) const override;
	
	void Action(ActionDef action);

private:
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

private:
	Registry& mRegistry;

	ActionDef mAction;
	
	bool mActionSet = false;

};
}

#endif