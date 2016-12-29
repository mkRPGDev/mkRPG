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
    signalsModel = new SignalTreeItemModel(this);
    slotsModel = new SlotTreeItemModel(this);

    params->setModel(paramsModel);
    flags->setModel(flagsModel);
    signalsV->setModel(signalsModel);
    slotsV->setModel(slotsModel);
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
    signalsModel->setObject(nullptr);
    slotsModel->setObject(nullptr);
    objectsModel->setGameObject(g);
    typesModel->setGame(g);
    params->expandView();
    flags->expandView();
    signalsV->expandView();
    slotsV->expandView();
    objects->expandAll();
    objects->resizeColumnToContents(0);

    types->expandAll();
}


void ObjectTab::currentElementChanged(const QModelIndex &ind){
    currentObject = static_cast<GameObject*>(ind.internalPointer());
    newParam->setEnabled(currentObject != nullptr);
    newFlag->setEnabled(currentObject != nullptr);
    newSignal->setEnabled(currentObject != nullptr);
    newSlot->setEnabled(currentObject != nullptr);
    paramsModel->setObject(currentObject);
    flagsModel->setObject(currentObject);
    signalsModel->setObject(currentObject);
    slotsModel->setObject(currentObject);
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
        for(int i(0); i<signalsModel->rowCount(QModelIndex()); ++i){
            signalsV->setFirstColumnSpanned(i, QModelIndex(), true);
            signalsV->setExpanded(signalsModel->index(i,0,QModelIndex()), true);
        }
        for(int i(0); i<slotsModel->rowCount(QModelIndex()); ++i){
            slotsV->setFirstColumnSpanned(i, QModelIndex(), true);
            slotsV->setExpanded(slotsModel->index(i,0,QModelIndex()), true);
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
            while(currentObject->hasParam(name+" ("+QString::number(++k)+")"));
            name+=" ("+QString::number(k)+")";
        }
        paramsModel->addParam(name);
    }
}


void ObjectTab::on_newFlag_clicked(){
    if(currentObject != nullptr){
        QString name(tr("new_flag"));
        if(currentObject->hasFlag(name)){
            int k(1);
            while(currentObject->hasFlag(name+" ("+QString::number(++k)+")"));
            name+=" ("+QString::number(k)+")";
        }
        flagsModel->addFlag(name);
    }
}


void ObjectTab::on_newSignal_clicked(){
    if(currentObject != nullptr){
        QString name(tr("new_signal"));
        if(currentObject->hasSignal(name)){
            int k(1);
            while(currentObject->hasSignal(name+" ("+QString::number(++k)+")"));
            name+=" ("+QString::number(k)+")";
        }
        signalsModel->addSignal(name);
    }
}


void ObjectTab::on_newSlot_clicked(){
    if(currentObject != nullptr){
        QString name(tr("new_slot"));
        if(currentObject->hasSlot(name)){
            int k(1);
            while(currentObject->hasSlot(name+" ("+QString::number(++k)+")"));
            name+=" ("+QString::number(k)+")";
        }
        slotsModel->addSlot(name);
    }
}
