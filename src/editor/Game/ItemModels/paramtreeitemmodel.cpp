#include "paramtreeitemmodel.h"



GameTreeItem::GameTreeItem(GameObject &obj) :
    GameTreeItem(0, &obj, nullptr, Object, nullptr) {}

GameTreeItem::GameTreeItem(GameObjectType &typ) :
    GameTreeItem(0, nullptr, &typ, Type, nullptr) {}

GameTreeItem::GameTreeItem(int rowNb, GameObject *obj, GameObjectType *typ, State state, GameTreeItem *parent) :
    rowNb(rowNb), parentItem(parent), obj(obj), typ(typ),
    anc(nullptr), attr(""), state(state)
{
    switch (state) {
    case Type:
        genealogy(typ);
        for(int i(0); i<ancestors.length(); ++i)
            children.append(new GameTreeItem(i, obj, typ, Object, this));
        break;
    case Object:
        if(parent != nullptr)
            anc = parent->ancestors[rowNb];
        attrs = anc == nullptr ? obj->params() : anc->properParams();
        for(int i(0); i<attrs.length(); ++i)
            children.append(new GameTreeItem(i, obj == nullptr ? anc : obj, typ, Parameter, this));
        break;
    case Parameter:
        attr = parent->attrs[rowNb];
        children.append(new GameTreeItem(0, obj, typ, Value, this));
        children.append(new GameTreeItem(1, obj, typ, Value, this));
        break;
    case Value:
        attr = parent->attr;
        break;
    default:
        assert(false);
        break;
    }
}

GameTreeItem::~GameTreeItem(){
    for(GameTreeItem* i : children)
        delete i;
}



void GameTreeItem::genealogy(GameObjectType *t){
    if(t == nullptr) return;
    genealogy(t->ancestor());
    ancestors.append(t);
}

int GameTreeItem::row() const{
    return rowNb;
}

int GameTreeItem::rowCount() const{
    return children.length();
}

GameTreeItem* GameTreeItem::parent() const{
    return parentItem;
}

GameTreeItem* GameTreeItem::child(int row) const{
    assert(row < rowCount());
    return children[row];
}


Qt::ItemFlags GameTreeItem::flags(int col) const{
    Qt::ItemFlags fl(Qt::NoItemFlags);
    if(col == 1 && (state == Parameter || state == Value)) fl |= Qt::ItemIsEditable;
    if(typ == nullptr || typ == obj || state != Value) fl |= Qt::ItemIsEnabled;
    return fl;
}

QVariant GameTreeItem::data(int col, int role) const{
    switch (state) {
    case Type: return typeData(col, role);
    case Object: return objectData(col, role);
    case Parameter: return parameterData(col, role);
    case Value: return valueData(col, role);
    default: return QVariant();
    }
}


QVariant GameTreeItem::typeData(int col, int role) const{
    return QVariant();
}

QVariant GameTreeItem::objectData(int col, int role) const{
    if(col) return QVariant();
    switch (role) {
    case Qt::DisplayRole: return QVariant(anc->name());
    default: return QVariant();
    }
}

QVariant GameTreeItem::parameterData(int col, int role) const{
    if(col == 0)
        switch (role) {
        case Qt::DisplayRole: return QVariant(attr);
        case Qt::FontRole:
            if(typ != nullptr && typ->isRedefiniedParam(attr)){
                QFont f;
                f.setBold(true);
                return QVariant(f);
            }
            else return QVariant();
        default: return QVariant();
        }
    else
        switch (role) {
        case Qt::DisplayRole: return QVariant(typ ? typ->getParam(attr) : obj->getParam(attr));
        default: return QVariant();
        }
}

QVariant GameTreeItem::valueData(int col, int role) const{
    if(col == 0)
        switch (role) {
        case Qt::DisplayRole: return QVariant(rowNb ? "Maximum" : "Minimum");
        default: return QVariant();
        }
    else
        switch (role) {
        case Qt::DisplayRole: return QVariant(rowNb ? obj->getParamMax(attr) : obj->getParamMin(attr));
        default: return QVariant();
        }
}


bool GameTreeItem::setData(int col, QVariant value, int role){
    switch (state) {
    case Parameter: return setParameterData(col, value, role);
    case Value: return setValueData(col, value, role);
    default: return false;
    }
}

bool GameTreeItem::setParameterData(int col, QVariant value, int role){
    if(col == 1){
        if(role == Qt::EditRole){
            if(typ) typ->setParam(attr, value.toInt());
            else obj->setParam(attr, value.toInt());
            return true;
        }
    }
    return false;
}


bool GameTreeItem::setValueData(int col, QVariant value, int role){
    if(col == 1){
        if(role == Qt::EditRole){
            if(rowNb) obj->setParamMax(attr, value.toInt());
            else obj->setParamMin(attr, value.toInt());
            return true;
        }
    }
}











ParamTreeItemModel::ParamTreeItemModel(QObject *parent) :
    QAbstractItemModel(parent),
    obj(nullptr), item(nullptr)
{

}


int ParamTreeItemModel::rowCount(const QModelIndex &parent) const{
    if(parent.isValid())
        return static_cast<GameTreeItem*>(parent.internalPointer())->rowCount();
    return item == nullptr ? 0 : item->rowCount();
}

int ParamTreeItemModel::columnCount(const QModelIndex &parent) const{
    return 2;
}

Qt::ItemFlags ParamTreeItemModel::flags(const QModelIndex &index) const{
    return static_cast<GameTreeItem*>(index.internalPointer())->flags(index.column());
}


QVariant ParamTreeItemModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return QVariant(section == 0 ? tr("Parameter") : tr("Value"));
    return QVariant();
}

QVariant ParamTreeItemModel::data(const QModelIndex &index, int role) const{
    return static_cast<GameTreeItem*>(index.internalPointer())->data(index.column(), role);
}

QModelIndex ParamTreeItemModel::index(int row, int column, const QModelIndex &parent) const{
    if(parent.isValid())
        return createIndex(row, column, static_cast<void*>(static_cast<GameTreeItem*>(parent.internalPointer())->child(row)));
    return createIndex(row, column, static_cast<void*>(item->child(row)));
}

QModelIndex ParamTreeItemModel::parent(const QModelIndex &child) const{
    if(!child.isValid()) return QModelIndex();
    GameTreeItem *c = static_cast<GameTreeItem*>(child.internalPointer());
    GameTreeItem *p = c->parent();
    assert(p != nullptr);
    if(p->parent() != nullptr) return createIndex(p->row(), 0, p);
    return QModelIndex();
}


void ParamTreeItemModel::setObject(GameObject *o){
    beginResetModel();
    obj = o;
    type =  dynamic_cast<GameObjectType*>(o);
    if(item != nullptr)
        delete item;
    if(o != nullptr)
        item = type == nullptr ? new GameTreeItem(*obj) : new GameTreeItem(*type);
    endResetModel();
}


bool ParamTreeItemModel::setData(const QModelIndex &index, const QVariant &value, int role){
    return static_cast<GameTreeItem*>(index.internalPointer())->setData(index.column(), value, role);
}
