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

GameObject::GameObject(GameObject &parent) :
    GameObject(parent.game, &parent)
{
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







int GameObject::getParamMin(const QString &param) const {
    return aParams.value(param).minimum();
}

int GameObject::getParamMax(const QString &param) const {
    return aParams.value(param).maximum();
}

void GameObject::setParamMin(const QString &param, int min) {
    if(hasParam(param)){
        aParams[param].setMinimum(min);
        touch();
    }
}

void GameObject::setParamMax(const QString &param, int max) {
    if(hasParam(param)){
        aParams[param].setMaximum(max);
        touch();
    }
}

void GameObject::setParamDomain(const QString &param, int min, int max) {
    if(hasParam(param)){
        aParams[param].setDomain(min,max);
        touch();
    }
}

int GameObject::getParam(const QString &param) const {
    return aParams.value(param).value();
}

void GameObject::setParam(const QString &param, int value) {
    // This can create a new parameter !
    aParams[param].setValue(value); touch();
}

bool GameObject::hasParam(const QString &param) const {
    return  aParams.contains(param);
}

QList<QString> GameObject::params() const {
    return filter(aParams.keys());
}

void GameObject::renameParam(const QString &param, QString &newParam){
    aParams[newParam] = aParams.value(param);
    removeParam(param);
}

void GameObject::addParam(const QString &param, int value, int min, int max){
    removeParam(param);
    aParams[param] = Parameter(min, max, value);
}

void GameObject::removeParam(const QString &param){
    aParams.remove(param);
}

bool GameObject::getFlag(const QString &flag) const{
    return aFlags.value(flag,false);
}

void GameObject::setFlag(const QString &flag, bool enable) {
    aFlags[flag] = enable;
    touch();
}

bool GameObject::hasFlag(const QString &flag) const {
    return aFlags.contains(flag);
}

QList<QString> GameObject::flags() const {
    return filter(aFlags.keys());
}

void GameObject::renameFlag(const QString &flag, QString &newFlag){
    aFlags[newFlag] = aFlags.value(flag);
    removeFlag(flag);
}

void GameObject::addFlag(const QString &flag, bool enable){
    removeFlag(flag);
    aFlags[flag] = enable;
}

void GameObject::removeFlag(const QString &flag){
    aFlags.remove(flag);
}







InheritableObject::InheritableObject(GameObject &parent, InheritableObject *ancestor) :
    GameObject(parent), aAncestor(ancestor)
{

}

bool InheritableObject::isInheritedParam(const QString &p) const {
    return aAncestor ? aAncestor->hasParam(p) : false;
}

bool InheritableObject::isRedefiniedParam(const QString &p) const {
    return isInheritedParam(p) && GameObject::hasParam(p);
}

int InheritableObject::getParam(const QString &p) const {
    return GameObject::hasParam(p) || aAncestor==nullptr ?
                GameObject::getParam(p) :
                aAncestor->getParam(p);
}

bool InheritableObject::hasParam(const QString &p) const {
    return  GameObject::hasParam(p) || (aAncestor && aAncestor->hasParam(p));
}

QList<QString> InheritableObject::params() const {
    return filter(aParams.keys()); // NOTE Vraiment ?
}

QList<QString> InheritableObject::properParams() const {
    return paramTree().last().second;
}

HierarchicalAttr InheritableObject::paramTree() const{
    HierarchicalAttr p(aAncestor ? aAncestor->paramTree() : HierarchicalAttr());
    p.append(QPair<QString,QList<QString>>(typeName(), GameObject::params()));
    removeLastRedondances(p);
    return p;
}




bool InheritableObject::isInheritedFlag(const QString &p) const {
    return aAncestor ? aAncestor->hasFlag(p) : false;
}

bool InheritableObject::isRedefiniedFlag(const QString &p) const {
    return isInheritedFlag(p) && GameObject::hasFlag(p);
}

bool InheritableObject::getFlag(const QString &p) const {
    return GameObject::hasFlag(p) || aAncestor==nullptr ?
                GameObject::getFlag(p) :
                aAncestor->getFlag(p);
}

bool InheritableObject::hasFlag(const QString &p) const {
    return  GameObject::hasFlag(p) || (aAncestor && aAncestor->hasFlag(p));
}

QList<QString> InheritableObject::flags() const {
    return filter(aFlags.keys()); // NOTE Vraiment ?
}

QList<QString> InheritableObject::properFlags() const {
    return flagTree().last().second;
}

HierarchicalAttr InheritableObject::flagTree() const{
    HierarchicalAttr f(aAncestor ? aAncestor->flagTree() : HierarchicalAttr());
    f.append(QPair<QString,QList<QString>>(typeName(), GameObject::flags()));
    removeLastRedondances(f);
    return f;
}

InheritableObject *InheritableObject::ancestor() const{
    return aAncestor;
}

void InheritableObject::removeLastRedondances(HierarchicalAttr &attr){
    if(attr.length()>1){
        auto &a = attr.last().second;
        for(auto i : attr)
            if(i != attr.last())
                a.erase(std::remove_if(a.begin(), a.end(), [&i](QString a){return i.second.contains(a);}),
                        a.end());
    }
}








GameObjectType::GameObjectType(GameObject &parent) :
    InheritableObject(parent), ancestorType(nullptr)
{
}

GameObjectType::GameObjectType(GameObjectType &ancestor) :
    InheritableObject(ancestor, &ancestor), ancestorType(this)
{
    ancestor.addDescendant(*this);
}

GameObjectType::~GameObjectType(){
    if(ancestorType != nullptr)
        ancestorType->removeDescendant(*this);
}

const QList<GameObjectType*> GameObjectType::descendants() const{
    return descendantTypes;
}

void GameObjectType::addDescendant(GameObjectType &type){
    descendantTypes.append(&type);
}

void GameObjectType::removeDescendant(GameObjectType &type){
    descendantTypes.removeAll(&type);
}






