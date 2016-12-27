#include "objecttab.h"






ObjectTab::ObjectTab(QWidget *parent) :
    TabWidget(parent), currentObject(nullptr)
{
    setupUi(this);

    objects->setItemDelegateForColumn(1, new ObjectNameItemDelegate(this));
    paramsModel = new ParamTreeItemModel(this);
    flagsModel = new FlagTreeItemModel(this);
    objectsModel = new ObjectsTreeModel(this);
    typesModel = new TypeItemModel2(this);

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

void ObjectTab::setGame(Game *g){
    paramsModel->setObject(nullptr);
    flagsModel->setObject(nullptr);
    objectsModel->setGame(g);
    typesModel->setGame(g);
    params->expandView();
    flags->expandView();
    objects->expandAll();
    objects->resizeColumnToContents(0);

    types->expandAll();
}


void ObjectTab::currentElementChanged(const QModelIndex &ind){
    currentObject = static_cast<GameObject*>(ind.internalPointer());
    newParam->setEnabled(currentObject != nullptr);
    newFlag->setEnabled(currentObject != nullptr);
    paramsModel->setObject(currentObject);
    flagsModel->setObject(currentObject);
    if(dynamic_cast<InheritableObject*>(currentObject) != nullptr){
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


void ObjectTab::on_newParam_clicked(){
    if(currentObject != nullptr){
        QList<QString> p(currentObject->params());
        QString name(tr("new_param"));
        if(p.contains(name)){
            int k(1);
            while(p.contains(name+" ("+QString::number(++k)+")"));
            name+=" ("+QString::number(k)+")";
        }
        paramsModel->addParam(name);
    }
}


void ObjectTab::on_newFlag_clicked(){
    if(currentObject != nullptr){
        QList<QString> p(currentObject->flags());
        QString name(tr("new_flag"));
        if(p.contains(name)){
            int k(1);
            while(p.contains(name+" ("+QString::number(++k)+")"));
            name+=" ("+QString::number(k)+")";
        }
        flagsModel->addFlag(name);
    }
}
