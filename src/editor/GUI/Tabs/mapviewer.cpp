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

    sm = PencilSelection;


    tiSel = new QTimer(this);
    tiSel->setInterval(10);
    connect(tiSel, SIGNAL(timeout()), this, SLOT(selectionOut()));
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
    if(mouseIn || ms == Selection){ if(updateMousePos(mp.pxlToPt(pt))) ti->start(100);}
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
    if(map == nullptr) return false;
    bool in = mp.setHighlightedCell(mp.ptToCoo(p));
    if(in || mouseIn) updateRequest();
    mouseIn = in;
    return mp.hasHighlightedCell();
}



void MapViewer::mousePressEvent(QMouseEvent *me){
    if(map == nullptr) return;
    clickPos = me->pos();
    cellClicked = mp.pxlToCoo(clickPos);
    cellMove = cellClicked;
    ms = me->button() == Qt::MiddleButton ? MClick :
         me->button() == Qt::RightButton ? RClick : LClick;
    if(ms == LClick){
        if(!(me->modifiers() & Qt::ControlModifier)) map->unSelectAll();
        if(mp.hasHighlightedCell()){
            map->cell(mp.highlightedCell()).invertSelected();
        }
        emit selectionChanged();
        map->confirmPreSelection();
    }
    updateRequest();
}

void MapViewer::mouseMoveEvent(QMouseEvent *me){
    if(map == nullptr) return;
    tiSel->stop();
    checkMousePos();
    if(ms == RClick){
        mp.setHighlightedCell(-1,-1);
        ti->stop();
        center = mp.viewCenter();
        ms = Moving;
        setCursor(Qt::ClosedHandCursor);
    }
    else if(ms == LClick){
        map->cell(cellClicked.x(), cellClicked.y()).setSelected(!(me->modifiers() & Qt::ShiftModifier));
        ms = Selection;
        //cellClicked = mp.pxlToCoo(clickPos);
    }
    int x = me->pos().x();
    int y = me->pos().y();
    if(ms == Moving){
        bool change = false;
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
        mp.setHighlightedCell(-1,-1);
        if(change)
            cursor().setPos(mapToGlobal(QPoint(x,y)));
    }
    else if(ms == Selection){
        int dx = std::max(0,x-wi)+std::min(x,0);
        int dy = std::max(0,y-he)+std::min(y,0);
        if(dx || dy){
            auto f = [](double x)->double{return atan(x)*8;};
            selectPos = QPoint(f(dx),f(dy));
            tiSel->start();
            mp.move(-selectPos, mp.viewCenter());
            checkMousePos();
        }
        updateSelection(mp.pxlToCoo(me->pos()));
    }
    updateRequest();

}

void MapViewer::selectionOut(){
    mp.move(-selectPos, mp.viewCenter());
    checkMousePos();
    if(mp.hasHighlightedCell()){
        map->cell(mp.highlightedCell()).setSelected(true); // TODO Voir avec shift...
        emit selectionChanged();
    }
}

void MapViewer::updateSelection(ClCoords pos){
    bool reverse = QApplication::keyboardModifiers() & Qt::ShiftModifier;
    if(sm == PencilSelection || sm == RegionSelection){
        if(mp.hasHighlightedCell())
            map->cell(mp.highlightedCell()).setSelected(!reverse);
        if(sm == RegionSelection){
            selectCellBetween(cellClicked, cellMove, pos);
            cellMove = pos;
        }
    }
    else{
        map->clearPreSelection();
        map->cell(cellClicked.x(), cellClicked.y()).setSelected(!reverse);
        int px = std::max((int)std::min(pos.x(), cellClicked.x()),0);
        int py = std::max((int)std::min(pos.y(), cellClicked.y()),0);
        int qx = std::min((int)std::max(pos.x(), cellClicked.x())+1,map->width());
        int qy = std::min((int)std::max(pos.y(), cellClicked.y())+1,map->height());
        for(int i(px); i<qx; ++i)
            for(int j(py); j<qy; ++j)
                map->cell(i,j).addSelection();
    }

    emit selectionChanged();

}



void MapViewer::selectCellBetween(ClCoords p0, ClCoords p1, ClCoords p2){
    Cell* cells = &map->cell(0,0); // Sorry Sorry Sorry...
    int w = map->width();
    double xMin = std::max(0.,std::min(std::min(p0.x(), p1.x()), p2.x()));
    double yMin = std::max(0.,std::min(std::min(p0.y(), p1.y()), p2.y()));
    double xMax = std::min((double)w,1+std::max(std::max(p0.x(), p1.x()), p2.x()));
    double yMax = std::min((double)map->height(),1+std::max(std::max(p0.y(), p1.y()), p2.y()));
    auto f = [](double x, double y, double z)->bool{return x*y>=0 && y*z>=0;};
    for(int i(xMin); i<xMax; ++i)
        for(int j(yMin); j<yMax; ++j)
            if(f((i+.5-p0.x())*(p1.y()-p0.y()) - (j+.5-p0.y())*(p1.x()-p0.x()),
                 (i+.5-p1.x())*(p2.y()-p1.y()) - (j+.5-p1.y())*(p2.x()-p1.x()),
                 (i+.5-p2.x())*(p0.y()-p2.y()) - (j+.5-p2.y())*(p0.x()-p2.x()))) // NOTE Temporaire ?
                cells[i+j*w].addSelection();
}


void MapViewer::mouseReleaseEvent(QMouseEvent *me){
    tiSel->stop();
    if(map == nullptr) return;
    checkMousePos();
    ms = Rest;
    map->confirmPreSelection(!(me->modifiers() & Qt::ShiftModifier));
    updateRequest();
    setCursor(Qt::ArrowCursor);
}

void MapViewer::setMap(Map *m){
    mp.setMap(m);
    map = m;
    updateRequest();
}

void MapViewer::setSelectionMode(SelectionMode m){
    sm = m;
}
