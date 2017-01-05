#ifndef ENTITY_H
#define ENTITY_H



/*!
 * \file entity.h
 *
 * \brief Definition of the Entity related classes.
 */


#include "object.h"



class EntityType; /*!
 * \brief The EntityType class defines the properties that are
 * mandatory in any EntityType object.
 */
class EntityType : public Type<EntityType>
{
public:
    TypeName(EntityType)
    EntityType(EntityType &ancestor);
    EntityType(DefaultTypes &parent);


    inline Image* image() const{return aImage ? aImage : aTypedAncestor ? aTypedAncestor->image() : nullptr;}
    inline void setImage(Image* im){aImage = im; touch();}


private:
    Image * aImage;
};

/*!
 * \brief The Entity class
 */
class Entity : public TypedObject<EntityType>
{
public:
    TypeName(Entity)
    Entity(EntityType &type, GameObject &parent);

    EntityType &entityType(){return *aObjectType;}

};





#endif // ENTITY_H
