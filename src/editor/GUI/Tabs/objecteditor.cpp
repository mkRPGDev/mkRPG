#include "objecteditor.h"






ObjectEditor::ObjectEditor(QWidget *parent) :
    TabWidget(parent)
{
    setupUi(this);

    //params->header()->setStretchLastSection(true);
    params->horizontalHeader()->setStretchLastSection(true);
    params->setItemDelegateForColumn(1, new ParamItemDelegate(this));
    flags->horizontalHeader()->setStretchLastSection(true);
    objects->setItemDelegateForColumn(1, new ObjectNameItemDelegate(this));
    paramsModel = new ObjectParamTableModel(this);
    flagsModel = new ObjectFlagTableModel(this);
    objectsModel = new ObjectsTreeModel(this);
    params->setModel(paramsModel);
    flags->setModel(flagsModel);
    objects->setModel(objectsModel);

    connect(objects->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this, SLOT(currentElementChanged(QModelIndex)));
//    params->horizontalHeader()->setModel(paramHeader);
//    params->verticalHeader()->setModel(paramHeader);
}

void ObjectEditor::setGame(Game *g){
    paramsModel->setObject(g->world());
    flagsModel->setObject(g->world());
    objectsModel->setGame(g);
}

void ObjectEditor::currentElementChanged(const QModelIndex &ind){
    GameObject *o = static_cast<GameObject*>(ind.internalPointer());
    paramsModel->setObject(o);
    flagsModel->setObject(o);
    flags->update(QModelIndex());
    params->update(flagsModel->index(0,0));
    flags->update(flagsModel->index(0,0));

}
