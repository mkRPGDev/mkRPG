#include "mapdock.h"

MapDock::MapDock(QWidget *parent) :
    BDockWidget(parent)
{
    setupUi(this);
    connect(editor, SIGNAL(mapModified()), this, SIGNAL(gameModified()));
}



void MapDock::updateGame(){
    currentMap = game->currentMap();
    setEnabled(currentMap != nullptr);
    editor->setMap(currentMap);
}

