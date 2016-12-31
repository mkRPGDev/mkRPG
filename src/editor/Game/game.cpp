#include "game.h"

DefaultTypes::DefaultTypes(World &parent) :
    GameObject(parent)
{
    setName(QObject::tr("Types"));
}












GameObjectInventory::GameObjectInventory(World &parent) :
    GameObject(parent)
{
    setName(QObject::tr("Inventory"));
    ListInitD(Map,,s);
    ListInitD(Object,,s);
}









World::World(Game *g, GameObject *parent) :
    GameObject(g, parent),
    aTypes(new DefaultTypes(*this)),
    aObjects(new GameObjectInventory(*this))
{}

DefaultTypes &World::types(){
    return *aTypes;
}

GameObjectInventory &World::objects(){
    return *aObjects;
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




QList<QString> Game::actions() const{
    return aActions.keys();
}

Action *Game::action(const QString &a){
    return aActions.value(a, nullptr);
}

QString Game::addAction(QString a, Action *act){
    if(aActions.contains(a)){
        int k(1);
        while(aActions.contains(a+"_"+QString::number(++k)));
        a+="_"+QString::number(k);
    }
    aActions[a] = act;
    return a;
}

void Game::removeAction(const QString &a){
    aActions.remove(a);
}

QString Game::renameAction(const QString &a, const QString &nv){
    if(!aActions.contains(a)) return a;
    Action *act = aActions[a];
    removeAction(a);
    return addAction(nv, act);
}
