#include "object.h"
#include "game.h"


BaseObject::BaseObject(Game* g, BaseObject *parent) :
    parent(parent),
    game(g), id(g ? g->newIdent() : 0), lastEdit(QDateTime::currentDateTime())
{
    setParent3(nullptr);
}

void BaseObject::init(Game *g, BaseObject* p){
    if(g == nullptr){
        game = g;
        id = g->newIdent();
        parent = p;
    }
}

void BaseObject::touch(){
    lastEdit = QDateTime::currentDateTime();
    if(parent) parent->lastChildEdit = lastEdit;
    //setParent3(this);
}








Object::Object(Game *g, BaseObject *parent) :
    BaseObject(g,parent)
{
    SetFlag(visible, true);
}



Image::Image(Game *g, BaseObject *parent, const QString &fileName) :
    BaseObject(g, parent)
{
    im.load(fileName);
}

