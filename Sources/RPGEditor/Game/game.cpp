#include "game.h"




World::World(Game *g) :
    Object(g)
{
}





Game::Game() :
    idDisp(0), w(World(this))
{

}

void Game::addImage(Image *im){
    picts[im->ident()] = im;
}
