#ifndef ITEMMODELS_H
#define ITEMMODELS_H


#include <QAbstractItemModel>
#include "../game.h"

/*!
 * \file itemmodels.h
 *
 * \brief Definition of some model presentation class
 */


/*!
 * \brief The ObjectsTreeModel class presents an GameObject and
 * its chidren as a tree model.
 */
class ObjectsTreeModel : public QAbstractItemModel
{
public:
    explicit ObjectsTreeModel(QObject *parent = nullptr);
    explicit ObjectsTreeModel(GameObject *o, QObject *parent = nullptr);
    void setGameObject(GameObject *o);
    int columnCount(const QModelIndex &) const;
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);

private:
    GameObject *obj;
};

#endif // ITEMMODELS_H
