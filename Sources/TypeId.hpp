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
 * @brief The TypeIDGenerator's sole purpose to correctly distribute unique Ids to 
 * Entities and Components depending on the template used for the class 
 * itself and the template used for the GetNewID() member function.  
 * Note the static private mCount variable here - for every template of the class,
 * a new mCount variable will be created and initialized to zero, but its value will
 * be correctly incremented for each call to the GetNewID() member function that belongs
 * to that same class.
*/
template<class T>
class TypeIDGenerator 
{
public:
    /**
     * @brief Returns a unique integer which is used to represent both entities and
     * components, although the two will never be confused in implementation. Note that
     * the id returned is based on a static variable, mCount which increments by 1 each
     * time this function is called. Bare in mind that both this class AND function is
     * templated, meaning that many static instances of mCount exist independantly of 
     * each other, essentially allowing us to have intances of mCount for each different
     * component.
     */
    template<class U>
    static const TypeID GetNewID() 
    {
        static const TypeID newId = mCount++; // Must be static to retain value of each type of component.
        return newId;
    }

private:
    static TypeID mCount;
};


template<class T> 
TypeID TypeIDGenerator<T>::mCount = 0;

}

#endif