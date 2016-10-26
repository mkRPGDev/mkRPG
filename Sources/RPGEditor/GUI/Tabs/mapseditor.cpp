#include "mapseditor.h"

MapsEditor::MapsEditor(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
}

void MapsEditor::setGame(Game *g){
    mapViewer->setMap(g->world()->maps().first());
}
