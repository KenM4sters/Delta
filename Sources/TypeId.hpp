#ifndef SILVERBACK_TYPEID_HPP
#define SILVERBACK_TYPEID_HPP

#include <iostream>
#include <vector>

namespace slv 
{

typedef uint32_t TypeID;
typedef TypeID EntityID;
typedef TypeID ComponentTypeID;
typedef std::vector<ComponentTypeID> ArchetypeID;

constexpr TypeID cNullEntity = 0;

/**
 * @brief Generates a new ID for each component of each type.
*/
template<class T>
class TypeIDGenerator 
{
public:
    template<class U>
    static const TypeID GetNewID() 
    {
        static TypeID const newId = mCount++; // Must be static to retain value of each type of component.
        return newId;
    }

private:
    static TypeID mCount;
};


template<class T> 
TypeID TypeIDGenerator<T>::mCount = 0; // Has to be defined within this header. 

}

#endif