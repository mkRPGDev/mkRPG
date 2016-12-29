#include "treeviews.h"

ParamTreeView::ParamTreeView(QWidget *parent) :
    QTreeView(parent)
{
    header()->setStretchLastSection(true);
    setItemDelegateForColumn(1, new ParamItemDelegate(this));


    setIndentation(10);
    setAlternatingRowColors(true);
    setSelectionMode(QAbstractItemView::NoSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setEditTriggers(editTriggers() | QAbstractItemView::CurrentChanged);
}


void ParamTreeView::contextMenuEvent(QContextMenuEvent *me){
    me->accept();
    qDebug() << me->pos() << indexAt(me->pos());
}

void ParamTreeView::drawRow(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyleOptionViewItem newOption(option);
    if(index.data(Qt::BackgroundRole).isValid()){
        painter->fillRect(option.rect, index.data(Qt::BackgroundRole).value<QBrush>());
        newOption.palette.setColor(QPalette::AlternateBase, index.data(Qt::BackgroundRole).value<QBrush>().color());
    }
    if(index.data(Qt::ForegroundRole).isValid())
        newOption.palette.setColor(QPalette::Text, index.data(Qt::ForegroundRole).value<QBrush>().color());

    QTreeView::drawRow(painter, newOption, index);
}


void ParamTreeView::expandView(const QModelIndex &index){
    int l = model()->rowCount(index);
    for(int i(0); i<l; ++i){
        QModelIndex c(model()->index(i,0,index));
        expand(c);
        //expandView(c);
    }
}






FlagTreeView::FlagTreeView(QWidget *parent) :
    QTreeView(parent)
{
    header()->setStretchLastSection(true);
    setItemDelegateForColumn(1, new FlagItemDelegate(this));


    setIndentation(10);
    setAlternatingRowColors(true);
    setSelectionMode(QAbstractItemView::NoSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setEditTriggers(editTriggers() | QAbstractItemView::CurrentChanged);
}


void FlagTreeView::contextMenuEvent(QContextMenuEvent *me){
    me->accept();
    qDebug() << me->pos() << indexAt(me->pos());
}

void FlagTreeView::drawRow(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyleOptionViewItem newOption(option);
    if(index.data(Qt::BackgroundRole).isValid()){
        painter->fillRect(option.rect, index.data(Qt::BackgroundRole).value<QBrush>());
        newOption.palette.setColor(QPalette::AlternateBase, index.data(Qt::BackgroundRole).value<QBrush>().color());
    }
    if(index.data(Qt::ForegroundRole).isValid())
        newOption.palette.setColor(QPalette::Text, index.data(Qt::ForegroundRole).value<QBrush>().color());

    QTreeView::drawRow(painter, newOption, index);
}

void FlagTreeView::expandView(const QModelIndex &index){
    int l = model()->rowCount(index);
    for(int i(0); i<l; ++i){
        QModelIndex c(model()->index(i,0,index));
        expand(c);
    }
}





SignalTreeView::SignalTreeView(QWidget *parent) :
    QTreeView(parent)
{
    header()->setStretchLastSection(true);
    // TODO
    //setItemDelegateForColumn(1, new FlagItemDelegate(this));


    setIndentation(10);
    setAlternatingRowColors(true);
    setSelectionMode(QAbstractItemView::NoSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setEditTriggers(editTriggers() | QAbstractItemView::CurrentChanged);
}


void SignalTreeView::contextMenuEvent(QContextMenuEvent *me){
    me->accept();
    qDebug() << me->pos() << indexAt(me->pos());
}

void SignalTreeView::drawRow(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyleOptionViewItem newOption(option);
    if(index.data(Qt::BackgroundRole).isValid()){
        painter->fillRect(option.rect, index.data(Qt::BackgroundRole).value<QBrush>());
        newOption.palette.setColor(QPalette::AlternateBase, index.data(Qt::BackgroundRole).value<QBrush>().color());
    }
    if(index.data(Qt::ForegroundRole).isValid())
        newOption.palette.setColor(QPalette::Text, index.data(Qt::ForegroundRole).value<QBrush>().color());

    QTreeView::drawRow(painter, newOption, index);
}

void SignalTreeView::expandView(const QModelIndex &index){
    int l = model()->rowCount(index);
    for(int i(0); i<l; ++i){
        QModelIndex c(model()->index(i,0,index));
        expand(c);
    }
}




SlotTreeView::SlotTreeView(QWidget *parent) :
    QTreeView(parent)
{
    header()->setStretchLastSection(true);
    // TODO
    //setItemDelegateForColumn(1, new FlagItemDelegate(this));


    setIndentation(10);
    setAlternatingRowColors(true);
    setSelectionMode(QAbstractItemView::NoSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setEditTriggers(editTriggers() | QAbstractItemView::CurrentChanged);
}


void SlotTreeView::contextMenuEvent(QContextMenuEvent *me){
    me->accept();
    qDebug() << me->pos() << indexAt(me->pos());
}

void SlotTreeView::drawRow(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyleOptionViewItem newOption(option);
    if(index.data(Qt::BackgroundRole).isValid()){
        painter->fillRect(option.rect, index.data(Qt::BackgroundRole).value<QBrush>());
        newOption.palette.setColor(QPalette::AlternateBase, index.data(Qt::BackgroundRole).value<QBrush>().color());
    }
    if(index.data(Qt::ForegroundRole).isValid())
        newOption.palette.setColor(QPalette::Text, index.data(Qt::ForegroundRole).value<QBrush>().color());

    QTreeView::drawRow(painter, newOption, index);
}

void SlotTreeView::expandView(const QModelIndex &index){
    int l = model()->rowCount(index);
    for(int i(0); i<l; ++i){
        QModelIndex c(model()->index(i,0,index));
        expand(c);
    }
}
