#include "game.h"




World::World(Game *g, Object *parent) :
    Object(g, parent)
{
}





Game::Game() :
    idDisp(0), w(World(this, this)), map(nullptr)
{

}

void Game::addImage(Image *im){
    picts[im->ident()] = im;
}

void Game::setCurrentMap(Map *m){
    map = m;
}
