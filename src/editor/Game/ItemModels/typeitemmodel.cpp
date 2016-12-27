#include "typeitemmodel.h"


TypeTreeItem::TypeTreeItem(Game *game) :
    state(RootItem), parentItem(nullptr), type(nullptr)
{
    TypeTreeItem *cellTypes = new TypeTreeItem("CellType", this);
    children.append(cellTypes);
    for(CellType *c : game->world().cellTypes())
        cellTypes->children.append(new TypeTreeItem(c, cellTypes));
}

TypeTreeItem::TypeTreeItem(GameObjectType *typ, TypeTreeItem *parent) :
    state(TypeItem), parentItem(parent), type(typ)
{
    rowNb = parent->children.length();
    for(GameObjectType *t : typ->descendants())
        children.append(new TypeTreeItem(t, this));
}

TypeTreeItem::TypeTreeItem(QString typeName, TypeTreeItem *parent) :
    state(BaseItem), typeName(typeName), parentItem(parent), type(nullptr)
{
    rowNb = parent->children.length();
}

TypeTreeItem::~TypeTreeItem(){
    for(TypeTreeItem *t : children)
        delete t;
}


int TypeTreeItem::rowCount() const{
    return children.length();
}

int TypeTreeItem::row() const{
    return rowNb;
}

TypeTreeItem* TypeTreeItem::parent() const{
    return parentItem;
}

TypeTreeItem* TypeTreeItem::child(int row) const{
    return children[row];
}

Qt::ItemFlags TypeTreeItem::flags(int UNUSED(col)) const{
    return 0;
}

QVariant TypeTreeItem::data(int UNUSED(col), int role) const{
    if(role == Qt::DisplayRole){
        if(state == TypeItem) return QVariant(type->name());
        return QVariant(typeName);
    }
    return QVariant();
}












TypeItemModel2::TypeItemModel2(QObject *parent) :
    QAbstractItemModel(parent),
    rootItem(nullptr)
{

}


int TypeItemModel2::columnCount(const QModelIndex &UNUSED(parent)) const{
    return 1;
}

int TypeItemModel2::rowCount(const QModelIndex &parent) const{
    if(!parent.isValid())
        return rootItem == nullptr ? 0 : rootItem->rowCount();
    return static_cast<TypeTreeItem*>(parent.internalPointer())->rowCount();
}

Qt::ItemFlags TypeItemModel2::flags(const QModelIndex &index) const{
    return QAbstractItemModel::flags(index);
}

QVariant TypeItemModel2::data(const QModelIndex &index, int role) const{
    if(role == Qt::DisplayRole) return static_cast<TypeTreeItem*>(index.internalPointer())->data(index.column(), role);
    return QVariant();
}

QModelIndex TypeItemModel2::index(int row, int column, const QModelIndex &parent) const{
    if(!parent.isValid())
        return rootItem == nullptr ? QModelIndex() : createIndex(row, column, static_cast<void*>(rootItem->child(row)));
    return createIndex(row, column, static_cast<TypeTreeItem*>(parent.internalPointer())->child(row));
}

QModelIndex TypeItemModel2::parent(const QModelIndex &child) const{
    TypeTreeItem *t = static_cast<TypeTreeItem*>(child.internalPointer())->parent();
    if(t->parent() != nullptr) return createIndex(t->row(),0,static_cast<TypeTreeItem*>(t));
    return QModelIndex();
}


void TypeItemModel2::setGame(Game *g){
    beginResetModel();
    if(rootItem != nullptr) delete rootItem;
    if(g != nullptr)
        rootItem = new TypeTreeItem(g);
    endResetModel();
}

bool TypeItemModel2::setData(const QModelIndex &UNUSED(index), const QVariant &UNUSED(value), int UNUSED(role)){
    return false;
}

QVariant TypeItemModel2::headerData(int UNUSED(section), Qt::Orientation UNUSED(orientation), int UNUSED(role)) const{
    return QVariant();
}
