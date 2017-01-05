#ifndef MAPSLISTMODEL_H
#define MAPSLISTMODEL_H

#include <QAbstractListModel>
#include <QAbstractTableModel>
#include "../game.h"
#include "../mappainter.h"

/*!
 * \file mapslistmodel.h
 * \brief Definition of Model/View presentation classes.
 */


/*!
 * \brief The MapsListModel class provides a presentation class
 * for the Qt Model-View framework.
 */
class MapsListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit MapsListModel(QObject *parent = 0);
    explicit MapsListModel(World &w, QObject *parent = 0);
    void setWorld(World &w);
    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    //Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    bool insertRows(int row, int count, const QModelIndex &parent) Q_DECL_OVERRIDE;
    bool removeRows(int row, int count, const QModelIndex &parent) Q_DECL_OVERRIDE;


public slots:
    void update();

private:
    QImage viewOf(Map *m) const;
    QList<Map*> maps; // WARNING : à mettre à jour !
    QMap<int, MapPainter*> mps; // WARNING : supprimer les cartes enlevées !
};


/*!
 * \brief The CellTypeListModel class presents the \ref "cell types" CellType
 * as a Qt Model-View framework's list model.
 */
class CellTypeListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit CellTypeListModel(QObject *parent = 0);
    void setGame(Game *game);
    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    CellType &cellTypeAt(int i) const;
    int indexOf(CellType* ct);

private:
    void readCellTypes(CellType *ct);
    QList<CellType*> cellTypes;
};



/*!
 * \brief The CellTypeListModel class presents the \ref "map types" MapType
 * as a Qt Model-View framework's list model.
 */
class MapTypeListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit MapTypeListModel(QObject *parent = 0);
    void setGame(Game *game);
    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    MapType &mapTypeAt(int i) const;
    int indexOf(MapType* mt);

private:
    void readMapTypes(MapType *mt);
    QList<MapType*> mapTypes;
};


#endif // MAPSLISTMODEL_H
