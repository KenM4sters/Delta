#include "Component.hpp"

namespace slv 
{

template<class T>
void Component<T>::DestroyData(unsigned char* data) 
{
    T* dataLoc = std::launder(reinterpret_cast<T*>(data));
    dataLoc->~T();
}

template<class T>
void Component<T>::MoveData(unsigned char* source, unsigned char* destination) 
{
    new (&destination[0]) T(std::move(reinterpret_cast<T*>(source)));
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