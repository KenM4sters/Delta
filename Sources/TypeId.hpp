#ifndef ECS_TYPEID_HPP
#define ECS_TYPEID_HPP
#include <iostream>
#include <vector>

namespace dt 
{

typedef uint32_t TypeID;
typedef TypeID EntityID;
typedef TypeID ComponentTypeID;
typedef std::vector<ComponentTypeID> ArchetypeID;
const TypeID NullEntity = 0;

template<class T>
class TypeIDGenerator 
{
public:
    template<class U>
    static const TypeID GetNewID() 
    {
        static TypeID const newId = mCount++;
        return newId;
    }

private:
    static TypeID mCount;
};

template<class T> TypeID TypeIDGenerator<T>::mCount = 0;

}

#endif