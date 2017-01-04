#ifndef TREEVIEWS_H
#define TREEVIEWS_H

/*!
 * \file treeviews.h
 * \brief Definition of custom tree view classes.
 *
 * Due to Qt's unsupported template feature in its MOC,
 * the class can't be templates so the code is duplicate
 * for each case.
 */

#include <QTreeView>
#include "itemdelegates.h"


/*!
 * \brief The ParamTreeView class customize Qt QTreeView class
 * to render ParamTreeItemModel.
 */
class ParamTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit ParamTreeView(QWidget *parent = nullptr);
    void expandView(const QModelIndex &index = QModelIndex());

private:
    void contextMenuEvent(QContextMenuEvent *me);
    void drawRow(QPainter *painter, const QStyleOptionViewItem &options, const QModelIndex &index) const;
};


/*!
 * \brief The FlagTreeView class customize Qt QTreeView class
 * to render FlagTreeItemModel.
 */
class FlagTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit FlagTreeView(QWidget *parent = nullptr);
    void expandView(const QModelIndex &index = QModelIndex());

private:
    void contextMenuEvent(QContextMenuEvent *me);
    void drawRow(QPainter *painter, const QStyleOptionViewItem &options, const QModelIndex &index) const;
};


/*!
 * \brief The EventTreeView class customize Qt QTreeView class
 * to render EventTreeItemModel.
 */
class EventTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit EventTreeView(QWidget *parent = nullptr);
    void expandView(const QModelIndex &index = QModelIndex());

private:
    void contextMenuEvent(QContextMenuEvent *me);
    void drawRow(QPainter *painter, const QStyleOptionViewItem &options, const QModelIndex &index) const;
};


/*!
 * \brief The OrderTreeView class customize Qt QTreeView class
 * to render OrderTreeItemModel.
 */
class OrderTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit OrderTreeView(QWidget *parent = nullptr);
    void expandView(const QModelIndex &index = QModelIndex());

private:
    void contextMenuEvent(QContextMenuEvent *me);
    void drawRow(QPainter *painter, const QStyleOptionViewItem &options, const QModelIndex &index) const;
};


#endif // TREEVIEWS_H
