#include "mapslistmodel.h"

MapsListModel::MapsListModel(QObject* parent) :
    QAbstractListModel(parent), mps(QMap<int, MapPainter*>())
{}

MapsListModel::MapsListModel(World &w, QObject* parent) :
    MapsListModel(parent)
{
    setWorld(w);
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


void MapsListModel::setWorld(World &w){
    beginResetModel();
    maps = w.objects().maps();
    update();
    endResetModel();
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
/*
Qt::ItemFlags MapsListModel::flags(const QModelIndex &UNUSED(index)) const{
    return Qt::ItemIsEnabled;
}*/

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







CellTypeListModel::CellTypeListModel(QObject* parent) :
    QAbstractListModel(parent)
{}

void CellTypeListModel::readCellTypes(CellType *ct){
    cellTypes.append(ct);
    for(CellType *c : ct->descendants())
        readCellTypes(c);
}

void CellTypeListModel::setGame(Game *game){
    cellTypes.clear();
    if(game)
        readCellTypes(&game->world().types().cellType());
}

int CellTypeListModel::rowCount(const QModelIndex &parent) const{
    return parent.isValid() ? 0 : cellTypes.length();
}

QVariant CellTypeListModel::data(const QModelIndex &index, int role) const{
    if(index.isValid()){
        CellType *ct = cellTypes.at(index.row());
        switch (role) {
        case Qt::DisplayRole:
            return QVariant(ct->name());
        case Qt::DecorationRole:
            if(ct->image() != nullptr)
                return QVariant(QPixmap::fromImage(ct->image()->image().scaled(32,32)));
            else{
                QPixmap p(32,32);
                p.fill(QColor(0,0,0,0));
                return QVariant(p);
            }
        case Qt::UserRole:
            return QVariant(ct->ident());
        default:
            break;
        }

    }
    return QVariant();
}


Qt::ItemFlags CellTypeListModel::flags(const QModelIndex &UNUSED(index)) const{
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

CellType &CellTypeListModel::cellTypeAt(int i) const{
    assert(i<cellTypes.length());
    return *cellTypes[i];
}

int CellTypeListModel::indexOf(CellType *ct){
    return cellTypes.indexOf(ct);
}








MapTypeListModel::MapTypeListModel(QObject* parent) :
    QAbstractListModel(parent)
{}

void MapTypeListModel::readMapTypes(MapType *ct){
    mapTypes.append(ct);
    for(MapType *c : ct->descendants())
        readMapTypes(c);
}

void MapTypeListModel::setGame(Game *game){
    mapTypes.clear();
    if(game)
        readMapTypes(&game->world().types().mapType());
}

int MapTypeListModel::rowCount(const QModelIndex &parent) const{
    return parent.isValid() ? 0 : mapTypes.length();
}

QVariant MapTypeListModel::data(const QModelIndex &index, int role) const{
    if(index.isValid()){
        MapType *ct = mapTypes.at(index.row());
        switch (role) {
        case Qt::DisplayRole:
            return QVariant(ct->name());
        case Qt::UserRole:
            return QVariant(ct->ident());
        default:
            break;
        }

    }
    return QVariant();
}


Qt::ItemFlags MapTypeListModel::flags(const QModelIndex &UNUSED(index)) const{
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

MapType &MapTypeListModel::mapTypeAt(int i) const{
    assert(i<mapTypes.length());
    return *mapTypes[i];
}

int MapTypeListModel::indexOf(MapType *mt){
    return mapTypes.indexOf(mt);
}
