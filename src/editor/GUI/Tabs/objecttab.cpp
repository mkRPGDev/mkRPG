#include "objecttab.h"






ObjectTab::ObjectTab(QWidget *parent) :
    TabWidget(parent), currentObject(nullptr)
{
    setupUi(this);

    objects->setItemDelegateForColumn(1, new ObjectNameItemDelegate(this));
    paramsModel = new ParamTreeItemModel(this);
    flagsModel = new FlagTreeItemModel(this);
    objectsModel = new ObjectsTreeModel(this);
    typesModel = new TypeItemModel(this);
    eventsModel = new EventTreeItemModel(this);
    ordersModel = new OrderTreeItemModel(this);

    params->setModel(paramsModel);
    flags->setModel(flagsModel);
    events->setModel(eventsModel);
    orders->setModel(ordersModel);
    objects->setModel(objectsModel);
    objects->setColumnWidth(0, objects->width()/2);
    types->setModel(typesModel);
    edit = new QWidget();
    editor->layout()->addWidget(edit);

    connect(objects->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this, SLOT(currentElementChanged(QModelIndex)));
//    params->horizontalHeader()->setModel(paramHeader);
//    params->verticalHeader()->setModel(paramHeader);
}

void ObjectTab::setGame(Game *g){
    paramsModel->setObject(nullptr);
    flagsModel->setObject(nullptr);
    eventsModel->setObject(nullptr);
    ordersModel->setObject(nullptr);
    objectsModel->setGameObject(g);
    typesModel->setGame(g);
    params->expandView();
    flags->expandView();
    events->expandView();
    orders->expandView();
    objects->expandAll();
    objects->resizeColumnToContents(0);

    types->expandAll();
}


void ObjectTab::currentElementChanged(const QModelIndex &ind){
    currentObject = static_cast<GameObject*>(ind.internalPointer());
    newParam->setEnabled(currentObject != nullptr);
    newFlag->setEnabled(currentObject != nullptr);
    newEvent->setEnabled(currentObject != nullptr);
    newOrder->setEnabled(currentObject != nullptr);
    paramsModel->setObject(currentObject);
    flagsModel->setObject(currentObject);
    eventsModel->setObject(currentObject);
    ordersModel->setObject(currentObject);
    QWidget *newEdit = GameObjectEditor::editor(*currentObject);
    editor->layout()->replaceWidget(edit, newEdit);
    delete edit;
    edit = newEdit;
    if(dynamic_cast<InheritableObject*>(currentObject) != nullptr){
        for(int i(0); i<paramsModel->rowCount(QModelIndex()); ++i){
            params->setFirstColumnSpanned(i, QModelIndex(), true);
            params->setExpanded(paramsModel->index(i,0,QModelIndex()), true);
        }
        for(int i(0); i<flagsModel->rowCount(QModelIndex()); ++i){
            flags->setFirstColumnSpanned(i, QModelIndex(), true);
            flags->setExpanded(flagsModel->index(i,0,QModelIndex()), true);
        }
        for(int i(0); i<eventsModel->rowCount(QModelIndex()); ++i){
            events->setFirstColumnSpanned(i, QModelIndex(), true);
            events->setExpanded(eventsModel->index(i,0,QModelIndex()), true);
        }
        for(int i(0); i<ordersModel->rowCount(QModelIndex()); ++i){
            orders->setFirstColumnSpanned(i, QModelIndex(), true);
            orders->setExpanded(ordersModel->index(i,0,QModelIndex()), true);
        }
    }
//    flags->update(QModelIndex());
    //params->update(flagsModel->index(0,0));
    //flags->update(flagsModel->index(0,0));

}


void ObjectTab::on_newParam_clicked(){
    if(currentObject != nullptr){
        QString name(tr("new_param"));
        if(currentObject->hasParam(name)){
            int k(1);
            while(currentObject->hasParam(name+"_"+QString::number(++k)));
            name+="_"+QString::number(k);
        }
        paramsModel->addParam(name);
    }
}


void ObjectTab::on_newFlag_clicked(){
    if(currentObject != nullptr){
        QString name(tr("new_flag"));
        if(currentObject->hasFlag(name)){
            int k(1);
            while(currentObject->hasFlag(name+"_"+QString::number(++k)));
            name+="_"+QString::number(k);
        }
        flagsModel->addFlag(name);
    }
}


void ObjectTab::on_newEvent_clicked(){
    if(currentObject != nullptr){
        QString name(tr("new_event"));
        if(currentObject->hasEvent(name)){
            int k(1);
            while(currentObject->hasEvent(name+"_"+QString::number(++k)));
            name+="_"+QString::number(k);
        }
        eventsModel->addEvent(name);
    }
}


void ObjectTab::on_newOrder_clicked(){
    if(currentObject != nullptr){
        QString name(tr("new_order"));
        if(currentObject->hasOrder(name)){
            int k(1);
            while(currentObject->hasOrder(name+"_"+QString::number(++k)));
            name+="_"+QString::number(k);
        }
        ordersModel->addOrder(name);
    }
}
