#include "game.h"




World::World(Game *g, GameObject *parent) :
    GameObject(g, parent)
{
}





Game::Game() :
    GameObject(),
    idDisp(0), w(World(this, this)), map(nullptr)
{
    init(this,nullptr);
}

void Game::addImage(Image *im){
    picts[im->ident()] = im;
}

void Game::setCurrentMap(Map *m){
    map = m;
}






ObjectsTreeModel::ObjectsTreeModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    game = nullptr;
}

ObjectsTreeModel::ObjectsTreeModel(Game *g, QObject *parent) :
    ObjectsTreeModel(parent)
{
    setGame(g);
}

void ObjectsTreeModel::setGame(Game *g){
    game = g;
}

int ObjectsTreeModel::columnCount(const QModelIndex &parent) const{
    return 1;
}

int ObjectsTreeModel::rowCount(const QModelIndex &parent) const{
    return parent.isValid() ? static_cast<GameObject*>(parent.internalPointer())->children().length() : game->children().length();
}

QVariant ObjectsTreeModel::data(const QModelIndex &index, int role) const{
    GameObject *obj = static_cast<GameObject*>(index.internalPointer());
    return role==Qt::DisplayRole ? QVariant(obj->name() + " ("  +QString::number(obj->ident()) + ")") : QVariant();
}

QModelIndex ObjectsTreeModel::index(int row, int column, const QModelIndex &parent) const{
    //qDebug() << "index" << row << column;
    GameObject *parentItem(parent.isValid() ? static_cast<GameObject*>(parent.internalPointer()) : game);
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
