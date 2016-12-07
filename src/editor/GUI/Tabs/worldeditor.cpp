#include "worldeditor.h"

WorldEditor::WorldEditor(QWidget *parent) :
    TabWidget(parent)
{
    setupUi(this);
}


void WorldEditor::setGame(Game *g){
    game = g;
    mapsView->setModel(new MapsListModel(&g->world(), this));
}

WorldEditor::~WorldEditor(){
    delete game;
}

void WorldEditor::on_mapsView_doubleClicked(const QModelIndex &index){
    game->setCurrentMap(game->world().maps().at(index.row()));
    emit editMap();
}
