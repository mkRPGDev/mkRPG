#ifndef OBJECT_H
#define OBJECT_H

#include "typedobject.hxx"



/*!
 * \file object.h
 *
 * \brief Definition of the Object related classes.
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
    TypeName(ObjectType)
    ObjectType(ObjectType &ancestor);
    ObjectType(DefaultTypes &parent);

    inline Image* image() const{return aImage ? aImage : aTypedAncestor ? aTypedAncestor->image() : nullptr;}
    inline void setImage(Image* im){aImage = im; touch();}


private:
    Image * aImage;
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
