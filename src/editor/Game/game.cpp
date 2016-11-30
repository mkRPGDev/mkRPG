#include "game.h"




World::World(Game *g, GameObject *parent) :
    GameObject(g, parent)
{
}





Game::Game() :
    GameObject(),
    idDisp(0), w(new World(this, this)), map(nullptr)
{
    init(this,nullptr);
}

Game::~Game(){
    for(Image* i : picts.values())
        delete i;
    delete w;
}

void Game::addImage(Image *im){
    picts[im->ident()] = im;
}

void Game::setCurrentMap(Map *m){
    map = m;
}




int Game::newIdent(GameObject *obj){
    objects[++idDisp] = obj;
    return idDisp;
}

void Game::aboutToDestroy(GameObject *obj){
    objects.remove(obj->ident());
}


GameObject* Game::object(int id){
    return objects[id];
}

