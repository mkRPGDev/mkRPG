#include "mapseditor.h"

MapsEditor::MapsEditor(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
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
