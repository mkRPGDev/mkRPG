#include "worldtab.h"

WorldTab::WorldTab(QWidget *parent) :
    TabWidget(parent)
{
    setupUi(this);
}


void WorldTab::setGame(Game *g){
    game = g;
    mapsView->setModel(new MapsListModel(&g->world(), this));
}

WorldTab::~WorldTab(){
    delete game;
}

void WorldTab::on_mapsView_doubleClicked(const QModelIndex &index){
    game->setCurrentMap(game->world().objects().maps().at(index.row()));
    emit editMap();
}
