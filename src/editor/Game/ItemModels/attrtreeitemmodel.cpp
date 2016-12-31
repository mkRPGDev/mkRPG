#include "attrtreeitemmodel.h"


template<ItemType type>
GameTreeItem<type>::GameTreeItem(GameObject &obj) :
    GameTreeItem<type>(0, &obj, nullptr, Object, nullptr) {}

template<ItemType type>
GameTreeItem<type>::GameTreeItem(InheritableObject &typ) :
GameTreeItem<type>(0, nullptr, &typ, Type, nullptr) {}


template<ItemType type>
GameTreeItem<type>::GameTreeItem(int rowNb, GameObject *obj, InheritableObject *typ, State state, GameTreeItem *parent) :
    rowNb(rowNb), parentItem(parent), obj(obj), typ(typ),
    anc(nullptr), attr(""), state(state), bgColor(QColor(250,250,250))
{
    switch (state) {
    case Type:
        genealogy(typ);
        for(int i(0); i<ancestors.length(); ++i)
            children.append(new GameTreeItem<type>(i, obj, typ, Object, this));
        break;
    case Object:
        if(parent != nullptr){
            anc = parent->ancestors[rowNb];
            bgColor = QColor::fromHsv(25*rowNb % 360,150, 255);
        }
        switch (type) {
        case ParamItem:
            attrs = anc == nullptr ? obj->params() : anc->properParams();
            break;
        case FlagItem:
            attrs = anc == nullptr ? obj->flags() : anc->properFlags();
            break;
        case EventItem:
            attrs = anc == nullptr ? obj->events() : anc->properEvents();
            break;
        case OrderItem:
            attrs = anc == nullptr ? obj->orders() : anc->properOrders();
            break;
        default:
            assert(false);
        }
        for(int i(0); i<attrs.length(); ++i)
            children.append(new GameTreeItem<type>(i, obj == nullptr ? anc : obj, typ, Attribute, this));
        break;
    case Attribute:
        attr = parent->attrs[rowNb];
        setAttributeRowNb(rowNb);
        switch (type) {
        case ParamItem:
            children.append(new GameTreeItem<type>(0, obj, typ, Value, this));
            children.append(new GameTreeItem<type>(1, obj, typ, Value, this));
            break;
        case EventItem:
            // TODO
            break;
        case OrderItem:
            // TODO
            break;
        default:
            break;
        }
        break;
    case Value:
        bgColor = QColor::fromHsv(parent->bgColor.hue(), 40 + 20*((1 + parent->rowNb + rowNb)%2), parent->bgColor.value());
        attr = parent->attr;
        break;
    default:
        assert(false);
        break;
    }
}

template<ItemType type>
void GameTreeItem<type>::setAttributeRowNb(int r){
    rowNb = r;
    bgColor = QColor::fromHsv(parentItem->bgColor.hue(), 40 + 20*(rowNb%2), parentItem->bgColor.value());
}

template<ItemType type>
GameTreeItem<type>::~GameTreeItem(){
    for(GameTreeItem* i : children)
        delete i;
}



template<ItemType type>
void GameTreeItem<type>::genealogy(InheritableObject *t){
    if(t == nullptr) return;
    genealogy(t->ancestor());
    ancestors.append(t);
}

template<ItemType type>
int GameTreeItem<type>::row() const{
    return rowNb;
}

template<ItemType type>
int GameTreeItem<type>::rowCount() const{
    return children.length();
}

template<ItemType type>
GameTreeItem<type>* GameTreeItem<type>::parent() const{
    return parentItem;
}

template<ItemType type>
GameTreeItem<type>* GameTreeItem<type>::child(int row) const{
    assert(row < rowCount());
    return children[row];
}


template<ItemType type>
Qt::ItemFlags GameTreeItem<type>::flags(int col) const{
    switch (state) {
    case Type: return typeFlags(col);
    case Object: return objectFlags(col);
    case Attribute: return attrFlags(col);
    case Value: return valueFlags(col);
    default: return Qt::NoItemFlags;
    }
}
template<ItemType type>
Qt::ItemFlags GameTreeItem<type>::typeFlags(int) const{
    return Qt::NoItemFlags;
}
template<ItemType type>
Qt::ItemFlags GameTreeItem<type>::objectFlags(int) const{
    return Qt::NoItemFlags;
}
template<ItemType type>
Qt::ItemFlags GameTreeItem<type>::attrFlags(int col) const{
    Qt::ItemFlags fl(Qt::ItemIsEnabled);
    if(col == 0 && (typ == nullptr || typ == obj)) fl |= Qt::ItemIsEditable;
    if(col == 1) fl |= Qt::ItemIsEditable;
    return fl;
}
template<ItemType type>
Qt::ItemFlags GameTreeItem<type>::valueFlags(int col) const{
    Qt::ItemFlags fl(Qt::NoItemFlags);
    if(typ == nullptr || typ == obj){
        fl |= Qt::ItemIsEnabled;
        if(col == 1)
            fl |= Qt::ItemIsEditable;
    }
    return fl;
}


template<ItemType type>
QVariant GameTreeItem<type>::data(int col, int role) const{
    switch (state) {
    case Type: return typeData(col, role);
    case Object: return objectData(col, role);
    case Attribute: return attrData(col, role);
    case Value: return valueData(col, role);
    default: return QVariant();
    }
}


template<ItemType type>
QVariant GameTreeItem<type>::typeData(int UNUSED(col), int UNUSED(role)) const{
    return QVariant();
}

template<ItemType type>
QVariant GameTreeItem<type>::objectData(int col, int role) const{
    if(col) return QVariant();
    switch (role) {
    case Qt::DisplayRole: return QVariant(anc->name());
    case Qt::BackgroundRole: return QVariant(QBrush(QColor(100,100,100)));
    case Qt::ForegroundRole: return QVariant(QBrush(bgColor));
    case Qt::FontRole:{
        QFont f;
        f.setItalic(true);
        return QVariant(f);
    }
    default: return QVariant();
    }
}
template<ItemType type>
QVariant GameTreeItem<type>::attrData(int col, int role) const{
    if(role == Qt::BackgroundRole) return typ ? QVariant(QBrush(bgColor)) : QVariant();
    if(col == 0)
        switch (role) {
        case Qt::EditRole:
        case Qt::DisplayRole: return QVariant(attr);
        case Qt::FontRole:
            if(typ != nullptr && ((type==ParamItem && typ->isRedefiniedParam(attr)) ||
                                  (type==FlagItem && typ->isRedefiniedFlag(attr)))){
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
            switch (type) {
            case ParamItem: return QVariant(typ ? typ->getParam(attr) : obj->getParam(attr));
            case FlagItem: return QVariant(typ ? typ->getFlag(attr) : obj->getFlag(attr));
            case EventItem: return QVariant(typ ? typ->getEvent(attr).typeName() : obj->getEvent(attr).typeName());
            case OrderItem: return QVariant(typ ? typ->getOrder(attr).typeName() : obj->getOrder(attr).typeName());
            default:assert(false);
            }
        case Qt::UserRole:
            if(!typ) return false;
            switch (type) {
            case ParamItem: return QVariant(typ ? typ->isRedefiniedParam(attr) : false);
            case FlagItem: return QVariant(typ ? typ->isRedefiniedFlag(attr) : false);
            default:return false;
            }
        case Qt::SizeHintRole:
            return type == ParamItem ? QVariant(QPoint(typ ? typ->getParamMin(attr) : obj->getParamMin(attr),
                                                       typ ? typ->getParamMax(attr) : obj->getParamMax(attr))) : QVariant();
        default: return QVariant();
        }
}
template<ItemType type>
QVariant GameTreeItem<type>::valueData(int col, int role) const{
    if(role == Qt::BackgroundRole && parentItem->typ != nullptr) return QVariant(QBrush(bgColor));
    if(role == Qt::UserRole) return QVariant(false);
    switch (type) {
    case ParamItem:
        if(col == 0)
            switch (role) {
            case Qt::DisplayRole: return QVariant(rowNb ? "Maximum" : "Minimum");
            default: return QVariant();
            }
        else
            switch (role) {
            case Qt::EditRole:
            case Qt::DisplayRole: return QVariant(rowNb ? obj->getParamMax(attr) : obj->getParamMin(attr));
            case Qt::SizeHintRole: return QVariant(QPoint(-10000,10000));
            default: return QVariant();
            }
    case EventItem:
        // TODO
    case OrderItem:
        // TODO
    default:
        return QVariant();
    }
}


template<ItemType type>
bool GameTreeItem<type>::setData(int col, QVariant value, int role){
    switch (state) {
    case Attribute: return setAttrData(col, value, role);
    case Value: return setValueData(col, value, role);
    default: return false;
    }
}
template<ItemType type>
bool GameTreeItem<type>::setAttrData(int col, QVariant value, int role){
    if(col == 1){
        if(role == Qt::EditRole){
            switch (type) {
            case ParamItem:
                if(typ){
                    if(typ->getParam(attr) != value.toInt())
                        typ->setParam(attr, value.toInt());
                }
                else if(obj->getParam(attr) != value.toInt())
                    obj->setParam(attr, value.toInt());
                break;
            case FlagItem:
                if(typ){
                    if(typ->getFlag(attr) != value.toBool())
                        typ->setFlag(attr, value.toBool());
                }
                else if(obj->getFlag(attr) != value.toBool())
                    obj->setFlag(attr, value.toBool());
                break;
            default:
                return false;
            }
            return true;
        }
        if(role == Qt::UserRole && typ != nullptr && typ->hasAncestor()){
            switch (type) {
            case ParamItem:
                typ->removeParam(attr);
                break;
            case FlagItem:
                typ->removeFlag(attr);
                break;
            default:
                return false;
            }
            return true;
        }
    }
    else if(col == 0){
        if(role == Qt::EditRole && value.toString() != attr && isValidName(value.toString())){
            switch (type) {
            case ParamItem:
                obj->renameParam(attr, value.toString());
                break;
            case FlagItem:
                obj->renameFlag(attr, value.toString());
                break;
            case EventItem:
                // TODO
            case OrderItem:
                // TODO
            default:
                break;
            }
            attr = value.toString();
            return true;
        }
    }
    return false;
}
template<ItemType type>
bool GameTreeItem<type>::setValueData(int col, QVariant value, int role){
    if(col == 1){
        if(role == Qt::EditRole){
            if(rowNb) obj->setParamMax(attr, value.toInt());
            else obj->setParamMin(attr, value.toInt());
            return true;
        }
    }

    return false;
}

template<ItemType type>
void GameTreeItem<type>::addAttr(const QString &attr){
    switch (type) {
    case ParamItem:
        typ == nullptr ? obj->addParam(attr) : typ->addParam(attr);
        break;
    case FlagItem:
        typ == nullptr ? obj->addFlag(attr) : typ->addFlag(attr);
        break;
    case EventItem:
        typ == nullptr ? obj->addEvent(attr) : typ->addEvent(attr);
        break;
    case OrderItem:
        typ == nullptr ? obj->addOrder(attr) : typ->addOrder(attr);
        break;
    default:
        break;
    }
    attrs.append(attr);
    children.append(new GameTreeItem<type>(attrs.length()-1,typ == nullptr ? obj : typ, typ, Attribute, this));
}

template<ItemType type>
void GameTreeItem<type>::sort(){
    if(state != Object) return;
    std::sort(children.begin(), children.end(),
              [](const GameTreeItem<type>* it1,
              const GameTreeItem<type>* it2){
        return cleverComp(it1->attr, it2->attr);
    });
    attrs.clear();
    int k(0);
    for(GameTreeItem *&i : children){
        i->setAttributeRowNb(k++);
        attrs.append(i->attr);
    }
}









ParamTreeItemModel::ParamTreeItemModel(QObject *parent) :
    QAbstractItemModel(parent),
    obj(nullptr), item(nullptr)
{}


int ParamTreeItemModel::rowCount(const QModelIndex &parent) const{
    if(parent.isValid())
        return static_cast<GameTreeItem<ParamItem>*>(parent.internalPointer())->rowCount();
    return item == nullptr ? 0 : item->rowCount();
}

int ParamTreeItemModel::columnCount(const QModelIndex &UNUSED(parent)) const{
    return 2;
}

Qt::ItemFlags ParamTreeItemModel::flags(const QModelIndex &index) const{
    return static_cast<GameTreeItem<ParamItem>*>(index.internalPointer())->flags(index.column());
}


QVariant ParamTreeItemModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return QVariant(section == 0 ? tr("Event") : tr("Type"));
    return QVariant();
}

QVariant ParamTreeItemModel::data(const QModelIndex &index, int role) const{
    return static_cast<GameTreeItem<ParamItem>*>(index.internalPointer())->data(index.column(), role);
}

QModelIndex ParamTreeItemModel::index(int row, int column, const QModelIndex &parent) const{
    if(parent.isValid())
        return createIndex(row, column, static_cast<void*>(static_cast<GameTreeItem<ParamItem>*>(parent.internalPointer())->child(row)));
    return createIndex(row, column, static_cast<void*>(item->child(row)));
}

QModelIndex ParamTreeItemModel::parent(const QModelIndex &child) const{
    if(!child.isValid()) return QModelIndex();
    GameTreeItem<ParamItem> *c = static_cast<GameTreeItem<ParamItem>*>(child.internalPointer());
    GameTreeItem<ParamItem> *p = c->parent();
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
        item = type == nullptr ? new GameTreeItem<ParamItem>(*obj) : new GameTreeItem<ParamItem>(*type);
    else item = nullptr;
    endResetModel();
}


bool ParamTreeItemModel::setData(const QModelIndex &index, const QVariant &value, int role){
    if(!static_cast<GameTreeItem<ParamItem>*>(index.internalPointer())->setData(index.column(), value, role))
        return false;
    emit dataChanged(index.parent().child(index.row(),0),index);
    if(index.column() == 0)
        sortAttr(index.parent());
    return true;
}

void ParamTreeItemModel::addParam(const QString &name){
    if(obj != nullptr){
        QModelIndex ins = type == nullptr ? QModelIndex() : index(rowCount(QModelIndex())-1, 0, QModelIndex());
        int l = rowCount(ins);
        emit beginInsertRows(ins, l, l);
        GameTreeItem<ParamItem> *it = ins.isValid() ? static_cast<GameTreeItem<ParamItem>*>(ins.internalPointer())
                                               : item;
        it->addAttr(name);
        emit endInsertRows();
        sortAttr(ins);
    }
}

void ParamTreeItemModel::sortAttr(const QModelIndex &par){
    emit layoutAboutToBeChanged();
    if(par.isValid())
        static_cast<GameTreeItem<ParamItem>*>(par.internalPointer())->sort();
    else
        item->sort();
    emit layoutChanged();
}









FlagTreeItemModel::FlagTreeItemModel(QObject *parent) :
    QAbstractItemModel(parent),
    obj(nullptr), item(nullptr)
{}


int FlagTreeItemModel::rowCount(const QModelIndex &parent) const{
    if(parent.isValid())
        return static_cast<GameTreeItem<FlagItem>*>(parent.internalPointer())->rowCount();
    return item == nullptr ? 0 : item->rowCount();
}

int FlagTreeItemModel::columnCount(const QModelIndex &UNUSED(parent)) const{
    return 2;
}

Qt::ItemFlags FlagTreeItemModel::flags(const QModelIndex &index) const{
    return static_cast<GameTreeItem<FlagItem>*>(index.internalPointer())->flags(index.column());
}


QVariant FlagTreeItemModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return QVariant(section == 0 ? tr("Flag") : tr("Type"));
    return QVariant();
}

QVariant FlagTreeItemModel::data(const QModelIndex &index, int role) const{
    return static_cast<GameTreeItem<FlagItem>*>(index.internalPointer())->data(index.column(), role);
}

QModelIndex FlagTreeItemModel::index(int row, int column, const QModelIndex &parent) const{
    if(parent.isValid())
        return createIndex(row, column, static_cast<void*>(static_cast<GameTreeItem<FlagItem>*>(parent.internalPointer())->child(row)));
    return createIndex(row, column, static_cast<void*>(item->child(row)));
}

QModelIndex FlagTreeItemModel::parent(const QModelIndex &child) const{
    if(!child.isValid()) return QModelIndex();
    GameTreeItem<FlagItem> *c = static_cast<GameTreeItem<FlagItem>*>(child.internalPointer());
    GameTreeItem<FlagItem> *p = c->parent();
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
        item = type == nullptr ? new GameTreeItem<FlagItem>(*obj) : new GameTreeItem<FlagItem>(*type);
    else item = nullptr;
    endResetModel();
}


bool FlagTreeItemModel::setData(const QModelIndex &index, const QVariant &value, int role){
    if(!static_cast<GameTreeItem<FlagItem>*>(index.internalPointer())->setData(index.column(), value, role))
        return false;
    emit dataChanged(index.parent(),index);
    if(index.column() == 0)
        sortAttr(index.parent());
    return true;
}

void FlagTreeItemModel::addFlag(const QString &name){
    if(obj != nullptr){
        QModelIndex ins = type == nullptr ? QModelIndex() : index(rowCount(QModelIndex())-1, 0, QModelIndex());
        int l = rowCount(ins);
        emit beginInsertRows(ins, l, l);
        GameTreeItem<FlagItem> *it = ins.isValid() ? static_cast<GameTreeItem<FlagItem>*>(ins.internalPointer())
                                                : item;
        it->addAttr(name);
        emit endInsertRows();
        sortAttr(ins);
    }
}

void FlagTreeItemModel::sortAttr(const QModelIndex &par){
    emit layoutAboutToBeChanged();
    if(par.isValid())
        static_cast<GameTreeItem<FlagItem>*>(par.internalPointer())->sort();
    else
        item->sort();
    emit layoutChanged();
}









EventTreeItemModel::EventTreeItemModel(QObject *parent) :
    QAbstractItemModel(parent),
    obj(nullptr), item(nullptr)
{}


int EventTreeItemModel::rowCount(const QModelIndex &parent) const{
    if(parent.isValid())
        return static_cast<GameTreeItem<EventItem>*>(parent.internalPointer())->rowCount();
    return item == nullptr ? 0 : item->rowCount();
}

int EventTreeItemModel::columnCount(const QModelIndex &UNUSED(parent)) const{
    return 2;
}

Qt::ItemFlags EventTreeItemModel::flags(const QModelIndex &index) const{
    return static_cast<GameTreeItem<EventItem>*>(index.internalPointer())->flags(index.column());
}


QVariant EventTreeItemModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return QVariant(section == 0 ? tr("Event") : tr("Type"));
    return QVariant();
}

QVariant EventTreeItemModel::data(const QModelIndex &index, int role) const{
    return static_cast<GameTreeItem<EventItem>*>(index.internalPointer())->data(index.column(), role);
}

QModelIndex EventTreeItemModel::index(int row, int column, const QModelIndex &parent) const{
    if(parent.isValid())
        return createIndex(row, column, static_cast<void*>(static_cast<GameTreeItem<EventItem>*>(parent.internalPointer())->child(row)));
    return createIndex(row, column, static_cast<void*>(item->child(row)));
}

QModelIndex EventTreeItemModel::parent(const QModelIndex &child) const{
    if(!child.isValid()) return QModelIndex();
    GameTreeItem<EventItem> *c = static_cast<GameTreeItem<EventItem>*>(child.internalPointer());
    GameTreeItem<EventItem> *p = c->parent();
    assert(p != nullptr);
    if(p->parent() != nullptr) return createIndex(p->row(), 0, p);
    return QModelIndex();
}


void EventTreeItemModel::setObject(GameObject *o){
    beginResetModel();
    obj = o;
    type =  dynamic_cast<InheritableObject*>(o);
    if(item != nullptr)
        delete item;
    if(o != nullptr)
        item = type == nullptr ? new GameTreeItem<EventItem>(*obj) : new GameTreeItem<EventItem>(*type);
    else item = nullptr;
    endResetModel();
}


bool EventTreeItemModel::setData(const QModelIndex &index, const QVariant &value, int role){
    if(!static_cast<GameTreeItem<EventItem>*>(index.internalPointer())->setData(index.column(), value, role))
        return false;
    emit dataChanged(index.parent(),index);
    if(index.column() == 0)
        sortAttr(index.parent());
    return true;
}

void EventTreeItemModel::addEvent(const QString &name){
    if(obj != nullptr){
        QModelIndex ins = type == nullptr ? QModelIndex() : index(rowCount(QModelIndex())-1, 0, QModelIndex());
        int l = rowCount(ins);
        emit beginInsertRows(ins, l, l);
        GameTreeItem<EventItem> *it = ins.isValid() ? static_cast<GameTreeItem<EventItem>*>(ins.internalPointer())
                                                : item;
        it->addAttr(name);
        emit endInsertRows();
        sortAttr(ins);
    }
}

void EventTreeItemModel::sortAttr(const QModelIndex &par){
    emit layoutAboutToBeChanged();
    if(par.isValid())
        static_cast<GameTreeItem<EventItem>*>(par.internalPointer())->sort();
    else
        item->sort();
    emit layoutChanged();
}






OrderTreeItemModel::OrderTreeItemModel(QObject *parent) :
    QAbstractItemModel(parent),
    obj(nullptr), item(nullptr)
{}


int OrderTreeItemModel::rowCount(const QModelIndex &parent) const{
    if(parent.isValid())
        return static_cast<GameTreeItem<OrderItem>*>(parent.internalPointer())->rowCount();
    return item == nullptr ? 0 : item->rowCount();
}

int OrderTreeItemModel::columnCount(const QModelIndex &UNUSED(parent)) const{
    return 2;
}

Qt::ItemFlags OrderTreeItemModel::flags(const QModelIndex &index) const{
    return static_cast<GameTreeItem<OrderItem>*>(index.internalPointer())->flags(index.column());
}


QVariant OrderTreeItemModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return QVariant(section == 0 ? tr("Order") : tr("Type"));
    return QVariant();
}

QVariant OrderTreeItemModel::data(const QModelIndex &index, int role) const{
    return static_cast<GameTreeItem<OrderItem>*>(index.internalPointer())->data(index.column(), role);
}

QModelIndex OrderTreeItemModel::index(int row, int column, const QModelIndex &parent) const{
    if(parent.isValid())
        return createIndex(row, column, static_cast<void*>(static_cast<GameTreeItem<OrderItem>*>(parent.internalPointer())->child(row)));
    return createIndex(row, column, static_cast<void*>(item->child(row)));
}

QModelIndex OrderTreeItemModel::parent(const QModelIndex &child) const{
    if(!child.isValid()) return QModelIndex();
    GameTreeItem<OrderItem> *c = static_cast<GameTreeItem<OrderItem>*>(child.internalPointer());
    GameTreeItem<OrderItem> *p = c->parent();
    assert(p != nullptr);
    if(p->parent() != nullptr) return createIndex(p->row(), 0, p);
    return QModelIndex();
}


void OrderTreeItemModel::setObject(GameObject *o){
    beginResetModel();
    obj = o;
    type =  dynamic_cast<InheritableObject*>(o);
    if(item != nullptr)
        delete item;
    if(o != nullptr)
        item = type == nullptr ? new GameTreeItem<OrderItem>(*obj) : new GameTreeItem<OrderItem>(*type);
    else item = nullptr;
    endResetModel();
}


bool OrderTreeItemModel::setData(const QModelIndex &index, const QVariant &value, int role){
    if(!static_cast<GameTreeItem<OrderItem>*>(index.internalPointer())->setData(index.column(), value, role))
        return false;
    emit dataChanged(index.parent(),index);
    if(index.column() == 0)
        sortAttr(index.parent());
    return true;
}

void OrderTreeItemModel::addOrder(const QString &name){
    if(obj != nullptr){
        QModelIndex ins = type == nullptr ? QModelIndex() : index(rowCount(QModelIndex())-1, 0, QModelIndex());
        int l = rowCount(ins);
        emit beginInsertRows(ins, l, l);
        GameTreeItem<OrderItem> *it = ins.isValid() ? static_cast<GameTreeItem<OrderItem>*>(ins.internalPointer())
                                                : item;
        it->addAttr(name);
        emit endInsertRows();
        sortAttr(ins);
    }
}

void OrderTreeItemModel::sortAttr(const QModelIndex &par){
    emit layoutAboutToBeChanged();
    if(par.isValid())
        static_cast<GameTreeItem<OrderItem>*>(par.internalPointer())->sort();
    else
        item->sort();
    emit layoutChanged();
}
