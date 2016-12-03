#include "itemdelegates.h"



ParamItemDelegate::ParamItemDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
    setItemEditorFactory(new QItemEditorFactory);
}


QWidget* ParamItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &UNUSED(option), const QModelIndex &UNUSED(index)) const{
    QSpinBox *sp = new QSpinBox(parent);
    sp->setAutoFillBackground(true);
    sp->setMaximum(10000);
    sp->setFrame(false);
    return sp;
}

void ParamItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const{
    editor->setProperty("value", index.data(Qt::DisplayRole));
}

void ParamItemDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &UNUSED(index)) const{
    editor->setGeometry(option.rect);
}

void ParamItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const{
    model->setData(index, editor->property("value"));
}

void ParamItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const{
    QStyledItemDelegate::paint(painter, option, index);
//    painter->drawText(option.rect, index.data().toString());
    /*QLabel l;
    l.setText(index.data());
    l.setGeometry(option.rect);
    painter->draw*/

    return; // EN COURS
    QStyleOptionButton button;
    button.palette = option.palette;
    button.rect = option.rect;
    button.rect.setLeft(option.rect.left()+2);
    button.text = index.data().toString();
    QApplication::style()->drawControl(QStyle::CE_CheckBoxLabel, &button, painter, 0);
    button.palette = option.palette;
    button.rect = option.rect;
    button.rect.setLeft(button.rect.right()-20);
    button.icon = QIcon(":/Icons/edit.png");
    button.iconSize = QSize(16,16);
    QApplication::style()->drawControl(QStyle::CE_PushButton, &button, painter, 0);

}





FlagItemDelegate::FlagItemDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
    setItemEditorFactory(new QItemEditorFactory);
}


QWidget* FlagItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &UNUSED(option), const QModelIndex &UNUSED(index)) const{
    QCheckBox *cb = new QCheckBox(parent);
    cb->setAutoFillBackground(true);
    return cb;
}

void FlagItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const{
    editor->setProperty("checked", index.data(Qt::DisplayRole));
}

void FlagItemDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &UNUSED(index)) const{
    editor->setGeometry(option.rect);
}

void FlagItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const{
    model->setData(index, editor->property("checked"));
}

void FlagItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const{
    //QStyledItemDelegate::paint(painter, option, index);
//    painter->drawText(option.rect, index.data().toString());
    /*QLabel l;
    l.setText(index.data());
    l.setGeometry(option.rect);
    painter->draw*/

    QStyleOptionButton check;
    check.state = check.state & ~QStyle::State_HasFocus;
    check.state |= index.data().toBool() ? QStyle::State_On : QStyle::State_Off;
    check.rect = option.rect;
    check.rect.setWidth(check.rect.height()-1);
    check.rect.setHeight(check.rect.width());
    QApplication::style()->drawPrimitive(QStyle::PE_IndicatorCheckBox, &check, painter);
    return; // EN COURS
    QStyleOptionButton button;
    button.palette = option.palette;
    button.rect = option.rect;
    button.rect.setLeft(option.rect.left()+2);
    button.text = index.data().toString();
    QApplication::style()->drawControl(QStyle::CE_CheckBoxLabel, &button, painter);
    button.palette = option.palette;
    button.rect = option.rect;
    button.rect.setLeft(button.rect.right()-20);
    button.icon = QIcon(":/Icons/edit.png");
    button.iconSize = QSize(16,16);
    QApplication::style()->drawControl(QStyle::CE_PushButton, &button, painter);

}







ObjectNameItemDelegate::ObjectNameItemDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
    setItemEditorFactory(new QItemEditorFactory);
}


QWidget* ObjectNameItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &UNUSED(option), const QModelIndex &UNUSED(index)) const{
    QLineEdit *le = new QLineEdit(parent);
    return le;
}

void ObjectNameItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const{
    editor->setProperty("text", index.data(Qt::EditRole));
}

void ObjectNameItemDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &UNUSED(index)) const{
    editor->setGeometry(option.rect);
}

void ObjectNameItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const{
    model->setData(index, editor->property("text"));
}



