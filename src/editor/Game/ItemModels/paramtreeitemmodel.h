#ifndef PARAMTREEITEMMODEL_H
#define PARAMTREEITEMMODEL_H


#include <QAbstractItemModel>
#include "../game.h"

class GameTreeItem{
    enum State{Type, Object, Parameter, Value};
public:
    explicit GameTreeItem(GameObject &obj);
    explicit GameTreeItem(InheritableObject &typ);
    ~GameTreeItem();

    Qt::ItemFlags flags(int col) const;
    QVariant data(int col, int role) const;
    GameTreeItem *parent() const ;
    GameTreeItem *child(int row) const ;
    int rowCount() const;
    int row() const;
    bool setData(int col, QVariant value, int role);

private:
    explicit GameTreeItem(int rowNb, GameObject *obj, InheritableObject* typ, State state, GameTreeItem *parent);
    int rowNb;
    GameTreeItem *parentItem;
    QList<GameTreeItem *> children;

    void genealogy(InheritableObject *t);

    QVariant typeData(int col, int role) const;
    QVariant objectData(int col, int role) const;
    QVariant parameterData(int col, int role) const;
    QVariant valueData(int col, int role) const;
    //bool setTypeData(int col, QVariant value, int role);
    //bool setObjectData(int col, QVariant value, int role);
    bool setParameterData(int col, QVariant value, int role);
    bool setValueData(int col, QVariant value, int role);

    GameObject *obj;
    InheritableObject *typ;

    InheritableObject *anc;
    QList<InheritableObject*> ancestors;
    QString attr;
    QList<QString> attrs;

    State state;
};


class ParamTreeItemModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit ParamTreeItemModel(QObject *parent = 0);
    void setObject(GameObject *o);
    int columnCount(const QModelIndex &parent) const;
    int rowCount(const QModelIndex &parent) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant data(const QModelIndex &index, int role) const;
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    //Qt::ItemFlags flags(const QModelIndex &index) const;
    //bool setData(const QModelIndex &index, const QVariant &value, int role);

private:
    Game *game;
    GameObject *obj;
    InheritableObject *type;


    GameTreeItem *item;
};

/*

class FlagTreeItemModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit FlagTreeItemModel(QObject *parent = 0);
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
};*/

#endif // PARAMTREEITEMMODEL_H
