#include "bvdockszone.h"



BVDocksZone::BVDocksZone(QWidget *parent) :
    BDocksZone(parent)
{
    bUnfold->setFixedSize(BUTTON-1,24);
    bUnfold->setIconSize(QSize(5,20));
    lay->addWidget(bUnfold, 0,0,1,1);
    connect(bUnfold, SIGNAL(clicked(bool)), &unfoldStates, SLOT(swap()));
    unfoldStates.defineProperty(bUnfold, "icon",
                             QIcon("/home/baptiste/ENS/Stage/Epidev/Icones/fermer5.png"),
                             QIcon("/home/baptiste/ENS/Stage/Epidev/Icones/fermer.png"));
    unfoldStates.defineProperty(this, "length", lId, BUTTON);
    unfoldStates.defineProperty(this, "cursor", QCursor(Qt::SplitHCursor), QCursor(Qt::ArrowCursor));

    dockArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    dockArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    lay->addWidget(dockArea, 0,1,2,1);
    lay->setRowStretch(1,2);
    lay->setColumnStretch(4,1);
    //espace->setMinimumWidth(0);
    inLength->link(this, "currentLength");
    //connect(intaille, SIGNAL(changement(int)), this, SLOT(animer(int)));
    docks = new BLayout(Qt::Vertical);
    dockArea->setWidget(docks);
    docks->setLength(lId-BUTTON);
}


void BVDocksZone::paintEvent(QPaintEvent *event){
    BDocksZone::paintEvent(event);
    QPainter p(this);
    p.setPen(QColor(100,100,100));
    p.drawLine(QPoint(0,0), QPoint(0,height()));
    //qDebug() << sizeHint();
}

int BVDocksZone::currentLength() const{
    return height();
}

void BVDocksZone::setCurrentLenght(int t){
    dockArea->setFixedWidth(Max(0,t-BUTTON));
    setFixedWidth(t);
}

void BVDocksZone::mousePressEvent(QMouseEvent *me){
    if(me->x() < BUTTON && unfoldStates.isPositive())
        baseLength(me->globalX());
}

void BVDocksZone::mouseMoveEvent(QMouseEvent *me){
    if(resizing){
        newLength(me->globalX());
        unfoldStates.defineProperty(this, "length", lId, BUTTON);
        docks->setLength(lId-BUTTON);
    }
}

