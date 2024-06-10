#ifndef SILVERBACK_COMPONENT_HPP
#define SILVERBACK_COMPONENT_HPP

#include <TypeId.hpp>

#include <iostream>

namespace slv 
{

/**
 * @brief Base Component interface.
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
 * @brief Derived Component.
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

