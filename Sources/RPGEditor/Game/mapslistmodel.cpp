#include "mapslistmodel.h"

MapsListModel::MapsListModel(World* w, QObject* parent) :
    QAbstractListModel(parent), maps(w->maps())
{

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
            return QVariant(index.row() < 5 ? QString("eee") : "i");
        case Qt::DecorationRole:
            return QVariant(QPixmap(":/Icons/main.png"));
        default:
            break;
        }

    }
    return QVariant();
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
