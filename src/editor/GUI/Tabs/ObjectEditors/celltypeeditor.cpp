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
    lineEdit->setText(t.name());
    imModel->setGame(t.getGame());
    bg->setCurrentIndex(imModel->getIndex(t.image()));
}


void CellTypeEditor::on_bg_currentIndexChanged(int i){
    ct->setImage(imModel->getImage(i));
    ct->touch();
}
