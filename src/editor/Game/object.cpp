#include "object.h"









Object::Object(Game *g, GameObject *parent) :
    TypedObject(g,parent)
{
    SetFlag(visible, true);
}



Image::Image(Game *g, GameObject *parent, const QString &fileName) :
    GameObject(g, parent)
{
    im.load(fileName);
}

