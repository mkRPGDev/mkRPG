#include "itemmodels.h"




ObjectsTreeModel::ObjectsTreeModel(QObject *parent) :
    QAbstractItemModel(parent), aEditable(true)
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
    if(static_cast<GameObject*>(index.internalPointer())->isEditable())
        return QAbstractItemModel::flags(index) | ((index.column() == 0 && aEditable) ? Qt::ItemIsEditable : Qt::NoItemFlags);
    return Qt::ItemIsEnabled;
}

bool ObjectsTreeModel::setData(const QModelIndex &index, const QVariant &value, int role){
    GameObject *obj = static_cast<GameObject*>(index.internalPointer());
    switch (role) {
    case Qt::EditRole:
        if(isValidName(value.toString())){
            obj->setName(value.toString());
            return true;
        }
    default:
        break;
    }
    return false;
}

void ObjectsTreeModel::setEditable(bool e){
    aEditable = e;
}











ActionsListModel::ActionsListModel(QObject *parent) :
    QAbstractListModel(parent), game(nullptr)
{}

ActionsListModel::ActionsListModel(Game *g, QObject *parent) :
    ActionsListModel(parent)
{
    setGame(g);
}

void ActionsListModel::setGame(Game *g){
    beginResetModel();
    game = g;
    actions = game ? game->actions() : QList<QString>();
    endResetModel();
}

int ActionsListModel::rowCount(const QModelIndex &parent) const{
    return parent.isValid() ? 0 : actions.length();
}

QVariant ActionsListModel::data(const QModelIndex &index, int role) const{
    if(!index.isValid()) return QVariant();
    switch (role) {
    case Qt::EditRole:
    case Qt::DisplayRole: return actions.at(index.row());
    default: return QVariant();
    }
}

Qt::ItemFlags ActionsListModel::flags(const QModelIndex &UNUSED(index)) const{
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

bool ActionsListModel::setData(const QModelIndex &index, const QVariant &value, int role){
    if(!index.isValid() || role == Qt::DisplayRole) return false;
    QString act = value.toString();
    if(!isValidName(act)) return false;
    actions[index.row()] = game->renameAction(actions.at(index.row()), act);
    emit dataChanged(index,index);
    sortActions();
    return true;
}



void ActionsListModel::addAction(const QString &name){
    if(game != nullptr){
        QModelIndex ins = index(rowCount(QModelIndex())-1, 0, QModelIndex());
        emit beginInsertRows(ins, 0, 0);
        actions.append(game->addAction(name, new Action()));
        emit endInsertRows();
        sortActions();
    }
}

void ActionsListModel::sortActions(){
    emit layoutAboutToBeChanged();
    std::sort(actions.begin(), actions.end(), cleverComp);
    emit layoutChanged();
}





ReceiverListModel::ReceiverListModel(QObject *parent) :
    QAbstractListModel(parent), aAction(nullptr)
{}

void ReceiverListModel::setAction(Action *action){
    beginResetModel();
    aAction = action;
    aReceivers = aAction ? aAction->receivers() : QList<QPair<GameObject*, QString>>();
    sortActions();
    endResetModel();
}


int ReceiverListModel::rowCount(const QModelIndex &parent) const{
    return aReceivers.length();
}

QVariant ReceiverListModel::data(const QModelIndex &index, int role) const{
    if(!index.isValid()) return QVariant();
    const QPair<GameObject*, QString> &rcv(aReceivers[index.row()]);
    switch (role) {
    case Qt::DisplayRole: return QVariant(rcv.first->name()+"::"+rcv.second);
    case Qt::SizeHintRole: return QSize(200,24);
    default: return QVariant();
    }
}

Qt::ItemFlags ReceiverListModel::flags(const QModelIndex &index) const{
    return QAbstractListModel::flags(index) | Qt::ItemIsEditable;
}

void ReceiverListModel::addReceiver(GameObject *rcv, const QString &order){
    if(aAction){
        beginResetModel();
        aAction->addReceiver(rcv, order);
        aReceivers = aAction->receivers();
        sortActions();
        endResetModel();
    }
}

void ReceiverListModel::sortActions(){
    std::sort(aReceivers.begin(), aReceivers.end(),
              [](const QPair<GameObject*, QString> &a,const QPair<GameObject*, QString> &b){
        return a.first->name() == b.first->name() ? cleverComp(a.second, b.second)
                                                  : cleverComp(a.first->name(), b.first->name());
    });
}


