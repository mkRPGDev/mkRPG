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



