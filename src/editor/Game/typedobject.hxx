#ifndef TYPEDOBJECT_HXX
#define TYPEDOBJECT_HXX

#include "gameobject.h"

/*!
 * \file typedobject.hxx
 *
 * \brief Definition of some template type-related base classes.
 */


/*!
 *\class Type
 * \brief The Type class represent a type of object.
 *
 * It adds to GameObjectType a constrain on the objects
 * that can be used as type, defined by le \c class \c T
 * template argument.
 *
 * \note
 * The \c class \c T template argument has to be inherited from Type.
 * (the application won't compile otherwise)
 *
 * \see CellType, ObjectType
 */
template<class T>
class Type : public GameObjectType
{
public:
    Type(DefaultTypes &parent) :
        GameObjectType(parent), aTypedAncestor(nullptr)
    {}
    Type(T &ancestor) :
        GameObjectType(ancestor), aTypedAncestor(&ancestor)
    {
        ancestor.typedDescendants.append(static_cast<T*>(this));
    }

    const QList<T*> &descendants() const{
        return typedDescendants;
    }

protected:
    T *aTypedAncestor;

private:
    QList<T*> typedDescendants;
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
 * (the application won't compile otherwise)
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


    T& objectType(){
        return *aObjectType;
    }
    void setObjectType(T &type){
        aObjectType = &type;
        aAncestor = &type;
    }

protected:
    T *aObjectType;

};

#endif // TYPEDOBJECT_HXX
