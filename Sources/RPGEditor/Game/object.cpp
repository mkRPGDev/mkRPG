#include "object.h"
#include "game.h"


Object::Object(Game* g) :
    game(g), id(g ? g->newIdent() : 0)
{

}

void Object::init(Game *g){
    if(g == nullptr){
        game = g;
        id = g->newIdent();
    }
}


Image::Image(Game *g, const QString &fileName) :
    Object(g)
{
    im.load(fileName);
}

