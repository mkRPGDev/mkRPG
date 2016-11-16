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
    paramsModel = new ObjectParamTableModel(this);
    flagsModel = new ObjectFlagTableModel(this);
    objectsModel = new ObjectsTreeModel(this);
    params->setModel(paramsModel);
    flags->setModel(flagsModel);
    objects->setModel(objectsModel);
//    params->horizontalHeader()->setModel(paramHeader);
//    params->verticalHeader()->setModel(paramHeader);
}

void ObjectEditor::setGame(Game *g){
    params->setItemDelegateForColumn(1, new ParamItemDelegate(this));
    paramsModel->setObject(g->world());
    flagsModel->setObject(g->world());
    objectsModel->setGame(g);
}

void ObjectEditor::on_objects_clicked(const QModelIndex &ind){
    GameObject *o = static_cast<GameObject*>(ind.internalPointer());
    paramsModel->setObject(o);
    flagsModel->setObject(o);
    flags->update(QModelIndex());
    params->update(flagsModel->index(0,0));
    flags->update(flagsModel->index(0,0));

}
