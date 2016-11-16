#include "mapdock.h"

MapDock::MapDock(QWidget *parent) :
    BDockWidget(parent)
{
    setupUi(this);
}

void MapDock::on_angleX_valueChanged(int i){
    if(currentMap == nullptr) return;
    currentMap->setAngleX(i);
    vAngleX->setText(QString::number(i/10.)+"°");
    emit gameModified();
}

void MapDock::on_angleY_valueChanged(int i){
    if(currentMap == nullptr) return;
    currentMap->setAngleY(i);
    vAngleY->setText(QString::number(i/10.)+"°");
    emit gameModified();
}

void MapDock::updateGame(){
    currentMap = game->currentMap();
    if(currentMap == nullptr) return;
    angleX->setValue(currentMap->angleX());
    angleY->setValue(currentMap->angleY());
    mapWidth->setValue(currentMap->width());
    mapHeight->setValue(currentMap->height());
}
