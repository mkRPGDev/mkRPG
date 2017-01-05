#include "object.h"




ObjectType::ObjectType(ObjectType &ancestor) :
    Type(ancestor), aImage(nullptr)
{

}

ObjectType::ObjectType(DefaultTypes &parent) :
    Type(parent), aImage(nullptr)
{
    setName(QObject::tr("ObjectTypes"));
    setName(typeName());
    SetFlag(visible, true);
    SetFlag(movable, false);
    SetFlag(interactive, false);
    setParam("life", 42);
    setParam("x", 5);
    setParam("y", 5);
}


Object::Object(ObjectType &type, GameObject &parent) :
    TypedObject(type,parent)
{

}



Image::Image(GameObject &parent, const QString &fileName) :
    GameObject(parent)
{
    im.load(fileName);
    QFileInfo in(fileName);
    setName(in.fileName().left(in.fileName().lastIndexOf(".")));
}

