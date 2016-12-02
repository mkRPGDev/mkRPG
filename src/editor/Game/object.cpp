#include "object.h"



void ObjectType::initialise(){

}



Object::Object(ObjectType &type, GameObject &parent) :
    TypedObject(type,parent)
{
    SetFlag(visible, true);
    SetFlag(movable, false);
    SetFlag(interactive, false);

}



Image::Image(GameObject &parent, const QString &fileName) :
    GameObject(parent)
{
    im.load(fileName);
}

