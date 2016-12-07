#include "object.h"




ObjectType::ObjectType(ObjectType &ancestor) :
    Type(ancestor)
{

}

ObjectType::ObjectType(DefaultTypes &parent) :
    Type(parent)
{
    setName(QObject::tr("ObjectTypes"));
    setName(typeName());
    SetFlag(visible, true);
    SetFlag(movable, false);
    SetFlag(interactive, false);
    setParam("life", 42);
}


Object::Object(ObjectType &type, GameObject &parent) :
    TypedObject(type,parent)
{

}



Image::Image(GameObject &parent, const QString &fileName) :
    GameObject(parent)
{
    im.load(fileName);
}

