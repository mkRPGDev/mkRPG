#include "mapviewer.h"


#define CELL_WIDTH 64




MapPainter::MapPainter() :
    map(nullptr), pWidth(42), pHeight(42), nbCellsX(0),
    mapWidth(1), mapHeight(1), selCellX(-1), selCellY(-1),
    angleX(0), angleY(0), centerX(.5), centerY(-5),
    viewScale(1), intersec(new PtCoords[1]), changesOccured(true)
{

    herbe = QImage("/home/baptiste/herbe256.png").transformed(QMatrix(1,0,0,-1,0,0));

}

MapPainter::MapPainter(Map *m) :
    MapPainter()
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
    centerVarX = std::max(0.,(1-pWidth/ratioFactor/mapWidth/viewScale)/2);
    centerVarY = std::max(0.,(1-pHeight/ratioFactor/mapHeight/viewScale)/2);
    centerX = MINMAX(.5-centerVarX,centerX,.5+centerVarX);
    centerY = MINMAX(.5-centerVarY,centerY,.5+centerVarY);
}


void MapPainter::updateMap(){
    if(map == nullptr) return;
    lastMapUpdate = QDateTime::currentDateTime();
    double angX = M_PI*map->angleX()/1800.;
    double angY = M_PI*(std::min(900+map->angleY(), 1800))/1800.;
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
    for(int i(0); i<=nbCellsX; ++i)
        for(int j(0); j<=nbCellsY; ++j)
            intersec[i+(nbCellsX+1)*j] = cooToPt(ClCoords(i,j));
    isometricTransform.setMatrix(cos(angleX), -sin(angleX),
                                 cos(angleY), -sin(angleY),
                                 0,0);


    herbeR = herbe.transformed(isometricTransform,Qt::SmoothTransformation);
}

void MapPainter::setMap(Map* m){
    map = m;
    updateMap();
}



void MapPainter::resize(QSize s){
    resize(s.width(), s.height());
}

void MapPainter::resize(int w, int h){
    pWidth = w;
    pHeight = h;
    double nf(std::min((double)pWidth/mapWidth,
                       (double)pHeight/mapHeight));
    if(nf != ratioFactor) cellBackgrounds.clear();
    ratioFactor = nf;
    updateViewParameters();
}




void MapPainter::paint(QPainter &p){
    if(map == nullptr) return;
    // look for changes in map
    //qDebug() << ratioFactor << viewScale;
    int iMin = std::max(0.,ptToCoo(pxlToPt(PxCoords(0,pHeight))).x());
    int iMax = std::min(nbCellsX+0.,ptToCoo(pxlToPt(PxCoords(pWidth,0))).x()+1);
    int jMin = std::max(0.,ptToCoo(pxlToPt(PxCoords(pWidth,pHeight))).y());
    int jMax = std::min(nbCellsY+0.,ptToCoo(pxlToPt(PxCoords(0,0))).y()+1);

    QImage hr(herbeR.scaled((-ptToPxl(indToPt(0,1))+ptToPxl(indToPt(1,0))).x()+1,
                            (-ptToPxl(indToPt(1,1))+ptToPxl(indToPt(0,0))).y()+1));
    QBrush b1(QColor(0,0,0,0));
    QBrush b2(QColor(255,180,0,60));
    p.setPen(QColor(80,80,80));
    for(int i(iMax); i-->iMin;)
        for(int j(jMax); j-->jMin;){
            p.setBrush(map->cell(i,j).isSelected() ? b2 : b1);
            // Tester map cellBackgrounds.
            p.drawImage(ptToPxl(indToPt(i,j+1)).x(), ptToPxl(indToPt(i+1,j+1)).y(), hr);
            p.drawConvexPolygon(QVector<QPointF>({ptToPxl(indToPt(i,j)),
                                                  ptToPxl(indToPt(i,j+1)),
                                                  ptToPxl(indToPt(i+1,j+1)),
                                                  ptToPxl(indToPt(i+1,j))}));
        }
    /*p.setPen(QColor(80,80,80));
    p.setBrush(Qt::NoBrush);
    for(int i(0); i<=nbCellX; ++i)
        p.drawLine(ptToPxl(indToPt(i,0)),
                   ptToPxl(indToPt(i,nbCellY)));
    for(int i(0); i<=nbCellY; ++i)
        p.drawLine(ptToPxl(indToPt(0,i)),
                   ptToPxl(indToPt(nbCellX,i)));*/
    if(0<=selCellX && selCellX<nbCellsX && 0<=selCellY && selCellY<=nbCellsY){
        p.setPen(QColor(255,255,20));
        p.drawPolygon(QVector<QPointF>({ptToPxl(indToPt(selCellX,selCellY)),
                                        ptToPxl(indToPt(selCellX,selCellY+1)),
                                        ptToPxl(indToPt(selCellX+1,selCellY+1)),
                                        ptToPxl(indToPt(selCellX+1,selCellY))}));
    }
}




void MapPainter::setScale(double scale){
    if(viewScale != scale) changeBackgroundSize();
    viewScale = scale;
}


double MapPainter::scale()const{
    return viewScale;
}



void MapPainter::setViewCenter(QPointF relativeCenter){
    setViewCenter(relativeCenter.x(), relativeCenter.y());
}

void MapPainter::setViewCenter(double relativeCenterX, double relativeCenterY){
    centerX = MINMAX(.5-centerVarX, relativeCenterX, .5+centerVarX);
    centerY = MINMAX(.5-centerVarY, relativeCenterY, .5+centerVarY);
    changeParameter();
}

QPointF MapPainter::viewCenter() const{
    return QPointF(centerX, centerY);
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
    factor = MINMAX(1./viewScale, factor,10./viewScale);
    d.setX(d.x()/mapWidth);
    d.setY(d.y()/mapHeight);
    changeBackgroundSize();
    viewScale *= factor; // NOTE ici ?
    //qDebug() << viewScale;
    updateViewParameters();
    setViewCenter(viewCenter()-(1-factor)*d/viewScale/ratioFactor);
}

QPair<bool,bool> MapPainter::move(QPoint delta, QPointF center){
    double newCX = center.x() - delta.x()/viewScale/ratioFactor/mapWidth;
    double newCY = center.y() - delta.y()/viewScale/ratioFactor/mapHeight;
    setViewCenter(newCX, newCY);
    return QPair<bool,bool>(centerX == newCX, centerY == newCY);
}


void MapPainter::changeBackgroundDistortion(){
    cellBackgrounds.clear();
    changeBackgroundSize();
}
void MapPainter::changeBackgroundSize(){
    scaledCellBackgrounds.clear();
    changeParameter();
}
void MapPainter::changeParameter(){
    changesOccured = true;
}

















void MapViewer::wheelEvent(QWheelEvent *we){
    if(map == nullptr) return;
    we->accept();
    double r = we->delta()<0 ? .95 : 1.05;
    mp.zoom(pow(r,(double) abs(we->delta())/120), we->pos());

    QTimer::singleShot(0,this, SLOT(update()));
    checkMousePos();
}



void MapViewer::mousePosChecking(){
    QPoint pt(mapFromGlobal(cursor().pos()));
    bool b = mouseIn;
    mouseIn = pt.x()>=0 && pt.x() < wi && pt.y() >=0 && pt.y() < he;
    if(mouseIn){ if(updateMousePos(mp.pxlToPt(pt))) ti->start(100);}
    else if(b) mouseOutEvent();

}

void MapViewer::checkMousePos(){
    if(map == nullptr) return;
    if(!ti->isActive()) ti->start(100);
    else ti->setInterval(100);
    mousePosChecking();
}


void MapViewer::resizeEvent(QResizeEvent *re){
    QWidget::resizeEvent(re);
    mp.resize(re->size());
    wi = re->size().width();
    he = re->size().height();
}

void MapViewer::paintEvent(QPaintEvent *pe){
    QWidget::paintEvent(pe);
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    mp.paint(p);
}




void MapViewer::updateRequest(){
    //qDebug() << "UpdateRequest" << rand();
    if(!tiUp->isActive()) tiUp->start();
}



MapViewer::MapViewer(QWidget *parent) :
    QWidget(parent), ms(Rest)
{
    setAutoFillBackground(true);
    QPalette p(palette());
    p.setColor(QPalette::Window, QColor(0,0,20));
    setPalette(p);
    setMouseTracking(true);
    ti = new QTimer(this);
    connect(ti, SIGNAL(timeout()), this, SLOT(mousePosChecking()));
    ti->setSingleShot(true);
    zooming = false;

    tiUp = new QTimer(this);
    tiUp->setInterval(10);
    tiUp->setSingleShot(true);
    connect(tiUp, SIGNAL(timeout()), this, SLOT(update()));





}



void MapViewer::mouseOutEvent(){
    mp.setHighlightedCell(-1,-1);
    updateRequest();
}


bool MapViewer::updateMousePos(PtCoords p){
    if(map == nullptr) false;
/*    QPoint p = mp.highlightedCell();;
    QPointF bInd = mp.ptToCoo(p);
    QPoint np(bInd.x() < 0 ? -1 : bInd.x(),
              bInd.y() < 0 ? -1 : bInd.y());
    if(!(0<=np.x() && iSelCell<nbCellX && 0<=jSelCell && jSelCell<nbCellY)){
        iSelCell = -1;
        jSelCell = -1;
    }*/
    if(mp.setHighlightedCell(mp.ptToCoo(p))) updateRequest();
    return mp.hasHighlightedCell();
}



void MapViewer::mousePressEvent(QMouseEvent *me){
    if(map == nullptr) return;
    clickPos = me->pos();
    ms = me->button() == Qt::MiddleButton ? MClick :
         me->button() == Qt::RightButton ? RClick : LClick;
    //qDebug() << "Clic" << (int) ms;
    if(ms == LClick){
        if(mp.hasHighlightedCell()){
            map->cell(mp.highlightedCell()).invertSelected();
        }
    }
    else if(ms == RClick){
        mp.setHighlightedCell(-1,-1);
    }
    updateRequest();
}

void MapViewer::mouseMoveEvent(QMouseEvent *me){
    if(map == nullptr) return;
    if(ms == RClick){
        ti->stop();
        center = mp.viewCenter();
        ms = Moving;
    }
    int x = me->pos().x();
    int y = me->pos().y();
    bool change = false;
    if(ms == Moving){
        QPair<bool,bool> real(mp.move(me->pos()-clickPos, center));
        if(x<=0 && real.first){
            x += wi-2;
            clickPos.setX(clickPos.x() + wi-2);
            change = true;
        }
        if(x>=wi-1 && real.first){
            x -= wi-2;
            clickPos.setX(clickPos.x() - wi+2);
            change = true;
        }
        if(y<=0 && real.second){
            y += he-2;
            clickPos.setY(clickPos.y() + he-2);
            change = true;
        }
        if(y>=he-1 && real.second){
            y -= he-2;
            clickPos.setY(clickPos.y() - he+2);
            change = true;
        }
        updateRequest();
    }
    else if(ms==Rest){
        checkMousePos();
    }
    if(change)
        cursor().setPos(mapToGlobal(QPoint(x,y)));
    //qDebug() <<  "Bouge" << (int) ms;
    if(ms == Moving) mp.setHighlightedCell(-1,-1);
    else if(mp.hasHighlightedCell()){
            if(me->modifiers() & Qt::AltModifier) map->cell(mp.highlightedCell()).setSelected(!(me->modifiers() & Qt::ShiftModifier));
        }
}

void MapViewer::mouseReleaseEvent(QMouseEvent *me){
    if(map == nullptr) return;
    checkMousePos();
    ms = Rest;
    updateRequest();
}

void MapViewer::setMap(Map *m){
    mp.setMap(m);
    map = m;
    updateRequest();
}
