#include "object.h"
#include "game.h"


GameObject::GameObject(Game* g, GameObject *parent) :
    parent(parent),
    game(g), id(g ? g->newIdent() : 0), nbRef(0),
    lastEdit(QDateTime::currentDateTime()),
    lastChildEdit(lastEdit)
{
    if(parent) parent->addChild(this);
}


GameObject::~GameObject(){
    if(nbRef)
        qDebug() << "Salut..." << id << "Références" << nbRef;
    for(GameObject* c : children.values()) delete c;
    if(parent) parent->removeChild(this);
}

void GameObject::init(Game *g, GameObject* p){
    if(game == nullptr){
        game = g;
        id = g->newIdent();
        parent = p;
        if(parent) parent->addChild(this);
    }
}

void GameObject::setParent(GameObject *p){
    if(parent) parent->removeChild(this);
    parent = p;
    if(parent) parent->addChild(this);
}

void GameObject::touch(){
    lastEdit = QDateTime::currentDateTime();
    if(parent) parent->childrenTouched(lastEdit);
}

void GameObject::childrenTouched(const QDateTime &d){
    lastChildEdit = d;
    if(parent) parent->childrenTouched(d);
}

void GameObject::addChild(GameObject *c){
    assert(c != nullptr);
    children[c->ident()] = c;
}

void GameObject::removeChild(GameObject *c){
    assert(c != nullptr);
    children.remove(c->ident());
}

void GameObject::addReference(){
    ++nbRef;
}

void GameObject::removeReference(){
    --nbRef;
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

