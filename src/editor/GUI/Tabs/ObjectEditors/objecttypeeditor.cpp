#include "objecttypeeditor.h"


ObjectTypeEditor::ObjectTypeEditor(QWidget *parent) :
    GameObjectEditor(parent)
{
    setupUi(this);
    imModel =  new ImageListModel(this);
    im->setModel(imModel);
}


ObjectTypeEditor::ObjectTypeEditor(ObjectType &t, QWidget *parent) :
    ObjectTypeEditor(parent)
{
    setObjectType(t);
}

void ObjectTypeEditor::setObjectType(ObjectType &o){
    objectType = &o;
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


void ObjectTypeEditor::on_im_currentIndexChanged(int i){
    objectType->setImage(imModel->getImage(i));
    objectType->touch();
}
