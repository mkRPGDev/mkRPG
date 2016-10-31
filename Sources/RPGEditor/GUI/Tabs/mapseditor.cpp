#include "mapseditor.h"

MapsEditor::MapsEditor(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
    connect(mapViewer, SIGNAL(viewSizeChanged(QSize)), this, SLOT(viewSizeChanged(QSize)));
    connect(&mapViewer->mapPainter(), SIGNAL(mapSizeChanged(QSize)), this, SLOT(mapSizeChanged(QSize)));
    connect(&mapViewer->mapPainter(), SIGNAL(viewCenterChanged(QPoint)), this, SLOT(viewCenterChanged(QPoint)));
}

void MapsEditor::setGame(Game *g){
    game = g;
    currentMap = g->world()->maps().first();
    mapViewer->setMap(currentMap);
    cellTypes->setModel(new CellTypeListModel(g->world(), this));

}

void MapsEditor::on_angleX_valueChanged(int i){
    currentMap->setAngleX(i);
    mapViewer->updateMap();
}

void MapsEditor::on_angleY_valueChanged(int i){
    currentMap->setAngleY(i);
    mapViewer->updateMap();
}

void MapsEditor::mapSizeChanged(QSize s){
    mapHScrollBar->setMaximum(std::max(0,s.width() - mapViewer->width()));
    mapVScrollBar->setMaximum(std::max(0,s.height() - mapViewer->height()));
    checkScrollBarUtility();
}

void MapsEditor::viewCenterChanged(QPoint p){
    mapHScrollBar->setSliderPosition(p.x());
    mapVScrollBar->setSliderPosition(p.y());

}

void MapsEditor::viewSizeChanged(QSize s){
    mapHScrollBar->setPageStep(s.width());
    mapVScrollBar->setPageStep(s.height());
    checkScrollBarUtility();
}

void MapsEditor::checkScrollBarUtility(){
    // TODO : Option
    mapHScrollBar->setVisible(mapHScrollBar->maximum());
    mapVScrollBar->setVisible(mapVScrollBar->maximum());
}

void MapsEditor::on_mapHScrollBar_valueChanged(int){
    if(mapHScrollBar->underMouse()) updateViewCenterPosition();
}


void MapsEditor::on_mapVScrollBar_valueChanged(int){
    if(mapVScrollBar->underMouse()) updateViewCenterPosition();
}

void MapsEditor::updateViewCenterPosition(){
    mapViewer->mapPainter().setRelativeCenterPosition(
                (double) mapHScrollBar->value()/mapHScrollBar->maximum(),
                (double) mapVScrollBar->value()/mapVScrollBar->maximum());
    mapViewer->updateRequest();
}
