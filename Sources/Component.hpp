#ifndef SILVERBACK_COMPONENT_HPP
#define SILVERBACK_COMPONENT_HPP

#include <TypeId.hpp>

#include <iostream>

namespace slv 
{

/**
 * @brief The base Component interface ensures that any "component" that the user
 * comes up with can be stored within an Archetype using a pointer and 
 * polymorphism. 
*/
class IComponentBase 
{
public:
    virtual ~IComponentBase() {}

    virtual void DestroyData(unsigned char* data) = 0;
    virtual void MoveData(unsigned char* source, unsigned char* data) = 0;
    virtual void ConstructData(unsigned char* data) = 0;
    virtual size_t GetSize() const = 0;
}; 


/**
 * @brief The derived component class is essentially a wrapper around the data
 * that the user is trying to associate an entity with. All "logic" depends on the 
 * System instances that care about at least this component (could be more), or 
 * more specifically the Archetype that the component belongs to. 
 * We're managing memory ourselves here so that the user doesn't need to worry about or 
 * inherit from some generic base class. Remember, the goal of this framework to 
 * release an API that's as simple and intuitive as possible.
 * The magic here is using templates to get Ids from the TypeIDGenerator on a per
 * template basis, which will in use be a per component type basis (Velocity, Position etc...).
*/
template<class T>
class Component : public IComponentBase 
{
public:
    /**
     * @brief Destorys the data given to it. Since we're managing the data associated
     * with each component ourselves, we will inevitably have to contstruct, move and 
     * destroy it whenever we need to allocate new memory for it, for example when we have 
     * more component data than space available in the current memory block.
     * @param data A pointer to the memory block where the data belongs. 
     */
    virtual void DestroyData(unsigned char* data) override;

    /**
     * @brief Moves the data given to it to a specified destination. 
     * Since we're managing the data associated with each component ourselves, 
     * we will inevitably have to contstruct, move and destroy it whenever we need 
     * to allocate new memory for it, for example when we have more component data 
     * than space available in the current memory block. 
     * @param source A pointer to the data which we wish to move.
     * @param destination A pointer to the memory where we wish to move the source data to.
     */
    virtual void MoveData(unsigned char* source, unsigned char* destination) override;
        
    /**
     * @brief Constructs a T object at the specified data location. 
     * Since we're managing the data associated with each component ourselves, 
     * we will inevitably have to contstruct, move and destroy it whenever we 
     * need to allocate new memory for it, for example when we have more component 
     * data than space available in the current memory block. 
     * @param data A pointer to the memory where we wish to construct T at.
     */
    virtual void ConstructData(unsigned char* data) override;

    /**
     * @brief Returns the size of T in bytes. This will be paramount to properly navigating
     * through the componentData vector of an Archetype when it comes to constructing, moving
     * and destroying data - we have to know the size of each component so that we know
     * which memory to target based on the componentId using pointer arithmetic. 
     * @return The size of T in bytes.
     */
    virtual size_t GetSize() const override;

    /**
     * @brief An abstraction from the TypeIDGenerator class so that we don't have to 
     * interact with it from the registry. 
     * @returns A uint32_t represting the ID generated from the TypeIDGenerator for
     * this specific component type.
     */
    static const ComponentTypeID GetTypeID();
};


template<class T>
void Component<T>::DestroyData(unsigned char* data) 
{
    T* dataLoc = std::launder(reinterpret_cast<T*>(data));
    dataLoc->~T();
}

template<class T>
void Component<T>::MoveData(unsigned char* source, unsigned char* destination) 
{
    new (&destination[0]) T(std::move(*reinterpret_cast<T*>(source)));
}

template<class T>
void Component<T>::ConstructData(unsigned char* data) 
{
    new (&data[0]) T();
}

template<class T>
size_t Component<T>::GetSize() const 
{
    return sizeof(T);
}

template<class T>
const ComponentTypeID Component<T>::GetTypeID() 
{
    return TypeIDGenerator<IComponentBase>::GetNewID<T>();
}


}

#endif

