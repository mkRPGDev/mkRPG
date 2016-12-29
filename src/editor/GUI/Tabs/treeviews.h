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


class SignalTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit SignalTreeView(QWidget *parent = nullptr);
    void expandView(const QModelIndex &index = QModelIndex());

private:
    void contextMenuEvent(QContextMenuEvent *me);
    void drawRow(QPainter *painter, const QStyleOptionViewItem &options, const QModelIndex &index) const;
};


class SlotTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit SlotTreeView(QWidget *parent = nullptr);
    void expandView(const QModelIndex &index = QModelIndex());

private:
    void contextMenuEvent(QContextMenuEvent *me);
    void drawRow(QPainter *painter, const QStyleOptionViewItem &options, const QModelIndex &index) const;
};


#endif // TREEVIEWS_H
