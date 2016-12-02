#include "typeitemmodel.h"


TypeTreeItem::TypeTreeItem(Game *game) :
    state(RootItem), parentItem(nullptr), type(nullptr)
{
    game->world()->cellTypes();
    TypeTreeItem *cellTypes = new TypeTreeItem("CellType", this);
    children.append(cellTypes);
    for(CellType *c : game->world()->cellTypes())
        cellTypes->children.append(new TypeTreeItem(c, cellTypes));
}

TypeTreeItem::TypeTreeItem(Type *typ, TypeTreeItem *parent) :
    parentItem(parent), state(TypeItem), type(typ)
{
    rowNb = parent->children.length();
    for(Type *t : typ->descendants())
        children.append(new TypeTreeItem(t, this));
}

TypeTreeItem::TypeTreeItem(QString typeName, TypeTreeItem *parent) :
    typeName(typeName), parentItem(parent), state(BaseItem), type(nullptr)
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

Qt::ItemFlags TypeTreeItem::flags(int col) const{
    return 0;
}

QVariant TypeTreeItem::data(int col, int role) const{
    if(role == Qt::DisplayRole){
        if(state == TypeItem) return QVariant(type->name());
        return QVariant(typeName);
    }
    return QVariant();
}












TypeItemModel::TypeItemModel(QObject *parent) :
    QAbstractItemModel(parent),
    rootItem(nullptr)
{

}


int TypeItemModel::columnCount(const QModelIndex &parent) const{
    return 1;
}

int TypeItemModel::rowCount(const QModelIndex &parent) const{
    if(!parent.isValid())
        return rootItem == nullptr ? 0 : rootItem->rowCount();
    return static_cast<TypeTreeItem*>(parent.internalPointer())->rowCount();
}

Qt::ItemFlags TypeItemModel::flags(const QModelIndex &index) const{
    return QAbstractItemModel::flags(index);
}

QVariant TypeItemModel::data(const QModelIndex &index, int role) const{
    if(role == Qt::DisplayRole) return static_cast<TypeTreeItem*>(index.internalPointer())->data(index.column(), role);
    return QVariant();
}

QModelIndex TypeItemModel::index(int row, int column, const QModelIndex &parent) const{
    if(!parent.isValid())
        return rootItem == nullptr ? QModelIndex() : createIndex(row, column, static_cast<void*>(rootItem->child(row)));
    return createIndex(row, column, static_cast<TypeTreeItem*>(parent.internalPointer())->child(row));
}

QModelIndex TypeItemModel::parent(const QModelIndex &child) const{
    TypeTreeItem *t = static_cast<TypeTreeItem*>(child.internalPointer())->parent();
    if(t->parent() != nullptr) return createIndex(t->row(),0,static_cast<TypeTreeItem*>(t));
    return QModelIndex();
}


void TypeItemModel::setGame(Game *g){
    beginResetModel();
    if(rootItem != nullptr) delete rootItem;
    if(g != nullptr)
        rootItem = new TypeTreeItem(g);
    endResetModel();
}

bool TypeItemModel::setData(const QModelIndex &index, const QVariant &value, int role){
    return false;
}

QVariant TypeItemModel::headerData(int section, Qt::Orientation orientation, int role) const{
    return QVariant();
}
