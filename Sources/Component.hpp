#ifndef ECS_COMPONENT_HPP
#define ECS_COMPONENT_HPP
#include <iostream>
#include <TypeId.hpp>

namespace dt 
{
class IComponentBase 
{
public:
    virtual ~IComponentBase() {}

    virtual void DestroyData(unsigned char* data) = 0;
    virtual void MoveData(unsigned char* source, unsigned char* data) = 0;
    virtual void ConstructData(unsigned char* data) = 0;

    virtual size_t GetSize() const = 0;

}; 


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
}

#endif

