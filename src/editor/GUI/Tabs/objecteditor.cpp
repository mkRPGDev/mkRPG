#include "objecteditor.h"






ObjectEditor::ObjectEditor(QWidget *parent) :
    TabWidget(parent)
{
    setupUi(this);

    //params->header()->setStretchLastSection(true);
    params->header()->setStretchLastSection(true);
    flags->header()->setStretchLastSection(true);
    params->setItemDelegateForColumn(1, new ParamItemDelegate(this));
    objects->setItemDelegateForColumn(1, new ObjectNameItemDelegate(this));
    paramsModel = new ParamTreeItemModel(this);
    flagsModel = new FlagTreeItemModel(this);
    objectsModel = new ObjectsTreeModel(this);
    typesModel = new TypeItemModel(this);

    params->setModel(paramsModel);
    flags->setModel(flagsModel);
    objects->setModel(objectsModel);
    objects->setColumnWidth(0, objects->width()/2);
    types->setModel(typesModel);

    connect(objects->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this, SLOT(currentElementChanged(QModelIndex)));
//    params->horizontalHeader()->setModel(paramHeader);
//    params->verticalHeader()->setModel(paramHeader);
}

void ObjectEditor::setGame(Game *g){
    paramsModel->setObject(nullptr);
    flagsModel->setObject(nullptr);
    objectsModel->setGame(g);
    typesModel->setGame(g);
    int l = objectsModel->rowCount(QModelIndex());
    for(int i(0); i<l; ++i) objects->expand(objectsModel->index(i,0,QModelIndex()));
    objects->resizeColumnToContents(0);
}

void ObjectEditor::currentElementChanged(const QModelIndex &ind){
    GameObject *o = static_cast<GameObject*>(ind.internalPointer());
    paramsModel->setObject(o);
    flagsModel->setObject(o);
    if(dynamic_cast<InheritableObject*>(o) != nullptr){
        for(int i(0); i<paramsModel->rowCount(QModelIndex()); ++i){
            params->setFirstColumnSpanned(i, QModelIndex(), true);
            params->setExpanded(paramsModel->index(i,0,QModelIndex()), true);
        }
        for(int i(0); i<flagsModel->rowCount(QModelIndex()); ++i){
            flags->setFirstColumnSpanned(i, QModelIndex(), true);
            flags->setExpanded(flagsModel->index(i,0,QModelIndex()), true);
        }
    }
//    flags->update(QModelIndex());
    //params->update(flagsModel->index(0,0));
    //flags->update(flagsModel->index(0,0));

}
