#include "entitytypeeditor.h"

EntityTypeEditor::EntityTypeEditor(QWidget *parent) :
    GameObjectEditor(parent)
{
    setupUi(this);
    imModel =  new ImageListModel(this);
    im->setModel(imModel);
}


EntityTypeEditor::EntityTypeEditor(EntityType &t, QWidget *parent) :
    EntityTypeEditor(parent)
{
    setEntityType(t);
}

void EntityTypeEditor::setEntityType(EntityType &o){
    entityType = &o;
    imModel->setGame(o.getGame());
    im->setCurrentIndex(imModel->getIndex(o.image()));
    if(o.image())
        imView->setPixmap(QPixmap::fromImage(o.image()->image().scaled(128,128)));
    else{
        QPixmap p(128,128);
        p.fill(QColor(0,0,0,0));
        imView->setPixmap(p);
    }
}


void EntityTypeEditor::on_im_currentIndexChanged(int i){
    entityType->setImage(imModel->getImage(i));
    entityType->touch();
}
