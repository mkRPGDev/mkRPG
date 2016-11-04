#include "selectiondock.h"

SelectionDock::SelectionDock(MapViewer *mv, QWidget *parent) :
    BDockWidget(parent), mv(mv)
{
    setupUi(this);
    updateSelectionMode();
    selectColor->setColor(mv->mapPainter().selectedCellColor());
}


void SelectionDock::updateSelectionMode(){
    switch (mv->selectionMode()){
    case MapViewer::PencilSelection:
        pencil->setChecked(true);
        break;
    case MapViewer::RectangleSelection:
        rectangle->setChecked(true);
        break;
    case MapViewer::RegionSelection:
        region->setChecked(true);
        break;
    }
}



void SelectionDock::on_pencil_toggled(bool b){
    if(b) mv->setSelectionMode(MapViewer::PencilSelection);
}

void SelectionDock::on_rectangle_toggled(bool b){
    if(b) mv->setSelectionMode(MapViewer::RectangleSelection);
}

void SelectionDock::on_region_toggled(bool b){
    if(b) mv->setSelectionMode(MapViewer::RegionSelection);
}

void SelectionDock::on_selectColor_colorChanged(const QColor &c){
    mv->mapPainter().setSelectedCellColor(c);
    emit gameModified();
}
