#ifndef MAPSLISTMODEL_H
#define MAPSLISTMODEL_H

#include <QAbstractListModel>
#include "game.h"
#include "mappainter.h"

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
};


#endif // MAPSLISTMODEL_H
