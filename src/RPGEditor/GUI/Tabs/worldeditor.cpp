#include "worldeditor.h"

WorldEditor::WorldEditor(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
}


void WorldEditor::setGame(Game *g){
    game = g;
    mapsView->setModel(new MapsListModel(g->world(), this));
}
