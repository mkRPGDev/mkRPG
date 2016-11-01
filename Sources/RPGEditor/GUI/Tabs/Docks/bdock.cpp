#include "bdock.h"

// TODO option : bords de fenetres natifs si dock séparé

BDock::BDock(QString title, QWidget* dock, QWidget *parent) :
    QFrame(parent),
    dock(dock)
{
    lay = new QVBoxLayout;
    button = new QHBoxLayout;
    name = new QLabel(title);
    bUnfold = new QToolButton;
    bLinked = new QToolButton;
    QPalette p(name->palette());
    p.setColor(QPalette::Window, p.color(QPalette::Mid));//QColor(220,100,250));
    name->setPalette(p);
    name->setAutoFillBackground(true);
    name->setIndent(2);
    name->setFixedHeight(26);
    name->setMinimumHeight(26);
    dUnfold.defineProperty(bUnfold, "icon",
                           QIcon("/home/baptiste/ENS/Stage/Epidev/Icones/reduire.png"),
                           QIcon("/home/baptiste/ENS/Stage/Epidev/Icones/agrandir.png"));
    dUnfold.defineProperty(bUnfold, "toolTip",
                           tr("Hide the dock"),
                           tr("show the dock"));
    dUnfold.defineProperty(this, "unfold");
    bUnfold->setFixedSize(QSize(24,24));
    bUnfold->setAutoRaise(true);
    connect(bUnfold, SIGNAL(pressed()), &dUnfold, SLOT(swap()));

    bLinked->setFixedSize(QSize(24,24));
    bLinked->setAutoRaise(true);
    dLinked.defineProperty(bLinked, "icon",
                             QIcon("/home/baptiste/ENS/Stage/Epidev/Icones/separer.png"),
                             QIcon("/home/baptiste/ENS/Stage/Epidev/Icones/rassembler.png"));
    connect(bLinked, SIGNAL(pressed()), &dLinked, SLOT(swap()));

    zoneSize.link(this, "currentSize");

    lay->setMargin(0);
    lay->setSpacing(0);
    lay->setStretch(0,2);
    button->setContentsMargins(1,1,1,1);
    button->setSpacing(2);
    button->setAlignment(Qt::AlignRight);
    button->addWidget(bLinked);
    button->addWidget(bUnfold);
    name->setLayout(button);
    //lay->setContentsMargins(0,0,0,0);
    container = new QScrollArea;
    container->setMinimumHeight(0);
    container->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    container->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    container->setFrameStyle(QFrame::NoFrame);
    lay->addWidget(name);
    lay->addWidget(container);

    container->setAutoFillBackground(true);
    setLayout(lay);
    setFrameStyle(QFrame::Plain | QFrame::Box);
    container->setWidget(dock);
    wHeight = dock->sizeHint().height();
    dock->setFixedSize(width(),wHeight);
    //setLineWidth(1);
    //setMidLineWidth(0);
}


void BDock::mouseDoubleClickEvent(QMouseEvent *me){
    if(name->underMouse())
        dUnfold.swap();
}

void BDock::paintEvent(QPaintEvent *pe){
    QFrame::paintEvent(pe);
    QPalette pal(palette());
    int l = width();
    int h = height();
    QPainter p(this);
    /*p.setPen(pal.color(QPalette::Window));
    p.drawPoint(1,1);
    p.drawPoint(l-2,1);
    p.drawPoint(1,h-2);
    p.drawPoint(l-2,h-2);*/
    // NOTE que si attaché ?
    p.setPen(pal.color(QPalette::Window));
    p.drawPoint(0,0);
    p.drawPoint(l-1,0);
    p.drawPoint(0,h-1);
    p.drawPoint(l-1,h-1);
}

bool BDock::unfold() const{
    return dUnfold.isPositive();
}

void BDock::setUnfold(bool v){
    dUnfold.define(v);
    zoneSize.setValue(v*wHeight);
}

int BDock::currentSize() const{
    return container->height();
}

void BDock::setCurrentSize(int t){
    container->setFixedHeight(t);
    resize(width(), name->height() + 2 + t);
}

void BDock::resizeEvent(QResizeEvent *re){
    QFrame::resizeEvent(re);
    dock->setFixedWidth(re->size().width()-2);
}
