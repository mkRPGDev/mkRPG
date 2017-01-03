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
    void setEditable(bool e);
    QModelIndex find(int id, const QModelIndex &root = QModelIndex());

private:
    GameObject *obj;
    bool aEditable;
};



class FilteredObjectsTreeModel : public QSortFilterProxyModel
{
public:
    explicit FilteredObjectsTreeModel(QObject *parent = nullptr);
    void setMode(bool eq);
    void setDisplayedItem(int nb);
    int displayedItem() const;
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
//    virtual int rowCount(const QModelIndex &parent) const;

private:
    int aDisplayedItem;
    bool eqTest;
};



class ActionsListModel : public QAbstractListModel
{
public:
    explicit ActionsListModel(QObject *parent = nullptr);
    explicit ActionsListModel(Game *g, QObject *parent = nullptr);

    void setGame(Game *g);

    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    bool setData(const QModelIndex &index, const QVariant &value, int role) Q_DECL_OVERRIDE;

    void addAction(const QString &action);


private:
    void sortActions();

    Game *game;
    QList<QString> actions;
};


class ReceiverListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit ReceiverListModel(QObject *parent = nullptr);

    void setAction(Action *action);
    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    bool removeRows(int row, int count, const QModelIndex &parent) Q_DECL_OVERRIDE;
    void addReceiver(GameObject *rcv, const QString &order);
    QPair<GameObject*, QString> receiver(int row);
private:
    void sortActions();

    Action* aAction;
    QList<QPair<GameObject*, QString>> aReceivers;
};



class ImageListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit ImageListModel(QObject *parent = nullptr);

    void setGame(Game *game);
    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;

    int getIndex(Image *im) const;
    Image *getImage(int i);

private:
    QList<Image*> images;
};


#endif // ITEMMODELS_H
