#include "mappainter.h"




#define CELL_WIDTH 64




MapPainter::MapPainter(QObject *parent) : QObject(parent),
    map(nullptr), displayed(All), pWidth(42), pHeight(42), nbCellsX(0),
    mapWidth(1), mapHeight(1), selCellX(-1), selCellY(-1),
    angleX(0), angleY(0), centerX(.5), centerY(-5),
    viewScale(1), intersec(new PtCoords[1]), changesOccured(true)
{

}

MapPainter::MapPainter(Map *m, QObject *parent) :
    MapPainter(parent)
{
    setMap(m);
}



PtCoords MapPainter::pxlToPt(PxCoords p) const{
    return PtCoords((p.x()-pWidth/2)/ratioFactor/viewScale+centerX*mapWidth,
                    mapHeight-(p.y()-pHeight/2)/ratioFactor/viewScale-centerY*mapHeight);
}
PxCoords MapPainter::ptToPxl(PtCoords p) const{
    return PxCoords((p.x()-centerX*mapWidth)*viewScale*ratioFactor+pWidth/2.,
                    (mapHeight-p.y() - centerY*mapHeight)*viewScale*ratioFactor+pHeight/2.);
}
ClCoords MapPainter::ptToCoo(PtCoords p) const{
    double d = cos(angleX)*sin(angleY)-sin(angleX)*cos(angleY);
    double x = p.x() + (nbCellsY+4)*cos(angleY)*CELL_WIDTH;
    return ClCoords((sin(angleY)*x -cos(angleY)*p.y())/d/CELL_WIDTH-2,
                    (-sin(angleX)*x +cos(angleX)*p.y())/d/CELL_WIDTH-2);
}
PtCoords MapPainter::cooToPt(ClCoords p) const{
    return PtCoords(((p.x()+2.)*cos(angleX) - (nbCellsY+2-p.y())*cos(angleY))*CELL_WIDTH,
                    ((p.x()+2.)*sin(angleX) + (p.y()+2)*sin(angleY))*CELL_WIDTH);
}
PtCoords MapPainter::indToPt(int i, int j) const{
    return intersec[i+(nbCellsX+1)*j];
}
PxCoords MapPainter::cooToPxl(ClCoords p) const{
    return ptToPxl(cooToPt(p));
}
ClCoords MapPainter::pxlToCoo(PxCoords p) const{
    return ptToCoo(pxlToPt(p));
}




void MapPainter::updateViewParameters(){
    if(map==nullptr) return;
    globalViewChanged();
    double nf(std::min((double)pWidth/mapWidth,
                       (double)pHeight/mapHeight));
    if(nf != ratioFactor) changeBackgroundSize();
    ratioFactor = nf;
    double fWidth = ratioFactor*mapWidth*viewScale;
    double fHeight = ratioFactor*mapHeight*viewScale;
    centerVarX = std::max(0.,(1-pWidth/fWidth)/2);
    centerVarY = std::max(0.,(1-pHeight/fHeight)/2);
    centerX = MINMAX(.5-centerVarX,centerX,.5+centerVarX);
    centerY = MINMAX(.5-centerVarY,centerY,.5+centerVarY);
    for(int i(0); i<=nbCellsX; ++i)
        for(int j(0); j<=nbCellsY; ++j)
            intersec[i+(nbCellsX+1)*j] = cooToPt(ClCoords(i,j));
    cellSize = QSize((-ptToPxl(indToPt(0,1))+ptToPxl(indToPt(1,0))).x()+1.,
                     (-ptToPxl(indToPt(1,1))+ptToPxl(indToPt(0,0))).y()+1.);
    emit viewCenterChanged(QPoint(fWidth*centerX + .5 - pWidth/2., fHeight*centerY + .5 - pHeight/2.));
    emit mapSizeChanged(QSize(fWidth + .5, fHeight + .5));
}


void MapPainter::updateMap(){
    if(map == nullptr || lastMapUpdate>map->lastModification()) return;
    lastMapUpdate = QDateTime::currentDateTime();
    double angX = M_PI*map->angleX()/1800.;
    double angY = M_PI*(std::min(900+map->angleY(), 1800))/1800.;
    isometricTransform.setMatrix(-cos(angX), +sin(angX),
                                 -cos(angY), +sin(angY),
                                 0,0);
    if(angX != angleX || angY != angleY) changeBackgroundDistortion();
    angleX = angX;
    angleY = angY;
    if(nbCellsX != map->width() || nbCellsY != map->height()){
        nbCellsX = map->width();
        nbCellsY = map->height();
        delete[] intersec;
        intersec = new PtCoords[(nbCellsX+1)*(nbCellsY+1)];
    }
    mapWidth = cooToPt(ClCoords(nbCellsX+2, -2)).x()+.5;
    mapHeight = cooToPt(ClCoords(nbCellsX+2, nbCellsY+2)).y()+.5;
    updateViewParameters();
    QTimer::singleShot(10, this, SLOT(updateMap()));
}

void MapPainter::setMap(Map* m){
    map = m;
    changeBackgroundDistortion();
    lastMapUpdate = m->lastModification();
}



void MapPainter::resize(QSize s){
    resize(s.width(), s.height());
}

void MapPainter::resize(int w, int h){
    pWidth = w;
    pHeight = h;
    im = QImage(w,h,QImage::Format_ARGB32_Premultiplied);
    updateViewParameters();
}

QImage& MapPainter::getBackground(CellType *ct){
    assert(ct != nullptr);
    int id = ct->ident();
    if(scaledCellBackgrounds.contains(id))
        return scaledCellBackgrounds[id];
    if(!cellBackgrounds.contains(id))
        cellBackgrounds[id] = ct->image()->image().transformed(isometricTransform,Qt::SmoothTransformation);
    scaledCellBackgrounds[id] = cellBackgrounds[id].scaled(cellSize,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    return scaledCellBackgrounds[id];
}


void MapPainter::globalViewChanged(){
    changesOccured = true;
}

void MapPainter::updateBackground(){
    updateMap();
    if(!changesOccured) return;
    changesOccured = false;
    im.fill(0);
    QPainter p(&im);
    iMin = std::max(0.,ptToCoo(pxlToPt(PxCoords(0,pHeight))).x());
    iMax = std::min(nbCellsX+0.,ptToCoo(pxlToPt(PxCoords(pWidth,0))).x()+1);
    jMin = std::max(0.,ptToCoo(pxlToPt(PxCoords(pWidth,pHeight))).y());
    jMax = std::min(nbCellsY+0.,ptToCoo(pxlToPt(PxCoords(0,0))).y()+1);

    CellType *ct;
    for(int i(iMax); i-->iMin;)
        for(int j(jMax); j-->jMin;){
            ct = map->cell(i,j).cellType();
            if(ct) p.drawImage(ptToPxl(indToPt(i,j+1)).x(), ptToPxl(indToPt(i+1,j+1)).y(), getBackground(ct));
        }
    if(displayed & Grid){
        p.setPen(QColor(80,80,80));
        p.setBrush(Qt::NoBrush);
        for(int i(iMin); i<=iMax; ++i)
            p.drawLine(ptToPxl(indToPt(i,jMin)),
                       ptToPxl(indToPt(i,jMax)));
        for(int j(jMin); j<=jMax; ++j)
            p.drawLine(ptToPxl(indToPt(iMin,j)),
                       ptToPxl(indToPt(iMax,j)));
    }
}


void MapPainter::paint(QPainter &p){
    if(map == nullptr) return;
    updateBackground();
    p.drawImage(0,0,im);
    QBrush b2(QColor(255,180,0,60));
    p.setPen(QColor(80,80,80, 0));
    p.setBrush(b2);
    for(int i(iMax); i-->iMin;)
        for(int j(jMax); j-->jMin;){
            if(map->cell(i,j).isSelected())
                p.drawConvexPolygon(QVector<QPointF>({ptToPxl(indToPt(i,j)),
                                                      ptToPxl(indToPt(i,j+1)),
                                                      ptToPxl(indToPt(i+1,j+1)),
                                                      ptToPxl(indToPt(i+1,j))}));
        }
    p.setBrush(Qt::NoBrush);
    if(0<=selCellX && selCellX<nbCellsX && 0<=selCellY && selCellY<=nbCellsY){
        p.setPen(QColor(255,255,20));
        p.drawPolygon(QVector<QPointF>({ptToPxl(indToPt(selCellX,selCellY)),
                                        ptToPxl(indToPt(selCellX,selCellY+1)),
                                        ptToPxl(indToPt(selCellX+1,selCellY+1)),
                                        ptToPxl(indToPt(selCellX+1,selCellY))}));
    }
}

const QImage& MapPainter::render(){
    if(map == nullptr) return QImage();
    updateBackground(); // TODO pas que le fond...
    return im;
}




void MapPainter::setScale(double scale){
    if(viewScale != scale) changeBackgroundSize();
    viewScale = scale;
    globalViewChanged();
}


double MapPainter::scale()const{
    return viewScale;
}



void MapPainter::setViewCenter(RlCoords relativeCenter){
    setViewCenter(relativeCenter.x(), relativeCenter.y());
}

void MapPainter::setViewCenter(double relativeCenterX, double relativeCenterY){
    centerX = MINMAX(.5-centerVarX, relativeCenterX, .5+centerVarX);
    centerY = MINMAX(.5-centerVarY, relativeCenterY, .5+centerVarY);
    globalViewChanged();
    emit viewCenterChanged(QPoint(ratioFactor*mapWidth*viewScale*centerX + .5 - pWidth/2.,
                                  ratioFactor*mapHeight*viewScale*centerY + .5 - pHeight/2.));
}

RlCoords MapPainter::viewCenter() const{
    return RlCoords(centerX, centerY);
}

void MapPainter::setViewCenterQuiet(double x, double y){
    centerX = .5 + (2*MINMAX(0.,x,1.) - 1)*centerVarX;
    centerY = .5 + (2*MINMAX(0.,y,1.) - 1)*centerVarY;
    globalViewChanged();
}

bool MapPainter::setHighlightedCell(const ClCoords& p){
    return setHighlightedCell(floor(p.x()), floor(p.y()));
}

bool MapPainter::setHighlightedCell(int i, int j){
    if(0<=i && i<nbCellsX && 0<=j && j<nbCellsY){
        selCellX = i;
        selCellY = j;
    }
    else{
        selCellX = -1;
        selCellY = -1;
    }
    return hasHighlightedCell();
}

QPoint MapPainter::highlightedCell() const{
    return QPoint(selCellX, selCellY);
}

bool MapPainter::hasHighlightedCell() const{
    return selCellX+1 && selCellY+1;
}

void MapPainter::zoom(double factor, QPointF fixedPoint){
    QPointF d = fixedPoint-QPointF(pWidth/2.,pHeight/2.);
    factor = MINMAX(1/viewScale, factor,40./viewScale);
    d.setX(d.x()/mapWidth);
    d.setY(d.y()/mapHeight);
    changeBackgroundSize();
    viewScale *= factor; // NOTE ici ?
    //qDebug() << viewScale;
    updateViewParameters();
    setViewCenter(RlCoords(viewCenter()-(1-factor)*d/viewScale/ratioFactor));
}

QPair<bool,bool> MapPainter::move(PxCoords delta, QPointF center){
    double newCX = center.x() - delta.x()/viewScale/ratioFactor/mapWidth;
    double newCY = center.y() - delta.y()/viewScale/ratioFactor/mapHeight;
    setViewCenter(newCX, newCY);
    return QPair<bool,bool>(centerX == newCX, centerY == newCY);
}


void MapPainter::changeBackgroundDistortion(){
    changeBackgroundSize();
    cellBackgrounds.clear();
}
void MapPainter::changeBackgroundSize(){
    scaledCellBackgrounds.clear();
    globalViewChanged();
}

void MapPainter::setPaintedElements(Element e){
    displayed = e;
    globalViewChanged();
}

void MapPainter::setPaintedElement(MapPainter::Element e, bool painted){
    setPaintedElements(painted ? displayed | e : displayed ^ e);
}
