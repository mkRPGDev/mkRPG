#include "mapeditor.h"

MapEditor::MapEditor(QWidget *parent) :
    GameObjectEditor(parent)
{
    setupUi(this);
}

MapEditor::MapEditor(Map &map, QWidget *parent) :
    MapEditor(parent)
{
    setMap(map);
}

void MapEditor::setMap(Map &UNUSED(m)){

}
