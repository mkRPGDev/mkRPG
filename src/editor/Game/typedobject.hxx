#ifndef TYPEDOBJECT_HXX
#define TYPEDOBJECT_HXX

#include "gameobject.h"

/*!
 * \file typedobject.hxx
 *
 * \brief This file define some templates base classes.
 */



template<class T>
class Type : public GameObjectType
{
public:
    Type(GameObject &parent) :
        GameObjectType(parent)
    {}
    Type(T &ancestor) :
        GameObjectType(ancestor)
    {}

protected:
};


/*!
 * \brief The TypedObject class
 *
 * This class inherited from Type, and provide the same behaviour,
 * except that the ancestor cannot be a null pointer, and it can be
 * modified after the construction.
 *
 * \note
 * The \c class \c T template argument has to be inherited from Type.
 */
template<class T>
class TypedObject : public InheritableObject
{
public:
    TypedObject(T &type, GameObject &parent) :
        InheritableObject(parent, &type), aObjectType(type)
    {}


    const T& objectType() const{
        return aObjectType;
    }
    void setObjectType(const T &type){
        aObjectType = type;
        aAncestor = &type;
    }

protected:
    const T &aObjectType;

};

#endif // TYPEDOBJECT_HXX
