#ifndef SILVERBACK_COMPONENT_HPP
#define SILVERBACK_COMPONENT_HPP

#include <TypeId.hpp>

#include <iostream>

namespace slv 
{

/**
 * @brief The base Component interface ensures that any "component" that the user
 * comes up with can be stores within an Archetype using a pointer and 
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
 * System instances that subscribe to at least this component (could be more), or 
 * more specifically the Archetype that the component belongs to. 
 * We're managing memory ourselves here so that the user doesn't need to worry about or 
 * inherit from some generic base class. Remember, the goal of this framework to 
 * release an API that's as simple, and intuitive as possible.
*/
template<class T>
class Component : public IComponentBase 
{
public:
    virtual void DestroyData(unsigned char* data) override;
    virtual void MoveData(unsigned char* source, unsigned char* destination) override;
    virtual void ConstructData(unsigned char* data) override;

    virtual size_t GetSize() const override;

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

