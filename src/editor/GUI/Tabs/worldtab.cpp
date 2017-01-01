#include "worldtab.h"

WorldTab::WorldTab(QWidget *parent) :
    TabWidget(parent)
{
    setupUi(this);
    maps = new MapsListModel(this);
    mapsView->setModel(maps);
}


void WorldTab::setGame(Game *g){
    game = g;
    updateGame();
}

void WorldTab::updateGame(){
    maps->setWorld(game->world());
}



WorldTab::~WorldTab(){
    delete game;
}

void WorldTab::on_mapsView_doubleClicked(const QModelIndex &index){
    game->setCurrentMap(game->world().objects().maps().at(index.row()));
    emit editMap();
}
