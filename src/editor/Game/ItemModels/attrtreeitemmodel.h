#ifndef ATTRTREEITEMMODEL_H
#define ATTRTREEITEMMODEL_H


#include <QAbstractItemModel>
#include "../game.h"
#include <algorithm>

/*!
 * \file attrtreeitemmodel.h
 *
 * \brief Definition of the model presentation class for
 * parameter and flags as a tree.
 *
 * \warning
 * Do not try to factorized the ParamTreeItemModel and FlagTreeItemModel
 * in a single template class, templates are not supported by Qt
 * Meta-Object Compiler.
 */

/*!
 * \brief The ItemType enum describes the several types
 * of items used in the template class GameTreeItem.
 */
enum ItemType{
    ParamItem,  /**< Integer parameter */
    FlagItem,   /**< Flag */
    EventItem, /**< %Event */
    OrderItem    /**< %Order */
};

/*!
 * \brief The GameTreeItem define a convienent tree item for
 * the ParamTreeItemModel and FlagTreeItemModel classes
 *
 * It ensures items to have a parent, and provides abstract
 * item (not related to object, parameter or flag).
 *
 * \see ParamTreeItemModel, FlagTreeItemModel
 */
template<ItemType type>
class GameTreeItem{
    enum State{Type, Object, Attribute, Value};
public:
    explicit GameTreeItem(GameObject &obj);
    explicit GameTreeItem(InheritableObject &typ);
    ~GameTreeItem();

    Qt::ItemFlags flags(int col, bool readOnly = false) const;
    QVariant data(int col, int role) const;
    GameTreeItem *parent() const ;
    GameTreeItem *child(int row) const ;
    int rowCount() const;
    int row() const;
    bool setData(int col, QVariant value, int role);
    void addAttr(const QString &attr);
    void sort();
    void setAttributeRowNb(int r);
    bool isAttr(const QString &a) const;

private:
    explicit GameTreeItem(int rowNb, GameObject *obj, InheritableObject* typ, State state, GameTreeItem *parent);
    int rowNb;
    GameTreeItem<type> *parentItem;
    QList<GameTreeItem<type>*> children;

    void genealogy(InheritableObject *t);

    QVariant typeData(int col, int role) const;
    QVariant objectData(int col, int role) const;
    QVariant attrData(int col, int role) const;
    QVariant valueData(int col, int role) const;
    //bool setTypeData(int col, QVariant value, int role);
    //bool setObjectData(int col, QVariant value, int role);
    bool setAttrData(int col, QVariant value, int role);
    bool setValueData(int col, QVariant value, int role);

    Qt::ItemFlags typeFlags(int, bool) const;
    Qt::ItemFlags objectFlags(int, bool) const;
    Qt::ItemFlags attrFlags(int col, bool readOnly) const;
    Qt::ItemFlags valueFlags(int col, bool readOnly) const;

    GameObject *obj;
    InheritableObject *typ;

    InheritableObject *anc;
    QList<InheritableObject*> ancestors;
    QString attr;
    QList<QString> attrs;

    State state;
    QColor bgColor;
};



/*!
 * \brief The ParamTreeItemModel class presents the parameters
 * of an object using the QAbstractItemModel interface.
 *
 * \see FlagTreeItemModel, EventTreeItemModel, OrderTreeItemModel
 */
class ParamTreeItemModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit ParamTreeItemModel(QObject *parent = 0);
    void setObject(GameObject *o);
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant data(const QModelIndex &index, int role) const;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    void addParam(const QString &name);
    void sortAttr(const QModelIndex &par);

private:
    Game *game;
    GameObject *obj;
    InheritableObject *type;

    GameTreeItem<ParamItem> *item;
};


/*!
 * \brief The ParamTreeItemModel class presents the flags
 * of an object using the QAbstractItemModel interface.
 *
 * \see ParamTreeItemModel, EventTreeItemModel, OrderTreeItemModel
 */
class FlagTreeItemModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit FlagTreeItemModel(QObject *parent = 0);
    void setObject(GameObject *o);
    int columnCount(const QModelIndex &parent) const;
    int rowCount(const QModelIndex &parent) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant data(const QModelIndex &index, int role) const;
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    void addFlag(const QString &name);
    void sortAttr(const QModelIndex &par);

private:
    Game *game;
    GameObject *obj;
    InheritableObject *type;

    GameTreeItem<FlagItem> *item;
};


/*!
 * \brief The EventTreeItemModel class presents the events
 * of an object using the QAbstractItemModel interface.
 *
 * \see ParamTreeItemModel, FlagTreeItemModel, OrderTreeItemModel
 */
class EventTreeItemModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit EventTreeItemModel(QObject *parent = 0, bool readOnly = false);
    void setObject(GameObject *o);
    int columnCount(const QModelIndex &parent) const;
    int rowCount(const QModelIndex &parent) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant data(const QModelIndex &index, int role) const;
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    void addEvent(const QString &name);
    void sortAttr(const QModelIndex &par);

private:
    Game *game;
    GameObject *obj;
    InheritableObject *type;

    GameTreeItem<EventItem> *item;
    bool readOnly;
};



/*!
 * \brief The OrderTreeItemModel class presents the orders
 * of an object using the QAbstractItemModel interface.
 *
 * \see ParamTreeItemModel, FlagTreeItemModel, EventTreeItemModel
 */
class OrderTreeItemModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit OrderTreeItemModel(QObject *parent = 0, bool readOnly = false);
    void setObject(GameObject *o);
    int columnCount(const QModelIndex &parent) const;
    int rowCount(const QModelIndex &parent) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant data(const QModelIndex &index, int role) const;
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    void addOrder(const QString &name);
    void sortAttr(const QModelIndex &par);

    QModelIndex findOrder(const QString &order, const QModelIndex &root = QModelIndex());

private:
    Game *game;
    GameObject *obj;
    InheritableObject *type;

    GameTreeItem<OrderItem> *item;
    bool readOnly;
};
#endif // ATTRTREEITEMMODEL_H
