#include "mapviewer.h"


#define CELL_WIDTH 64





void MapViewer::setVirtualSize(QSize s){
    setVirtualSize(s.width(), s.height());
}

void MapViewer::setVirtualSize(int w, int h){
    vWi = w;
    vHe = h;
    f = std::min(wi/vWi, he/vHe);
    updateViewParameters();
}


void MapViewer::updateViewParameters(){
    xMD = std::max(0.,(1-wi/f/vWi/aScale)/2);
    yMD = std::max(0.,(1-he/f/vHe/aScale)/2);
    xCenter = MINMAX(.5-xMD,xCenter,.5+xMD);
    yCenter = MINMAX(.5-yMD,yCenter,.5+yMD);
}


void MapViewer::wheelEvent(QWheelEvent *we){
    if(map == nullptr) return;
    we->accept();
    if(zooming) return;
    zooming = true;
    double r = we->delta()<0 ? .95 : 1.05;
    r = pow(r,(double) abs(we->delta())/120);
    QPoint d = we->pos()-QPoint(wi/2,he/2);
    r = MINMAX(1./aScale, r,10./aScale);
    updateViewParameters();
    xCenter = MINMAX(.5-xMD, xCenter - (1-r)*d.x()/aScale/f/vWi, .5+xMD);
    yCenter = MINMAX(.5-yMD, yCenter - (1-r)*d.y()/aScale/f/vHe, .5+yMD);
    aScale *= r;
    QTimer::singleShot(0,this, SLOT(update()));
    checkMousePos();
    zooming = false;
}



void MapViewer::mousePosChecking(){
    QPoint pt(mapFromGlobal(cursor().pos()));
    bool b = mouseIn;
    mouseIn = pt.x()>=0 && pt.x() < wi && pt.y() >=0 && pt.y() < he;
    if(mouseIn){ if(updateMousePos(pxlToPt(pt))) ti->start(100);}
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
    wi = re->size().width();
    he = re->size().height();
    f = std::min(wi/vWi, he/vHe);
    updateViewParameters();
    //update();
}

void MapViewer::paintEvent(QPaintEvent *pe){
    QWidget::paintEvent(pe);
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    paint(p);
}




void MapViewer::updateRequest(){
    //qDebug() << "UpdateRequest" << rand();
    if(!tiUp->isActive()) tiUp->start();
}



MapViewer::MapViewer(QWidget *parent) :
    QWidget(parent), ms(Rest),
    aScale(1), xCenter(0.5), yCenter(0.5),
    intersec(new QPointF[0]), map(nullptr),
    nbCellX(0), nbCellY(0),iSelCell(-1)
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



    herbe = QImage("/home/baptiste/glace.png").transformed(QMatrix(1,0,0,-1,0,0));


}



void MapViewer::paint(QPainter &p){
    if(map == nullptr) return;
    int iMin = std::max(0.,ptToCoo(pxlToPt(QPointF(0,he))).x());
    int iMax = std::min(nbCellX+0.,ptToCoo(pxlToPt(QPointF(wi,0))).x()+1);
    int jMin = std::max(0.,ptToCoo(pxlToPt(QPointF(wi,he))).y());
    int jMax = std::min(nbCellY+0.,ptToCoo(pxlToPt(QPointF(0,0))).y()+1);
    QImage hr(herbeR.scaled((-ptToPxl(indToPt(0,1))+ptToPxl(indToPt(1,0))).x()+1,
                            (-ptToPxl(indToPt(1,1))+ptToPxl(indToPt(0,0))).y()+1));
    QBrush b1(QColor(0,0,0,0));
    QBrush b2(QColor(255,180,0,60));
    p.setPen(QColor(0,0,0,0));
    for(int i(iMax); i-->iMin;)
        for(int j(jMax); j-->jMin;){
            p.setBrush(map->cell(i,j).isSelected() ? b2 : b1);
            p.drawImage(ptToPxl(indToPt(i,j+1)).x(), ptToPxl(indToPt(i+1,j+1)).y(), hr);
            p.drawConvexPolygon(QVector<QPointF>({ptToPxl(indToPt(i,j)),
                                                  ptToPxl(indToPt(i,j+1)),
                                                  ptToPxl(indToPt(i+1,j+1)),
                                                  ptToPxl(indToPt(i+1,j))}));
        }

    p.setPen(QColor(80,80,80));
    p.setBrush(Qt::NoBrush);
    for(int i(0); i<=nbCellX; ++i)
        p.drawLine(ptToPxl(indToPt(i,0)),
                   ptToPxl(indToPt(i,nbCellY)));
    for(int i(0); i<=nbCellY; ++i)
        p.drawLine(ptToPxl(indToPt(0,i)),
                   ptToPxl(indToPt(nbCellX,i)));
    if(0<=iSelCell && iSelCell<nbCellX && 0<=jSelCell && jSelCell<=nbCellY && ms != Moving){
        p.setPen(QColor(255,255,20));
        p.drawPolygon(QVector<QPointF>({ptToPxl(indToPt(iSelCell,jSelCell)),
                                       ptToPxl(indToPt(iSelCell,jSelCell+1)),
                                       ptToPxl(indToPt(iSelCell+1,jSelCell+1)),
                                       ptToPxl(indToPt(iSelCell+1,jSelCell))}));
    }return;
}


QPointF MapViewer::pxlToPt(QPointF p) const{
    return QPointF((p.x()-wi/2)/f/aScale+xCenter*vWi,
                   vHe-(p.y()-he/2)/f/aScale-yCenter*vHe);
}
QPointF MapViewer::ptToPxl(QPointF p) const{
    return QPointF((p.x()-xCenter*vWi)*aScale*f+wi/2.,
                   (vHe-p.y() - yCenter*vHe)*aScale*f+he/2.);
}
inline QPointF MapViewer::ptToCoo(QPointF p) const{
    double d = cos(angX)*sin(angY)-sin(angX)*cos(angY);
    double x = p.x() + (map->height()+4)*cos(angY)*CELL_WIDTH;
    return QPointF((sin(angY)*x -cos(angY)*p.y())/d/CELL_WIDTH-2,
                   (-sin(angX)*x +cos(angX)*p.y())/d/CELL_WIDTH-2);
}
// Add two row of hidden box
inline QPointF MapViewer::cooToPt(QPointF p) const{
    return QPointF(((p.x()+2.)*cos(angX) - (map->height()+2-p.y())*cos(angY))*CELL_WIDTH,
                   ((p.x()+2.)*sin(angX) + (p.y()+2)*sin(angY))*CELL_WIDTH);
}
inline QPointF MapViewer::indToPt(int i, int j) const{
    return intersec[i+(nbCellX+1)*j];
}

void MapViewer::updateMap(){
    if(map == nullptr) return;
    angX = M_PI*map->angleX()/1800.;
    angY = M_PI*(std::min(900+map->angleY(), 1800))/1800.;
    if(nbCellX-map->width() || nbCellY-map->height()){
        nbCellX = map->width();
        nbCellY = map->height();
        delete[] intersec;
        intersec = new QPointF[(nbCellX+1)*(nbCellY+1)];
    }
    setVirtualSize(cooToPt(QPointF(nbCellX+2, -2)).x(),
                   cooToPt(QPointF(nbCellX+2, nbCellY+2)).y());
    updateViewParameters();
    for(int i(0); i<=nbCellX; ++i)
        for(int j(0); j<=nbCellY; ++j)
            intersec[i+(nbCellX+1)*j] = cooToPt(QPointF(i,j));

    QMatrix tra;
    tra.setMatrix(cos(angX), -sin(angX), cos(angY), -sin(angY),0,0);


    herbeR = herbe.transformed(tra,Qt::SmoothTransformation);
    updateRequest();
}

void MapViewer::setMap(Map* m){
    map = m;
    updateMap();
}

void MapViewer::mouseOutEvent(){
    iSelCell = -1;
    jSelCell = -1;
    updateRequest();
}


bool MapViewer::updateMousePos(QPointF p){
    if(map == nullptr) false;
    int i = iSelCell;
    int j = jSelCell;
    QPointF bInd = ptToCoo(p);
    iSelCell = bInd.x() < 0 ? -1 : bInd.x();
    jSelCell = bInd.y() < 0 ? -1 : bInd.y();
    if(!(0<=iSelCell && iSelCell<nbCellX && 0<=jSelCell && jSelCell<nbCellY)){
        iSelCell = -1;
        jSelCell = -1;
    }
    if(i-iSelCell || j-jSelCell) updateRequest();
    return iSelCell+1;
}



void MapViewer::mousePressEvent(QMouseEvent *me){
    if(map == nullptr) return;
    clickPos = me->pos();
    ms = me->button() == Qt::MiddleButton ? MClick :
         me->button() == Qt::RightButton ? RClick : LClick;
    //qDebug() << "Clic" << (int) ms;
    if(ms == LClick){
        if(iSelCell+1){
            map->cell(iSelCell, jSelCell).invertSelected();
        }
    }
    else if(ms == RClick){
        iSelCell = -1;
    }
    updateRequest();
}

void MapViewer::mouseMoveEvent(QMouseEvent *me){
    if(map == nullptr) return;
    if(ms == RClick){
        ti->stop();
        center = QPointF(xCenter, yCenter);
        ms = Moving;
    }
    int x = me->pos().x();
    int y = me->pos().y();
    bool change = false;
    if(ms == Moving){
        QPoint d = me->pos()-clickPos;
        xCenter = MINMAX(.5-xMD,center.x() - d.x()/aScale/f/vWi, .5+xMD);
        yCenter = MINMAX(.5-yMD, center.y() - d.y()/aScale/f/vHe, .5+yMD);
        if(x<=0 && xCenter < .5+xMD){
            x += wi-2;
            clickPos.setX(clickPos.x() + wi-2);
            change = true;
        }
        if(x>=wi-1 && xCenter > .5-xMD){
            x -= wi-2;
            clickPos.setX(clickPos.x() - wi+2);
            change = true;
        }
        if(y<=0 && yCenter < .5+yMD){
            y += he-2;
            clickPos.setY(clickPos.y() + he-2);
            change = true;
        }
        if(y>=he-1 && yCenter > .5-yMD){
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
    if(ms == Moving) iSelCell = -1;
    else if(iSelCell+1){
            if(me->modifiers() & Qt::AltModifier) map->cell(iSelCell, jSelCell).setSelected(!(me->modifiers() & Qt::ShiftModifier));
        }
}

void MapViewer::mouseReleaseEvent(QMouseEvent *me){
    if(map == nullptr) return;
    checkMousePos();
    ms = Rest;
    updateRequest();
}
