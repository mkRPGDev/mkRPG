#include "mapeditor.h"

MapEditor::MapEditor(QWidget *parent) :
    GameObjectEditor(parent)
{
    setupUi(this);
    mtModel = new MapTypeListModel(this);
    mt->setModel(mtModel);
}

MapEditor::MapEditor(Map &map, QWidget *parent) :
    MapEditor(parent)
{
    setMap(&map);
}

void MapEditor::setMap(Map *m){
    setEnabled(m != nullptr);
    map = m;
    if(map == nullptr) return;
    mtModel->setGame(map->getGame());
    mt->setCurrentIndex(mtModel->indexOf(&map->mapType()));
    angleX->setValue(map->angleX());
    angleY->setValue(map->angleY());
    mapWidth->setValue(map->width());
    mapHeight->setValue(map->height());
}


void MapEditor::on_resizing_clicked(){
    MapResizeDialog m(map->width(), map->height(), this);
    if(m.exec()){
        map->resize(m.newRectangle());
        emit mapModified();
    }
}

void MapEditor::on_angleX_valueChanged(int i){
    if(map == nullptr) return;
    map->setAngleX(i);
    vAngleX->setText(QString::number(i/10.)+"°");
    emit mapModified();
}

void MapEditor::on_angleY_valueChanged(int i){
    if(map == nullptr) return;
    map->setAngleY(i);
    vAngleY->setText(QString::number(i/10.)+"°");
    emit mapModified();
}
