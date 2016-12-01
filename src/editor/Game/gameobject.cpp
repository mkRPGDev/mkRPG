#include "gameobject.h"

#include "game.h"


GameObject::GameObject(Game* g, GameObject *parent) :
    aParent(parent), game(g), nbRef(0),
    lastEdit(QDateTime::currentDateTime()),
    lastChildEdit(lastEdit),
    id(g ? g->newIdent(this) : 0)
{
    if(parent) parent->addChild(this);
    setName("Untitled");
}


GameObject::~GameObject(){
    if(id) game->aboutToDestroy(this);
    if(nbRef)
        qDebug() << "Salut..." << id << "Références" << nbRef;
    for(GameObject* c : aChildren.values()) delete c;
    if(aParent) aParent->removeChild(this);
}

void GameObject::init(Game *g, GameObject* parent){
    if(game == nullptr){
        game = g;
        id = g->newIdent(this);
        aParent = parent;
        if(aParent) aParent->addChild(this);
    }
}

void GameObject::setParent(GameObject *p){
    if(aParent) aParent->removeChild(this);
    aParent = p;
    if(aParent) aParent->addChild(this);
}

void GameObject::touch(){
    lastEdit = QDateTime::currentDateTime();
    if(aParent) aParent->childrenTouched(lastEdit);
}

void GameObject::childrenTouched(const QDateTime &d){
    lastChildEdit = d;
    if(aParent) aParent->childrenTouched(d);
}

void GameObject::addChild(GameObject *c){
    assert(c != nullptr);
    aChildren[c->ident()] = c;
}

void GameObject::removeChild(GameObject *c){
    assert(c != nullptr);
    aChildren.remove(c->ident());
}

void GameObject::addReference(){
    ++nbRef;
}

void GameObject::removeReference(){
    --nbRef;
}

QList<QString> GameObject::filter(QList<QString> l) const{
    l.erase(std::remove_if(l.begin(), l.end(), [this](QString &a){return reserved.contains(a);}), l.end());
    return l;
}

void GameObject::setName(const QString &n){
    aName = n;
}





GameObjectType::GameObjectType(GameObjectType* ancestor, Game *g, GameObject *aParent) :
    GameObject(g, aParent), aAncestor(ancestor)
{
    if(ancestor != nullptr) ancestor->addDescendant(this);
}
GameObjectType::GameObjectType(Game*g, GameObject *aParent) : GameObject(g, aParent), aAncestor(nullptr){} // temporaire


bool GameObjectType::isInheritedParam(const QString &p) const {
    return aAncestor ? aAncestor->hasParam(p) : false;
}
bool GameObjectType::isRedefiniedParam(const QString &p) const {
    return isInheritedParam(p) && GameObject::hasParam(p);
}
int GameObjectType::getParam(const QString &p) const {
    return GameObject::hasParam(p) || aAncestor==nullptr ?
                GameObject::getParam(p) :
                aAncestor->getParam(p);
}

bool GameObjectType::hasParam(const QString &p) const {return  GameObject::hasParam(p) || (aAncestor && aAncestor->hasParam(p));}
QList<QString> GameObjectType::params() const {return filter(aParams.keys());}
QList<QString> GameObjectType::properParams() const {return paramTree().last().second;}
HierarchicalAttr GameObjectType::paramTree() const{
    HierarchicalAttr p(aAncestor ? aAncestor->paramTree() : HierarchicalAttr());
    p.append(QPair<QString,QList<QString>>(typeName(), GameObject::params()));
    removeLastRedondances(p);
    return p;
}

bool GameObjectType::getFlag(const QString &f) const{return aFlags.value(f,false);}
void GameObjectType::setFlag(const QString &f, bool v) {aFlags[f] = v; touch();}
bool GameObjectType::hasFlag(const QString &f) const {return aFlags.contains(f) || (aAncestor && aAncestor->hasFlag(f));}
QList<QString> GameObjectType::flags() const {

    return (filter(GameObject::flags()));}

GameObjectType* GameObjectType::ancestor() const{return aAncestor;}
const QList<GameObjectType*>& GameObjectType::descendants() const{return descendantTypes;}


void GameObjectType::addDescendant(GameObjectType *gt){descendantTypes.append(gt); qDebug() << "type" <<  gt->name();}

void GameObjectType::removeLastRedondances(HierarchicalAttr &attr){
    if(attr.length()>1){
        auto &a = attr.last().second;
        for(auto i : attr)
            if(i != attr.last())
                a.erase(std::remove_if(a.begin(), a.end(), [&i](QString a){return i.second.contains(a);}),
                        a.end());
    }
}



