#include "treeviews.h"

ParamTreeView::ParamTreeView(QWidget *parent) :
    QTreeView(parent)
{
    header()->setStretchLastSection(true);
    setItemDelegateForColumn(1, new ParamItemDelegate(this));
}


void ParamTreeView::contextMenuEvent(QContextMenuEvent *me){
    me->accept();
    qDebug() << me->pos() << indexAt(me->pos());
}

void ParamTreeView::drawRow(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyleOptionViewItem newOption(option);
    if(index.data(Qt::BackgroundRole).isValid())
        painter->fillRect(option.rect, index.data(Qt::BackgroundRole).value<QBrush>());
    if(index.data(Qt::ForegroundRole).isValid())
        newOption.palette.setColor(QPalette::Text, index.data(Qt::ForegroundRole).value<QBrush>().color());

    QTreeView::drawRow(painter, newOption, index);
}





FlagTreeView::FlagTreeView(QWidget *parent) :
    QTreeView(parent)
{
    header()->setStretchLastSection(true);
    setItemDelegateForColumn(1, new FlagItemDelegate(this));
}


void FlagTreeView::contextMenuEvent(QContextMenuEvent *me){
    me->accept();
    qDebug() << me->pos() << indexAt(me->pos());
}

void FlagTreeView::drawRow(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyleOptionViewItem newOption(option);
    if(index.data(Qt::BackgroundRole).isValid())
        painter->fillRect(option.rect, index.data(Qt::BackgroundRole).value<QBrush>());
    if(index.data(Qt::ForegroundRole).isValid())
        newOption.palette.setColor(QPalette::Text, index.data(Qt::ForegroundRole).value<QBrush>().color());

    QTreeView::drawRow(painter, newOption, index);
}

