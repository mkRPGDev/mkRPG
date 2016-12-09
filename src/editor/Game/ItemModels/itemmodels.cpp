#include "itemmodels.h"




ObjectsTreeModel::ObjectsTreeModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    obj = nullptr;
}

ObjectsTreeModel::ObjectsTreeModel(GameObject *o, QObject *parent) :
    ObjectsTreeModel(parent)
{
    setGameObject(o);
}

void ObjectsTreeModel::setGameObject(GameObject *o){
    obj = o;
}

int ObjectsTreeModel::columnCount(const QModelIndex &) const{
    return 2;
}

int ObjectsTreeModel::rowCount(const QModelIndex &parent) const{
    return parent.isValid() ? static_cast<GameObject*>(parent.internalPointer())->children().length() : obj->children().length();
}

QVariant ObjectsTreeModel::data(const QModelIndex &index, int role) const{
    GameObject *obj = static_cast<GameObject*>(index.internalPointer());
    if(index.column()) return role==Qt::DisplayRole ? obj->typeName() : QVariant();
    switch (role) {
    case Qt::DisplayRole:
        return  QVariant(obj->name() + " ("  +QString::number(obj->ident()) + ")");
    case Qt::EditRole:
        return QVariant(obj->name());
    }
    return QVariant();
}

QModelIndex ObjectsTreeModel::index(int row, int column, const QModelIndex &parent) const{
    //qDebug() << "index" << row << column;
    GameObject *parentItem(parent.isValid() ? static_cast<GameObject*>(parent.internalPointer()) : obj);
    //if(!hasIndex(row, column, parent)) return QModelIndex();

    GameObject *childItem = parentItem->children().at(row);
    return childItem ? createIndex(row, column, childItem) : QModelIndex();
}

QModelIndex ObjectsTreeModel::parent(const QModelIndex &child) const{
    if(!child.isValid()) return QModelIndex();
    GameObject *c = static_cast<GameObject*>(child.internalPointer());
    GameObject *p = c->parent();
    return p ? createIndex(p->children().indexOf(c),0,p) : QModelIndex();
}

QVariant ObjectsTreeModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if(orientation == Qt::Horizontal){
        if(role == Qt::DisplayRole){
            return QVariant(section ? tr("Type") : tr("Element"));
        }
    }
    return QVariant();
}


Qt::ItemFlags ObjectsTreeModel::flags(const QModelIndex &index) const{
    return QAbstractItemModel::flags(index) | ((index.column() == 0) ? Qt::ItemIsEditable : Qt::NoItemFlags);
}

bool ObjectsTreeModel::setData(const QModelIndex &index, const QVariant &value, int role){
    GameObject *obj = static_cast<GameObject*>(index.internalPointer());
    switch (role) {
    case Qt::EditRole:
        obj->setName(value.toString());
        return true;
    default:
        break;
    }
    return false;
}
