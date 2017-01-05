#include "celltypeeditor.h"

CellTypeEditor::CellTypeEditor(QWidget *parent) :
    GameObjectEditor(parent)
{
    setupUi(this);

    imModel = new ImageListModel(this);
    bg->setModel(imModel);
}


CellTypeEditor::CellTypeEditor(CellType &t, QWidget *parent) :
    CellTypeEditor(parent)
{
    setCellType(t);
}

void CellTypeEditor::setCellType(CellType &t){
    ct = &t;
    imModel->setGame(t.getGame());
    bg->setCurrentIndex(imModel->getIndex(t.image()));
    if(t.image())
        bgView->setPixmap(QPixmap::fromImage(t.image()->image().scaled(128,128)));
    else{
        QPixmap p(128,128);
        p.fill(QColor(0,0,0,0));
        bgView->setPixmap(p);
    }
}


void CellTypeEditor::on_bg_currentIndexChanged(int i){
    ct->setImage(imModel->getImage(i));
    ct->touch();
}
