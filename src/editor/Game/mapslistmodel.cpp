#include "mapslistmodel.h"

MapsListModel::MapsListModel(World* w, QObject* parent) :
    QAbstractListModel(parent), maps(w->maps()), mps(QMap<int, MapPainter*>())
{

    update();
}

int MapsListModel::rowCount(const QModelIndex &parent) const{
    //qDebug() << maps.length() << parent.isValid() << parent.model() << parent;
    return maps.length();
}

QVariant MapsListModel::data(const QModelIndex &index, int role) const{
    //qDebug() << "data" << role;
    if(index.isValid()){
        switch (role) {
        case Qt::DisplayRole:
            return QVariant(QString("Map ") +QString::number(maps.at(index.row())->ident()));
        case Qt::DecorationRole:
            return QVariant(viewOf(maps.at(index.row())));
        case Qt::UserRole:
            return QVariant(maps.at(index.row())->ident());
        default:
            break;
        }

    }
    return QVariant();
}

QImage MapsListModel::viewOf(Map *m) const{
    int id = m->ident();
    return mps.contains(id) ? QImage(mps[id]->render()) : QImage();
}

void MapsListModel::update(){
    // TODO ? Recharger maps
    for(Map* m : maps)
        if(!mps.contains(m->ident())){
            MapPainter *mp = new MapPainter(m, this);
            mps[m->ident()] = mp;
            mp->setPaintedElement(MapPainter::Grid | MapPainter::CellSelection | MapPainter::CellHighlighting, false);
            mp->resize(200,160);
        }
    // TODO : enlever les déchets
}

bool MapsListModel::insertRows(int row, int count, const QModelIndex &parent){
    beginInsertRows(parent, row, row+count);

    endInsertRows();
    return true;
}

bool MapsListModel::removeRows(int row, int count, const QModelIndex &parent){
    beginRemoveRows(parent, row, row+count);

    endInsertRows();
    return true;
}







CellTypeListModel::CellTypeListModel(World* w, QObject* parent) :
    QAbstractListModel(parent), cellTypes(w->cellTypes())
{

}

int CellTypeListModel::rowCount(const QModelIndex &parent) const{
    //qDebug() << maps.length() << parent.isValid() << parent.model() << parent;
    return cellTypes.length();
}

QVariant CellTypeListModel::data(const QModelIndex &index, int role) const{
    //qDebug() << "data" << role;
    if(index.isValid()){
        CellType *ct = cellTypes.at(index.row());
        switch (role) {
        case Qt::DisplayRole:
            return QVariant(QString("Ident : ")+QString::number(ct->ident()));
        case Qt::DecorationRole:
            return QVariant(QPixmap::fromImage(ct->image()->image().scaled(32,32)));
        case Qt::UserRole:
            return QVariant(ct->ident());
        default:
            break;
        }

    }
    return QVariant();
}


bool CellTypeListModel::insertRows(int row, int count, const QModelIndex &parent){
    beginInsertRows(parent, row, row+count);

    endInsertRows();
    return true;
}

bool CellTypeListModel::removeRows(int row, int count, const QModelIndex &parent){
    beginRemoveRows(parent, row, row+count);

    endInsertRows();
    return true;
}






ObjectParamTableModel::ObjectParamTableModel(GameObject *obj, QObject *parent) :
    QAbstractTableModel(parent),
    obj(obj)
{
}

int ObjectParamTableModel::rowCount(const QModelIndex &parent) const{
    return obj->params().length();
}

int ObjectParamTableModel::columnCount(const QModelIndex &parent) const{
    return 2;
}

QVariant ObjectParamTableModel::data(const QModelIndex &index, int role) const{
    if(role == Qt::DisplayRole){
        if(index.column() == 1) return QVariant(obj->getParam(obj->params().at(index.row())));
        if(index.column() == 0) return QVariant(obj->params().at(index.row()));
    }
    return QVariant();
}

Qt::ItemFlags ObjectParamTableModel::flags(const QModelIndex &index) const{
    return QAbstractTableModel::flags(index) | ((index.column() == 1) ? Qt::ItemIsEditable : Qt::NoItemFlags);
}

bool ObjectParamTableModel::setData(const QModelIndex &index, const QVariant &value, int role){
    obj->setParam(obj->params().at(index.row()), value.toInt());
    return true;
}
