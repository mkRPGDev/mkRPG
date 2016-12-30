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

bool GameObject::hasEvent(const QString &event) const{
    return aEvents.contains(event);
}

Event& GameObject::getEvent(const QString &event){
    return aEvents[event];
}

Event& GameObject::addEvent(const QString &event){
    aEvents[event] = Event();
    return aEvents[event];
}

void GameObject::removeEvent(const QString &event){
    aEvents.remove(event);
}

QList<QString> GameObject::getEvents() const{
    return aEvents.keys();
}


bool GameObject::hasOrder(const QString &order) const{
    return aOrders.contains(order);
}

Order& GameObject::getOrder(const QString &order){
    return aOrders[order];
}

Order& GameObject::addOrder(const QString &order){
    aOrders[order] = Order();
    return aOrders[order];
}

void GameObject::removeOrder(const QString &order){
    aOrders.remove(order);
}

QList<QString> GameObject::getOrders() const{
    return aOrders.keys();
}










InheritableObject::InheritableObject(GameObject &parent, InheritableObject *ancestor) :
    GameObject(parent), aAncestor(ancestor)
{

}

bool InheritableObject::isInheritedParam(const QString &param) const {
    return aAncestor ? aAncestor->hasParam(param) : false;
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


bool InheritableObject::isInheritedEvent(const QString &event) const{
    return aAncestor ? aAncestor->hasEvent(event) : false;
}

bool InheritableObject::hasEvent(const QString &event) const{
    return GameObject::hasEvent(event) | (aAncestor && aAncestor->hasEvent(event));
}

Event& InheritableObject::getEvent(const QString &event){
    if(hasEvent(event))
        return aEvents.contains(event) ? aEvents[event] : aAncestor->getEvent(event);
    return aEvents[event];
}

QList<QString> InheritableObject::getEvents() const {
    return aAncestor ? aAncestor->getEvents() << aEvents.keys() : aEvents.keys();
}

QList<QString> InheritableObject::properEvents() const {
    return eventTree().last().second;
}

HierarchicalAttr InheritableObject::eventTree() const{
    return (aAncestor ? aAncestor->eventTree() : HierarchicalAttr()) << QPair<QString,QList<QString>>(typeName(), GameObject::getEvents());
}



bool InheritableObject::isInheritedOrder(const QString &order) const{
    return aAncestor ? aAncestor->hasOrder(order) : false;
}

bool InheritableObject::hasOrder(const QString &order) const{
    return GameObject::hasOrder(order) | (aAncestor && aAncestor->hasOrder(order));
}

Order &InheritableObject::getOrder(const QString &order){
    if(hasOrder(order))
        return aOrders.contains(order) ? aOrders[order] : aAncestor->getOrder(order);
    return aOrders[order];
}

QList<QString> InheritableObject::getOrders() const {
    return aAncestor ? aAncestor->getOrders() << aOrders.keys() : aOrders.keys();
}

QList<QString> InheritableObject::properOrders() const {
    return orderTree().last().second;
}

HierarchicalAttr InheritableObject::orderTree() const{
    return (aAncestor ? aAncestor->orderTree() : HierarchicalAttr()) << QPair<QString,QList<QString>>(typeName(), GameObject::getOrders());
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






