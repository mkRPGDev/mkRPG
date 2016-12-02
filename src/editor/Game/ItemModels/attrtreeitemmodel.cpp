#include "attrtreeitemmodel.h"


template<bool ParamItem>
GameTreeItem<ParamItem>::GameTreeItem(GameObject &obj) :
    GameTreeItem<ParamItem>(0, &obj, nullptr, Object, nullptr) {}

template<bool ParamItem>
GameTreeItem<ParamItem>::GameTreeItem(InheritableObject &typ) :
    GameTreeItem<ParamItem>(0, nullptr, &typ, Type, nullptr) {}


template<bool ParamItem>
GameTreeItem<ParamItem>::GameTreeItem(int rowNb, GameObject *obj, InheritableObject *typ, State state, GameTreeItem *parent) :
    rowNb(rowNb), parentItem(parent), obj(obj), typ(typ),
    anc(nullptr), attr(""), state(state)
{
    switch (state) {
    case Type:
        genealogy(typ);
        for(int i(0); i<ancestors.length(); ++i)
            children.append(new GameTreeItem<ParamItem>(i, obj, typ, Object, this));
        break;
    case Object:
        if(parent != nullptr)
            anc = parent->ancestors[rowNb];
        if(ParamItem)
            attrs = anc == nullptr ? obj->params() : anc->properParams();
        else
            attrs = anc == nullptr ? obj->flags() : anc->properFlags();
        for(int i(0); i<attrs.length(); ++i)
            children.append(new GameTreeItem<ParamItem>(i, obj == nullptr ? anc : obj, typ, Attribute, this));
        break;
    case Attribute:
        attr = parent->attrs[rowNb];
        if(ParamItem){
            children.append(new GameTreeItem<ParamItem>(0, obj, typ, Value, this));
            children.append(new GameTreeItem<ParamItem>(1, obj, typ, Value, this));
        }
        break;
    case Value:
        attr = parent->attr;
        break;
    default:
        assert(false);
        break;
    }
}

template<bool ParamItem>
GameTreeItem<ParamItem>::~GameTreeItem(){
    for(GameTreeItem* i : children)
        delete i;
}



template<bool ParamItem>
void GameTreeItem<ParamItem>::genealogy(InheritableObject *t){
    if(t == nullptr) return;
    genealogy(t->ancestor());
    ancestors.append(t);
}

template<bool ParamItem>
int GameTreeItem<ParamItem>::row() const{
    return rowNb;
}

template<bool ParamItem>
int GameTreeItem<ParamItem>::rowCount() const{
    return children.length();
}

template<bool ParamItem>
GameTreeItem<ParamItem>* GameTreeItem<ParamItem>::parent() const{
    return parentItem;
}

template<bool ParamItem>
GameTreeItem<ParamItem>* GameTreeItem<ParamItem>::child(int row) const{
    assert(row < rowCount());
    return children[row];
}


template<bool ParamItem>
Qt::ItemFlags GameTreeItem<ParamItem>::flags(int col) const{
    Qt::ItemFlags fl(Qt::NoItemFlags);
    if(col == 1 && (state == Attribute || state == Value)) fl |= Qt::ItemIsEditable;
    if(typ == nullptr || typ == obj || state != Value) fl |= Qt::ItemIsEnabled;
    return fl;
}

template<bool ParamItem>
QVariant GameTreeItem<ParamItem>::data(int col, int role) const{
    switch (state) {
    case Type: return typeData(col, role);
    case Object: return objectData(col, role);
    case Attribute: return parameterData(col, role);
    case Value: return valueData(col, role);
    default: return QVariant();
    }
}


template<bool ParamItem>
QVariant GameTreeItem<ParamItem>::typeData(int UNUSED(col), int UNUSED(role)) const{
    return QVariant();
}

template<bool ParamItem>
QVariant GameTreeItem<ParamItem>::objectData(int col, int role) const{
    if(col) return QVariant();
    switch (role) {
    case Qt::DisplayRole: return QVariant(anc->name());
    default: return QVariant();
    }
}

template<bool ParamItem>
QVariant GameTreeItem<ParamItem>::parameterData(int col, int role) const{
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
        case Qt::DisplayRole:
            if(ParamItem)
                return QVariant(typ ? typ->getParam(attr) : obj->getParam(attr));
            else
                return QVariant(typ ? typ->getFlag(attr) : obj->getFlag(attr));
        default: return QVariant();
        }
}

template<bool ParamItem>
QVariant GameTreeItem<ParamItem>::valueData(int col, int role) const{
    if(!ParamItem) return QVariant();
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


template<bool ParamItem>
bool GameTreeItem<ParamItem>::setData(int col, QVariant value, int role){
    switch (state) {
    case Attribute: return setParameterData(col, value, role);
    case Value: return setValueData(col, value, role);
    default: return false;
    }
}

template<bool ParamItem>
bool GameTreeItem<ParamItem>::setParameterData(int col, QVariant value, int role){
    if(!ParamItem) return false; // NOTE : temporaire
    if(col == 1){
        if(role == Qt::EditRole){
            if(typ) typ->setParam(attr, value.toInt());
            else obj->setParam(attr, value.toInt());
            return true;
        }
    }
    return false;
}


template<bool ParamItem>
bool GameTreeItem<ParamItem>::setValueData(int col, QVariant value, int role){
    if(col == 1){
        if(role == Qt::EditRole){
            if(rowNb) obj->setParamMax(attr, value.toInt());
            else obj->setParamMin(attr, value.toInt());
            return true;
        }
    }

    return false;
}











ParamTreeItemModel::ParamTreeItemModel(QObject *parent) :
    QAbstractItemModel(parent),
    obj(nullptr), item(nullptr)
{

}


int ParamTreeItemModel::rowCount(const QModelIndex &parent) const{
    if(parent.isValid())
        return static_cast<GameTreeItem<true>*>(parent.internalPointer())->rowCount();
    return item == nullptr ? 0 : item->rowCount();
}

int ParamTreeItemModel::columnCount(const QModelIndex &UNUSED(parent)) const{
    return 2;
}

Qt::ItemFlags ParamTreeItemModel::flags(const QModelIndex &index) const{
    return static_cast<GameTreeItem<true>*>(index.internalPointer())->flags(index.column());
}


QVariant ParamTreeItemModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return QVariant(section == 0 ? tr("Parameter") : tr("Value"));
    return QVariant();
}

QVariant ParamTreeItemModel::data(const QModelIndex &index, int role) const{
    return static_cast<GameTreeItem<true>*>(index.internalPointer())->data(index.column(), role);
}

QModelIndex ParamTreeItemModel::index(int row, int column, const QModelIndex &parent) const{
    if(parent.isValid())
        return createIndex(row, column, static_cast<void*>(static_cast<GameTreeItem<true>*>(parent.internalPointer())->child(row)));
    return createIndex(row, column, static_cast<void*>(item->child(row)));
}

QModelIndex ParamTreeItemModel::parent(const QModelIndex &child) const{
    if(!child.isValid()) return QModelIndex();
    GameTreeItem<true> *c = static_cast<GameTreeItem<true>*>(child.internalPointer());
    GameTreeItem<true> *p = c->parent();
    assert(p != nullptr);
    if(p->parent() != nullptr) return createIndex(p->row(), 0, p);
    return QModelIndex();
}


void ParamTreeItemModel::setObject(GameObject *o){
    beginResetModel();
    obj = o;
    type =  dynamic_cast<InheritableObject*>(o);
    if(item != nullptr)
        delete item;
    if(o != nullptr)
        item = type == nullptr ? new GameTreeItem<true>(*obj) : new GameTreeItem<true>(*type);
    endResetModel();
}


bool ParamTreeItemModel::setData(const QModelIndex &index, const QVariant &value, int role){
    return static_cast<GameTreeItem<true>*>(index.internalPointer())->setData(index.column(), value, role);
}











FlagTreeItemModel::FlagTreeItemModel(QObject *parent) :
    QAbstractItemModel(parent),
    obj(nullptr), item(nullptr)
{

}


int FlagTreeItemModel::rowCount(const QModelIndex &parent) const{
    if(parent.isValid())
        return static_cast<GameTreeItem<false>*>(parent.internalPointer())->rowCount();
    return item == nullptr ? 0 : item->rowCount();
}

int FlagTreeItemModel::columnCount(const QModelIndex &UNUSED(parent)) const{
    return 2;
}

Qt::ItemFlags FlagTreeItemModel::flags(const QModelIndex &index) const{
    return static_cast<GameTreeItem<false>*>(index.internalPointer())->flags(index.column());
}


QVariant FlagTreeItemModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return QVariant(section == 0 ? tr("Flag") : tr("Value"));
    return QVariant();
}

QVariant FlagTreeItemModel::data(const QModelIndex &index, int role) const{
    return static_cast<GameTreeItem<false>*>(index.internalPointer())->data(index.column(), role);
}

QModelIndex FlagTreeItemModel::index(int row, int column, const QModelIndex &parent) const{
    if(parent.isValid())
        return createIndex(row, column, static_cast<void*>(static_cast<GameTreeItem<false>*>(parent.internalPointer())->child(row)));
    return createIndex(row, column, static_cast<void*>(item->child(row)));
}

QModelIndex FlagTreeItemModel::parent(const QModelIndex &child) const{
    if(!child.isValid()) return QModelIndex();
    GameTreeItem<false> *c = static_cast<GameTreeItem<false>*>(child.internalPointer());
    GameTreeItem<false> *p = c->parent();
    assert(p != nullptr);
    if(p->parent() != nullptr) return createIndex(p->row(), 0, p);
    return QModelIndex();
}


void FlagTreeItemModel::setObject(GameObject *o){
    beginResetModel();
    obj = o;
    type =  dynamic_cast<InheritableObject*>(o);
    if(item != nullptr)
        delete item;
    if(o != nullptr)
        item = type == nullptr ? new GameTreeItem<false>(*obj) : new GameTreeItem<false>(*type);
    endResetModel();
}


bool FlagTreeItemModel::setData(const QModelIndex &index, const QVariant &value, int role){
    return static_cast<GameTreeItem<false>*>(index.internalPointer())->setData(index.column(), value, role);
}
