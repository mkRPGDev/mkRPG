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
    explicit MapsListModel(World *w, QObject *parent = 0);
    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
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
 * \brief The CellTypeListModel class
 */
class CellTypeListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit CellTypeListModel(World *w, QObject *parent = 0);
    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    bool insertRows(int row, int count, const QModelIndex &parent) Q_DECL_OVERRIDE;
    bool removeRows(int row, int count, const QModelIndex &parent) Q_DECL_OVERRIDE;

private:
    QList<CellType*> cellTypes;
    QPixmap empty;
};




#endif // MAPSLISTMODEL_H
