#ifndef ECS_SYSTEM_HPP
#define ECS_SYSTEM_HPP
#include "TypeId.hpp"
#include "Archetype.hpp"
#include <functional>

namespace dt 
{

class ECS;

class ISystemBase 
{
public:
    virtual ~ISystemBase() {}

    virtual ArchetypeID GetArchetypeTarget() const = 0;
    
    virtual void DoAction(float elapsedTime, Archetype* archetype) const = 0;

};


template<class... Cs>
class System : public ISystemBase 
{
public:
	System(ECS& ecs, const std::uint8_t& layer);
    ~System();

	typedef std::function<void(const float, const std::vector<EntityID>&, Cs*...)> ActionDef;

	virtual ArchetypeID GetArchetypeTarget() const override;

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

	virtual void DoAction(const float elapsedMilliseconds, Archetype* archetype) const override;

	ECS& mECS;
	ActionDef mAction;
	bool mActionSet{false};

};
}

#endif