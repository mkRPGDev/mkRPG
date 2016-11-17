#include "game.h"




World::World(Game *g, GameObject *parent) :
    GameObject(g, parent)
{
}





Game::Game() :
    GameObject(),
    idDisp(0), w(World(this, this)), map(nullptr)
{
    init(this,nullptr);
}

void Game::addImage(Image *im){
    picts[im->ident()] = im;
}

void Game::setCurrentMap(Map *m){
    map = m;
}









