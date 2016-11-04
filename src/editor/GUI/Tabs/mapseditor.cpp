#include "mapseditor.h"

MapsEditor::MapsEditor(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
    connect(mapViewer, SIGNAL(viewSizeChanged(QSize)), this, SLOT(viewSizeChanged(QSize)));
    connect(&mapViewer->mapPainter(), SIGNAL(mapSizeChanged(QSize)), this, SLOT(mapSizeChanged(QSize)));
    connect(&mapViewer->mapPainter(), SIGNAL(viewCenterChanged(QPoint)), this, SLOT(viewCenterChanged(QPoint)));

    SelectionDock *sd = new SelectionDock;
    docksW.append(sd);
    docks->addDock(tr("Selection options"), sd);
    connect(sd, SIGNAL(gameModified()), this, SLOT(updateGame()));
    MapDock *md = new MapDock;
    docksW.append(md);
    docks->addDock(tr("Map"), md);
    connect(md, SIGNAL(gameModified()), this, SLOT(updateGame()));
    CellDock *cd = new CellDock;
    connect(mapViewer, SIGNAL(selectionChanged()), cd, SLOT(selectionChanged()));
    docksW.append(cd);
    docks->addDock(tr("Cell (0 selected)"), cd);
    connect(cd, SIGNAL(gameModified()), this, SLOT(updateGame()));
    CellTypesDock *ctd = new CellTypesDock;
    docksW.append(ctd);
    docks->addDock(tr("Cell types"), ctd);
    connect(ctd, SIGNAL(gameModified()), this, SLOT(updateGame()));

}

void MapsEditor::setGame(Game *g){
    game = g;
    currentMap = g->world()->maps().first();
    mapViewer->setMap(currentMap);
    for(BDockWidget *d : docksW)
        d->setGame(g);
}

void MapsEditor::updateGame(){
    mapViewer->updateRequest();
    for(BDockWidget *d : docksW)
        d->updateGame();
}

void MapsEditor::mapSizeChanged(QSize s){
    mapHScrollBar->setMaximum(std::max(0,s.width() - mapViewer->width()));
    mapVScrollBar->setMaximum(std::max(0,s.height() - mapViewer->height()));
    checkScrollBarUtility();
}

void MapsEditor::viewCenterChanged(QPoint p){
    mapHScrollBar->setSliderPosition(p.x());
    mapVScrollBar->setSliderPosition(p.y());

}

void MapsEditor::viewSizeChanged(QSize s){
    mapHScrollBar->setPageStep(s.width());
    mapVScrollBar->setPageStep(s.height());
    checkScrollBarUtility();
}

void MapsEditor::checkScrollBarUtility(){
    // TODO : Option
    mapHScrollBar->setVisible(mapHScrollBar->maximum());
    mapVScrollBar->setVisible(mapVScrollBar->maximum());
}

void MapsEditor::on_mapHScrollBar_valueChanged(int){
    if(mapHScrollBar->underMouse()) updateViewCenterPosition();
}


void MapsEditor::on_mapVScrollBar_valueChanged(int){
    if(mapVScrollBar->underMouse()) updateViewCenterPosition();
}

void MapsEditor::updateViewCenterPosition(){
    mapViewer->mapPainter().setViewCenterQuiet(
                (double) mapHScrollBar->value()/mapHScrollBar->maximum(),
                (double) mapVScrollBar->value()/mapVScrollBar->maximum());
    mapViewer->updateRequest();
}
