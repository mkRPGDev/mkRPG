#include "objecteditor.h"



ParamItemDelegate::ParamItemDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
    setItemEditorFactory(new QItemEditorFactory);
}


QWidget* ParamItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const{
    QSpinBox *sp = new QSpinBox(parent);
    sp->setMaximum(10000);
    return sp;
}

void ParamItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const{
    editor->setProperty("value", index.data(Qt::DisplayRole));
}

void ParamItemDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const{
    editor->setGeometry(option.rect);
}

void ParamItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const{
    model->setData(index, editor->property("value"));
}


ObjectEditor::ObjectEditor(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
}

void ObjectEditor::setGame(Game *g){
    params->setItemDelegateForColumn(1, new ParamItemDelegate(this));
    params->setModel(new ObjectParamTableModel(g->world()->maps().first(),this));
}
