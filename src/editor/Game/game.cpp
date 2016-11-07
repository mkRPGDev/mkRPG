#include "game.h"




World::World(Game *g, BaseObject *parent) :
    BaseObject(g, parent)
{
}





Game::Game() :
    BaseObject(this),
    idDisp(0), w(World(this, this)), map(nullptr)
{

}

void Game::addImage(Image *im){
    picts[im->ident()] = im;
}

void Game::setCurrentMap(Map *m){
    map = m;
}
