#ifndef TYPEITEMMODEL_H
#define TYPEITEMMODEL_H


#include <QAbstractItemModel>
#include "../game.h"



class TypeTreeItem{
    enum State{RootItem, BaseItem, TypeItem};
public:
    explicit TypeTreeItem(Game *game);
    ~TypeTreeItem();

    Qt::ItemFlags flags(int col) const;
    QVariant data(int col, int role) const;
    TypeTreeItem *parent() const ;
    TypeTreeItem *child(int row) const ;
    int rowCount() const;
    int row() const;
    bool setData(int col, QVariant value, int role);

private:
    explicit TypeTreeItem(QString typeName, TypeTreeItem *parent);
    explicit TypeTreeItem(Type *typ, TypeTreeItem *parent);

    TypeTreeItem* findType(const QString &t);

    int rowNb;
    State state;
    QString typeName;
    TypeTreeItem *parentItem;
    Type *type;
    QList<TypeTreeItem*> children;
};


class TypeItemModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit TypeItemModel(QObject *parent = nullptr);

    int columnCount(const QModelIndex &parent) const;
    int rowCount(const QModelIndex &parent) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant data(const QModelIndex &index, int role) const;
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);

    void setGame(Game* g);
private:
    TypeTreeItem *rootItem;



};

#endif // TYPEITEMMODEL_H
