#include "celltypeeditor.h"

CellTypeEditor::CellTypeEditor(QWidget *parent) :
    GameObjectEditor(parent)
{
    setupUi(this);
}


CellTypeEditor::CellTypeEditor(CellType &t, QWidget *parent) :
    CellTypeEditor(parent)
{
    setCellType(t);
}

void CellTypeEditor::setCellType(CellType &t){
    lineEdit->setText(t.name());
}
