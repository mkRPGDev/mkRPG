#include "celltypesdock.h"

CellTypesDock::CellTypesDock(QWidget *parent) :
    BDockWidget(parent)
{
    setupUi(this);
}


void CellTypesDock::updateGame(){
    cellTypes->setModel(game ? new CellTypeListModel(game->world(), this) : nullptr);
}
