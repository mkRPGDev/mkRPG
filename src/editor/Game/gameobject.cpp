#include "gameobject.h"

#include "game.h"

bool cleverComp(const QString &na, const QString &nb){
    if(na.startsWith(nb)) return false;
    if(nb.startsWith(na)) return true;
    int k(0);
    for(;na[k]==nb[k];++k);
    for(;k+1 && na[k].isDigit(); --k);
    if(!++k || na[k-1] != '(' || nb[k-1] != '(') return na < nb;
    int ka(k);
    for(;ka<na.length() && na[ka].isDigit();ka++);
    int kb(k);
    for(;kb<nb.length() && nb[kb].isDigit();kb++);
    if(k==ka || k==kb || ka==na.length() || kb==nb.length() || na[ka] != ')' || nb[kb] != ')') return na < nb;
    return na.midRef(k, ka-k).toInt() < nb.midRef(k, kb-k).toInt();
}





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

void GameObject::renameParam(const QString &param, const QString &newParam){
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

void GameObject::renameFlag(const QString &flag, const QString &newFlag){
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

bool InheritableObject::isRedefiniedParam(const QString &param) const {
    return isInheritedParam(param) && GameObject::hasParam(param);
}

int InheritableObject::getParam(const QString &param) const {
    return GameObject::hasParam(param) || aAncestor==nullptr ?
                GameObject::getParam(param) :
                aAncestor->getParam(param);
}

bool InheritableObject::hasParam(const QString &param) const {
    return  GameObject::hasParam(param) || (aAncestor && aAncestor->hasParam(param));
}

void InheritableObject::setParam(const QString &param, int value){
    if(isInheritedParam(param))
        setParamDomain(param, getParamMin(param), getParamMax(param));
    GameObject::setParam(param, value);
}

int InheritableObject::getParamMax(const QString &param) const{
    return isInheritedParam(param) ? aAncestor->getParamMax(param) : GameObject::getParamMax(param);
}

int InheritableObject::getParamMin(const QString &param) const{
    return isInheritedParam(param) ? aAncestor->getParamMin(param) : GameObject::getParamMin(param);
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




bool InheritableObject::isInheritedFlag(const QString &flag) const {
    return aAncestor ? aAncestor->hasFlag(flag) : false;
}

bool InheritableObject::isRedefiniedFlag(const QString &flag) const {
    return isInheritedFlag(flag) && GameObject::hasFlag(flag);
}

bool InheritableObject::getFlag(const QString &flag) const {
    return GameObject::hasFlag(flag) || aAncestor==nullptr ?
                GameObject::getFlag(flag) :
                aAncestor->getFlag(flag);
}

bool InheritableObject::hasFlag(const QString &flag) const {
    return  GameObject::hasFlag(flag) || (aAncestor && aAncestor->hasFlag(flag));
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

bool InheritableObject::hasAncestor() const{
    return aAncestor != nullptr;
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







#include "game.h"
GameObjectType::GameObjectType(DefaultTypes &parent) :
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

const QList<GameObjectType*> &GameObjectType::descendants() const{
    return descendantTypes;
}

void GameObjectType::addDescendant(GameObjectType &type){
    descendantTypes.append(&type);
}

void GameObjectType::removeDescendant(GameObjectType &type){
    descendantTypes.removeAll(&type);
}






