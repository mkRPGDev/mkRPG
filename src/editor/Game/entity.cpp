#include "entity.h"

EntityType::EntityType(EntityType &ancestor) :
    Type(ancestor), aImage(nullptr)
{
}

EntityType::EntityType(DefaultTypes &parent) :
    Type(parent), aImage(nullptr)
{
    setName(QObject::tr("EntityTypes"));
    setName(typeName());
    SetFlag(sleeping, true);
    setParam("life", 79);
    setParam("x", 5);
    setParam("y", 5);
}


Entity::Entity(EntityType &type, GameObject &parent) :
    TypedObject(type,parent)
{
}

