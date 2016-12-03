#ifndef TYPEDOBJECT_HXX
#define TYPEDOBJECT_HXX

#include "gameobject.h"

/*!
 * \file typedobject.hxx
 *
 * \brief This file define some templates base classes.
 */


/*!
 *\class Type
 * \brief The Type class is the base class for types of
 * TypedObject.
 *
 * It adds to InheritableObject a constrain on the objects
 * that can be used as type, defined by le \c class \c T
 * template argument.
 *
 * \note
 * The \c class \c T template argument has to be inherited from Type.
 *
 * \see CellType, ObjectType
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
 * \brief The TypedObject class is the base class for GameObject
 * with a mutable Type.
 *
 * It adds to InheritableObject the guarranty that the object has
 * a type, with a constrain on it defined by le \c class \c T template
 * argument.
 *
 *
 * \note
 * The \c class \c T template argument has to be inherited from Type.
 *
 * \see Cell, Object
 */
template<class T>
class TypedObject : public InheritableObject
{
public:
    TypedObject(T &type, GameObject &parent) :
        InheritableObject(parent, &type), aObjectType(&type)
    {}


    const T& objectType() const{
        return *aObjectType;
    }
    void setObjectType(T &type){
        aObjectType = &type;
        aAncestor = &type;
    }

protected:
    const T *aObjectType;

};

#endif // TYPEDOBJECT_HXX
