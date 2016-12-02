#include "mapslistmodel.h"

MapsListModel::MapsListModel(World* w, QObject* parent) :
    QAbstractListModel(parent), maps(w->maps()), mps(QMap<int, MapPainter*>())
{

    update();
}

int MapsListModel::rowCount(const QModelIndex &parent) const{
    //qDebug() << maps.length() << parent.isValid() << parent.model() << parent;
    return parent.isValid() ? 0 : maps.length();
}

QVariant MapsListModel::data(const QModelIndex &index, int role) const{
    //qDebug() << "data" << role;
    if(index.isValid()){
        switch (role) {
        case Qt::DisplayRole:
            return QVariant(maps.at(index.row())->name());
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
    QAbstractListModel(parent), cellTypes(w->cellTypes()), empty(QPixmap(32,32))
{
    empty.fill(QColor(0,0,0,0));
}

int CellTypeListModel::rowCount(const QModelIndex &parent) const{
    //qDebug() << maps.length() << parent.isValid() << parent.model() << parent;
    return parent.isValid() ? 0 : cellTypes.length()+1;
}

QVariant CellTypeListModel::data(const QModelIndex &index, int role) const{
    //qDebug() << "data" << role;
    if(index.isValid()){
        if(index.row() == 0) return role == Qt::DisplayRole ? QVariant(tr("EmptyType")) :
                                    role == Qt::DecorationRole ? QVariant(empty) : QVariant();
        CellType *ct = cellTypes.at(index.row()-1);
        switch (role) {
        case Qt::DisplayRole:
            return QVariant(QString("Ident : ")+QString::number(ct->ident()));
        case Qt::DecorationRole:
            if(ct->image() != nullptr)
                return QVariant(QPixmap::fromImage(ct->image()->image().scaled(32,32)));
            else
                return QVariant(QPixmap(32,32));
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





