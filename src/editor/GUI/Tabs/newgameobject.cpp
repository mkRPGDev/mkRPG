#include "newgameobject.h"

NewGameObject::NewGameObject(Game &g, QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    types = new ObjectsTreeModel(&g.world().types(), this);
    preciseTypes = new ObjectsTreeModel(&g.world().types(), this);
    preciseType = new FilteredObjectsTreeModel(this);
    allowedType = new FilteredObjectsTreeModel(this);

    preciseType->setSourceModel(preciseTypes);
    preciseType->setDisplayedItem(-1);

    allowedType->setSourceModel(types);
    allowedType->setDisplayedItem(0);
    allowedType->setMode(false);

    treeView->setModel(allowedType);
    treeView->setRootIndex(QModelIndex());
    treeView->setColumnHidden(0, true);

    treeView_2->setModel(preciseType);
    treeView_2->setColumnHidden(1, true);
    treeView_2->expandAll();

    connect(treeView->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this, SLOT(typeChanged(QModelIndex)));
    connect(treeView_2->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this, SLOT(preciseTypeChanged(QModelIndex)));


    buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}



void NewGameObject::on_gameObject_toggled(bool t){
    allowedType->setDisplayedItem(-!t);
    preciseType->setDisplayedItem(-1);
    buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}


void NewGameObject::typeChanged(const QModelIndex &ty){
    title->setText(ty.data().toString());
    preciseType->setDisplayedItem(ty.row()+gameObject->isChecked());
    treeView_2->expandAll();
    buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}

void NewGameObject::preciseTypeChanged(const QModelIndex &ty){
    if(ty.isValid()){
        buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
        selType = ty.data(Qt::UserRole).toInt();
    }
}

int NewGameObject::selectedType() const{
    return selType;
}

bool NewGameObject::createObject() const{
    return gameObject->isChecked();
}
