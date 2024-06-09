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
}

#endif

