#include "celltypesdock.h"

CellTypesDock::CellTypesDock(QWidget *parent) :
    BDockWidget(parent)
{
    setupUi(this);
    ctModel = new CellTypeListModel(this);
    cellTypes->setModel(ctModel);
}


void CellTypesDock::updateGame(){
    ctModel->setGame(game);
}
