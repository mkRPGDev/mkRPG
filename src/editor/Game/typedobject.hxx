#ifndef TYPEDOBJECT_HXX
#define TYPEDOBJECT_HXX

#include "gameobject.h"

/*!
 * \file typedobject.hxx
 *
 * \brief This file define some templates base classes.
 */





/*!
 * \brief The Type class
 */
template<class T>
class Type : public GameObjectType
{
public:
    Type(T* ancestor, Game *g, GameObject *parent) : GameObjectType(ancestor, g, parent){}

    Type(Game*g, GameObject *parent) : GameObjectType(g, parent){} // temporaire
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
class TypedObject : public Type<T>
{
public:
    TypedObject(Game*g, GameObject *aParent) : Type<T>(g, aParent)/*, aType(T::defaultType())*/{} // temporaire

    TypedObject(T &type, Game *g, GameObject *aParent) : Type<T>(&type, g, aParent) {}



    const T& objectType(){return *Type<T>::aAncestor;}
    void setObjectType(const T &type){Type<T>::aAncestor = &type;}

protected:


};

#endif // TYPEDOBJECT_HXX
