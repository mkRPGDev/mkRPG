#ifndef ITEMMODELS_H
#define ITEMMODELS_H


#include <QAbstractItemModel>
#include "../game.h"



class ObjectsTreeModel : public QAbstractItemModel
{
public:
    explicit ObjectsTreeModel(QObject *parent = nullptr);
    explicit ObjectsTreeModel(Game *g, QObject *parent = nullptr);
    void setGame(Game *g);
    int columnCount(const QModelIndex &parent) const;
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);

private:
    Game *game;
};

#endif // ITEMMODELS_H
