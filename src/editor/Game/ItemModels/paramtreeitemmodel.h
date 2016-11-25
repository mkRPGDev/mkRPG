#ifndef PARAMTREEITEMMODEL_H
#define PARAMTREEITEMMODEL_H


#include <QAbstractItemModel>
#include "../game.h"

class ParamTreeItemModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit ParamTreeItemModel(QObject *parent = 0);
    void setObject(GameObject *o);
    int columnCount(const QModelIndex &parent) const;
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    //Qt::ItemFlags flags(const QModelIndex &index) const;
    //bool setData(const QModelIndex &index, const QVariant &value, int role);

private:
    Game *game;
    GameObject *obj;
    GameObjectType *type;

    GameObjectType *ancestor(GameObjectType* obj, int &gen) const;
};

#endif // PARAMTREEITEMMODEL_H
