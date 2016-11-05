#include "selectiondock.h"

SelectionDock::SelectionDock(MapViewer *mv, QWidget *parent) :
    BDockWidget(parent), mv(mv)
{
    setupUi(this);
    updateSelectionMode();
    selectColor->setColor(mv->mapPainter().selectedCellColor());
    on_autoPreSelect_toggled(autoPreSelect->isChecked());
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
    selectOpacity->setValue(c.alpha());
    if(autoPreSelect->isChecked()){
        preSelectColor->setColor(QColor::fromHsv((c.hue()+340)%360, c.saturation(), c.value(), c.alpha()));
        preSelectOpacity->setValue(c.alpha());
    }
    emit gameModified();
}

void SelectionDock::on_selectOpacity_valueChanged(int v){
    const QColor &c(selectColor->color());
    selectColor->setColor(QColor(c.red(),c.green(),c.blue(), v));
    if(autoPreSelect->isChecked())
        preSelectOpacity->setValue(c.alpha());
}

void SelectionDock::on_autoPreSelect_toggled(bool b){
    preSelectColor->setEnabled(!b);
    preSelectOpacity->setEnabled(!b);
    label_4->setEnabled(!b);
    label_5->setEnabled(!b);
    if(b) on_selectColor_colorChanged(selectColor->color());
}

void SelectionDock::on_preSelectColor_colorChanged(const QColor &c){
    mv->mapPainter().setPreSelectedCellColor(c);
    preSelectOpacity->setValue(c.alpha());
    emit gameModified();
}

void SelectionDock::on_preSelectOpacity_valueChanged(int v){
    const QColor &c(preSelectColor->color());
    preSelectColor->setColor(QColor(c.red(),c.green(),c.blue(), v));
}
