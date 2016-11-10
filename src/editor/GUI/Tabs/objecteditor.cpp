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

    //params->header()->setStretchLastSection(true);
    params->horizontalHeader()->setStretchLastSection(true);
    flags->horizontalHeader()->setStretchLastSection(true);

//    params->horizontalHeader()->setModel(paramHeader);
//    params->verticalHeader()->setModel(paramHeader);
}

void ObjectEditor::setGame(Game *g){
    params->setItemDelegateForColumn(1, new ParamItemDelegate(this));
    params->setModel(new ObjectParamTableModel(g->world()->maps().first(),this));
    flags->setModel(new ObjectFlagTableModel(g->world()->maps().first(), this));
}
