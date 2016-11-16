#include "object.h"
#include "game.h"


GameObject::GameObject(Game* g, GameObject *parent) :
    aParent(parent),
    game(g), id(g ? g->newIdent() : 0), nbRef(0),
    lastEdit(QDateTime::currentDateTime()),
    lastChildEdit(lastEdit)
{
    if(parent) parent->addChild(this);
    setName("Unnamed");
}


GameObject::~GameObject(){
    if(nbRef)
        qDebug() << "Salut..." << id << "Références" << nbRef;
    for(GameObject* c : aChildren.values()) delete c;
    if(aParent) aParent->removeChild(this);
}

void GameObject::init(Game *g, GameObject* p){
    if(game == nullptr){
        game = g;
        id = g->newIdent();
        aParent = p;
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






Object::Object(Game *g, GameObject *parent) :
    GameObject(g,parent)
{
    SetFlag(visible, true);
}



Image::Image(Game *g, GameObject *parent, const QString &fileName) :
    GameObject(g, parent)
{
    im.load(fileName);
}

