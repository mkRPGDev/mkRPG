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
    if(map == nullptr || ang->hasFocus() || etir->hasFocus() || angleX->hasFocus() || angleY->hasFocus()) return;
    mtModel->setGame(map->getGame());
    mt->setCurrentIndex(mtModel->indexOf(&map->mapType()));
    angleX->setValue(map->angleX());
    angleY->setValue(map->angleY());
    mapWidth->setValue(map->width());
    mapHeight->setValue(map->height());
    updateParam();
}


void MapEditor::on_resizing_clicked(){
    MapResizeDialog m(map->width(), map->height(), this);
    if(m.exec()){
        map->resize(m.newRectangle());
        emit mapModified();
    }
}

void MapEditor::on_angleX_valueChanged(int i){
    vAngleX->setText(QString::number(i/10.)+"°");
    if(map == nullptr || !angleX->hasFocus()) return;
    map->setAngleX(i);
    updateParam();
    emit mapModified();
}

void MapEditor::on_angleY_valueChanged(int i){
    vAngleY->setText(QString::number(i/10.)+"°");
    if(map == nullptr || !angleY->hasFocus()) return;
    map->setAngleY(i);
    updateParam();
    emit mapModified();
}

void MapEditor::on_ang_valueChanged(int i){
    vAng->setText(QString::number(i/10.)+"°");
    if(ang->hasFocus())
        updateAngles();
}
void MapEditor::on_etir_valueChanged(int i){
    vEtir->setText(QString::number(i-100)+"%");
    if(etir->hasFocus())
        updateAngles();
}

void MapEditor::updateAngles(){
    angleX->setValue(((100-abs(etir->value()-100))*ang->value()+900*std::max(0,etir->value()-100))/100);
    map->setAngleX(angleX->value());
    angleY->setValue(((100-abs(etir->value()-100))*ang->value()+900*std::max(0,100-etir->value()))/100);
    map->setAngleY(angleY->value());
    emit mapModified();
}

void MapEditor::updateParam(){
    int a = 100*(angleX->value()-angleY->value())/900;
    etir->setValue(a+100);
    if(abs(a) != 100)
        ang->setValue((100*(angleX->value()+angleY->value())- 900*abs(a))/(2*(100-abs(a))));
}
