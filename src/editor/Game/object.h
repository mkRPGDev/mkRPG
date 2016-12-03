#ifndef OBJECT_H
#define OBJECT_H

#include "typedobject.hxx"

/*!
 * \file object.h
 *
 * \brief Definition of the base class GameObject, and some inherited classes.
 *
 * ## The objects structure
 *
 * ### Inheritance and parent
 *
 * Each class which is part of game representation has to inherit from GameObject.
 * It gives the class standard params and flags mechanisms that are used for edition
 * purposes.
 *
 * Each instance must have a parent (except the Game one) which will keep an eye on
 * his children. If the parent cannot be given at the construction time (case or array)s,
 * the \ref init function must then be called as soon as possible.
 *
 *
 * ### Reference count
 *
 * ### %Objects destructors
 *
 * A default implementation of GameObject destructor is provided. It destroy every
 * children the instance has. This avoid fastidious destructor implementation.
 *
 *
 * ## The Macro System
 *
 * To add conveniently attributes and flags to GameObject subclassed objects,
 * a set of macro is provided.
 *
 * ### Name conventions
 *
 * For a attribute named \c attr, the following conventions are observed :
 * - \c attr() is the getter method
 * - \c setAttr() is the setter method
 * - \c aAttr is the name of the attribut (if any)
 *
 * A specific convention is applied for flags (boolean attributes) :
 * - isAttr() is the getter method
 *
 * ### Macros
 *
 * To define a new attribute, a global macro can be used in the class declaration.
 * The provided basic implementations keep the object edition synchronization.
 *
 * If a cleverer process is needed, custom getter or setter can be implemented,
 * and the getter and setter macros can be used separately to define the obvious methods
 *
 * **Provided macros**
 *
 * Attribute type               | Complete declaration  | Getter      | Setter
 * -----------------------------|-----------------------|-------------|------------
 * Flags (bool)                 | #Flag                 |#FlagGetter  |#FlagSetter
 * Parameters (int)             | #Param                |#ParamGetter |#ParamSetter
 * GameObject based Attributes  | #Attr                 |#AttrGetter  |#AttrSetter
 *
 *
 * **The case of attributes**
 *
 * An additionnal #AttrT macro is provided, that deduce a default name from the type.
 *
 *
 * ### Name tools
 *
 * To make the definition easier and avoid the name repetition that is introduced by
 * the name convention, a #C macro is provided to construct the names with lower and upper
 * initial letter from theses letter and the end of the name.
 *
 *
 * \author Baptiste Pauget
 */








/*!
 * \brief The Image class stores an external file in a QImage, and
 * gives each image ressources a unique identifier.
 */
class Image : public GameObject
{
public:
    TypeName(Image)
    Image(GameObject &parent, const QString &fileName);
    inline bool isValid() const{return GameObject::isValid() && !im.isNull();} /**<
     * Checks if the GameObject is valid and if the image really exists.
     */
    inline const QImage& image() const{return im;}
    inline const QSize size() const{return im.size();}
    void update(){} // TODO
private:
    QImage im;
};




class ObjectType; /*!
 * \brief The ObjectType class defines the properties that are
 * mandatory in any ObjectType object.
 */
class ObjectType : public Type<ObjectType>
{
public:
    ObjectType(ObjectType &ancestor) : Type(ancestor){}
    ObjectType(GameObject &parent) : Type(parent){}
private:
};

/*!
 * \brief The Object class
 */
class Object : public TypedObject<ObjectType>
{
public:
    TypeName(Object)
    Object(ObjectType &type, GameObject &parent);

    C0(Flag, v,V,isible)
    C0(Flag, m,M,ovable)
    C0(Flag, i,I,nteractive)
    C0(AttrT,i,I,mage)

};




#endif // OBJECT_H
