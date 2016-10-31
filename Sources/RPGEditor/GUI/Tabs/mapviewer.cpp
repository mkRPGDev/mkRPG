#include "mapviewer.h"




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

    tiUp = new QTimer(this);
    tiUp->setInterval(10);
    tiUp->setSingleShot(true);
    connect(tiUp, SIGNAL(timeout()), this, SLOT(update()));


}



void MapViewer::updateMap(){
    updateRequest();
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
    mp.resize(re->size());
    wi = re->size().width();
    he = re->size().height();
    QWidget::resizeEvent(re);
    emit viewSizeChanged(re->size());
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


void MapViewer::mouseOutEvent(){
    mp.setHighlightedCell(-1,-1);
    updateRequest();
}


bool MapViewer::updateMousePos(PtCoords p){
    if(map == nullptr) false;
    bool in = mp.setHighlightedCell(mp.ptToCoo(p));
    if(in || mouseIn) updateRequest();
    mouseIn = in;
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
