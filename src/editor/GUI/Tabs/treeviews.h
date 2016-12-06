#ifndef TREEVIEWS_H
#define TREEVIEWS_H

#include <QTreeView>
#include "itemdelegates.h"

class ParamTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit ParamTreeView(QWidget *parent = nullptr);

private:
    void contextMenuEvent(QContextMenuEvent *me);
    void drawRow(QPainter *painter, const QStyleOptionViewItem &options, const QModelIndex &index) const;
};


class FlagTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit FlagTreeView(QWidget *parent = nullptr);

private:
    void contextMenuEvent(QContextMenuEvent *me);
    void drawRow(QPainter *painter, const QStyleOptionViewItem &options, const QModelIndex &index) const;
};




#endif // TREEVIEWS_H
