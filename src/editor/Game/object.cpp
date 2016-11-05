#include "object.h"
#include "game.h"


Object::Object(Game* g, Object *parent) :
    parent(parent),
    game(g), id(g ? g->newIdent() : 0), lastEdit(QDateTime::currentDateTime())
{

}

void Object::init(Game *g, Object* p){
    if(g == nullptr){
        game = g;
        id = g->newIdent();
        parent = p;
    }
}

void Object::touch(){
    Editing;
}



Image::Image(Game *g, Object *parent, const QString &fileName) :
    Object(g, parent)
{
    im.load(fileName);
}

