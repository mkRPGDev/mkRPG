#include "maptab.h"

MapsTab::MapsTab(QWidget *parent) :
    TabWidget(parent)
{
    setupUi(this);
    connect(mapViewer, SIGNAL(viewSizeChanged(QSize)), this, SLOT(viewSizeChanged(QSize)));
    connect(&mapViewer->mapPainter(), SIGNAL(mapSizeChanged(QSize)), this, SLOT(mapSizeChanged(QSize)));
    connect(&mapViewer->mapPainter(), SIGNAL(viewCenterChanged(QPoint)), this, SLOT(viewCenterChanged(QPoint)));

    SelectionDock *sd = new SelectionDock(mapViewer);
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
    /*CellTypesDock *ctd = new CellTypesDock;
    docksW.append(ctd);
    docks->addDock(tr("Cell types"), ctd);
    connect(ctd, SIGNAL(gameModified()), this, SLOT(updateGame()));*/

}


void MapsTab::mapChanged(const QModelIndex &to, const QModelIndex &UNUSED(from)){
    game->setCurrentMap(game->world().objects().maps().at(to.row()));
    // TODO mieux
    updateGame();
}

void MapsTab::setGame(Game *g){
    maps = new MapsListModel(g->world(), this);
    mapsView->setModel(maps);


    connect(mapsView->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this, SLOT(mapChanged(QModelIndex,QModelIndex)));
    game = g;
    for(BDockWidget *d : docksW)
        d->setGame(g);
    updateGame();
}


void MapsTab::updateGame(){
    maps->setWorld(game->world());
    mapViewer->setMap(game->currentMap());
    mapViewer->updateRequest();
    for(BDockWidget *d : docksW)
        d->updateGame();
}

void MapsTab::mapSizeChanged(QSize s){
    mapHScrollBar->setMaximum(std::max(0,s.width() - mapViewer->width()));
    mapVScrollBar->setMaximum(std::max(0,s.height() - mapViewer->height()));
    checkScrollBarUtility();
}

void MapsTab::viewCenterChanged(QPoint p){
    mapHScrollBar->setSliderPosition(p.x());
    mapVScrollBar->setSliderPosition(p.y());

}

void MapsTab::viewSizeChanged(QSize s){
    mapHScrollBar->setPageStep(s.width());
    mapVScrollBar->setPageStep(s.height());
    checkScrollBarUtility();
}

void MapsTab::checkScrollBarUtility(){
    // TODO : Option
    mapHScrollBar->setVisible(mapHScrollBar->maximum());
    mapVScrollBar->setVisible(mapVScrollBar->maximum());
}

void MapsTab::on_mapHScrollBar_valueChanged(int){
    if(mapHScrollBar->underMouse()) updateViewCenterPosition();
}


void MapsTab::on_mapVScrollBar_valueChanged(int){
    if(mapVScrollBar->underMouse()) updateViewCenterPosition();
}

void MapsTab::updateViewCenterPosition(){
    mapViewer->mapPainter().setViewCenterQuiet(
                (double) mapHScrollBar->value()/mapHScrollBar->maximum(),
                (double) mapVScrollBar->value()/mapVScrollBar->maximum());
    mapViewer->updateRequest();
}

