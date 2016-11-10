#include "bdockszone.h"


BLayout::BLayout(QWidget *parent) :
    QWidget(parent), orient(Qt::Vertical), movingDock(nullptr)
{
//    lay = orient == Qt::Vertical ? new QVBoxLayout : new QHBoxLayout;
//    setLayout(lay);
    len = width(); // TEMPORAIRE ?
    space = 2;
    resize(len,0);
    setCursor(Qt::SplitVCursor);
    //setAutoFillBackground(true);
    repainting = false;
    initPos = 0;
}

void BLayout::setOrientation(Qt::Orientation o){
    orient = o;
}

void BLayout::insert(BDock *d, int ind){
    if(ind<0 || ind > docks.length()) ind = docks.length();
    docks.insert(ind, d);
    d->setIndex(ind);
    d->setParent(this);
    d->setCursor(Qt::ArrowCursor);
    placement.insert(ind, QPair<int, int>(0,0));
    adjust();

    connect(d, SIGNAL(mouseClick(int,QPoint)), this, SLOT(dockClick(int,QPoint)));
    connect(d, SIGNAL(mouseMove(int,QPoint)), this, SLOT(dockMove(int,QPoint)));
    connect(d, SIGNAL(mouseRelease(int,QPoint)), this, SLOT(dockRelease(int,QPoint)));
    connect(d, SIGNAL(movementFinished(int)), this, SLOT(dockStopped(int)));
}

void BLayout::paintEvent(QPaintEvent *event){
    adjust();
    QWidget::paintEvent(event);
    if(movingDock){
        QPainter p(this);
        int k = movingDock->index();
        int min = k ? docks[k-1]->y() + placement[k-1].second + 2 : 0;
        int max = k==docks.length()-1 ? size : docks[k+1]->y() - 2;
        --max;
        p.setBrush(QBrush(QColor(100,200,255)));
        p.drawRect(0,min,len-1,max-min);
        p.setPen(palette().color(QPalette::Window));
        p.drawPoint(0,min);
        p.drawPoint(0,max);
        p.drawPoint(len-1,min);
        p.drawPoint(len-1,max);
        if(!repainting) update();
        repainting = !repainting;
        //QTimer::singleShot(40, this, SLOT(update()));
    }
}

void BLayout::adjust(){
    int i(0);
    if(orient == Qt::Vertical){
        for(BDock *d : docks){
            int h = d->sizeHint().height();
            if(d!=movingDock) d->moveTo(i, movingDock != nullptr);
            d->setLength(len);
            //d->setGeometry(0,i,len, h);
            placement[d->index()] = QPair<int,int>(i,h);
            i+=h+space;
            //qDebug() << i;
        }
        //qDebug() << docks;
        int newSize = i - space;
        if(newSize != size) emit sizeChanged(newSize);
        size = newSize;
        resize(len, size);
    }
    else{

    }
}

void BLayout::setSpacing(int e){
    space = e;
    adjust();
}

int BLayout::spacing() const{
    return space;
}


void BLayout::resizeEvent(QResizeEvent *re){
    QWidget::resizeEvent(re);
    // Orient !!!
    len = re->size().width();
    adjust();
}

void BLayout::mouseDoubleClickEvent(QMouseEvent *me){
    me->accept();
}

void BLayout::setLength(int t){
    orient == Qt::Vertical ? setFixedWidth(t-1)
                           : setFixedHeight(t-1);
}



void BLayout::dockClick(int i, const QPoint &p){
    clPos = p.y();
    movingDock = docks[i];
    initPos = placement[i].first;
    movingDock->raise();
    prev = p;
}

void BLayout::dockMove(int k,const QPoint &p){
    if(movingDock == nullptr) return;
    int h = movingDock->height();
    int newPos = MinMax(0, initPos + p.y() - clPos, size - h);
    movingDock->moveTo(newPos, false);
    int endPos = newPos + h;
    if(p.y() > prev.y())
        emit showPoint(0,endPos);
    prev = p;
    emit showPoint(0,newPos);

    if(newPos<placement[k].first){
        int i(0);
        while(i<k && placement[i].first + placement[i].second/2<newPos) ++i;
        if(i!=k){
            placement[k].first = placement[i].first - placement[i].second + placement[k].second;
            docks.move(k,i);
            placement.move(k,i);
            movingDock->setIndex(i);
            movingDock->setIndex(i);
            for(;i++<k;){
                placement[i].first += h+2;
                docks[i]->moveTo(placement[i].first);
                docks[i]->setIndex(i);
            }
        }
    }
    else{
        int i(docks.length()-1);
        while(i>k && placement[i].first + placement[i].second/2>endPos) --i;
        if(i!=k){
            placement[k].first = placement[i].first;
            docks.move(k,i);
            placement.move(k,i);
            movingDock->setIndex(i);
            movingDock->setIndex(i);
            for(;i-->k;){
                placement[i].first -= h+2;
                docks[i]->moveTo(placement[i].first);
                docks[i]->setIndex(i);
            }
        }
    }
}

void BLayout::dockRelease(int i, const QPoint &p){
    if(movingDock == nullptr) return;
    movingDock->moveTo(placement[i].first);
    initPos = -1;
}

void BLayout::dockStopped(int i){
    if(initPos == -1 && i == movingDock->index()){
        initPos = 0;
        movingDock = nullptr;
    }
}







BDocksZone::BDocksZone(QWidget *parent) :
    QWidget(parent), resizing(false)
{
    lMin = 150;
    lMax = 400;
    docks = new BLayout(this);
    inLength = new Intertie(this);
    lay = new QGridLayout;
    setLayout(lay);
    lay->setContentsMargins(1,0,0,0);
    lay->setSpacing(0);
    bUnfold = new QToolButton;
    bUnfold->setAutoRaise(true);
    bUnfold->setCursor(Qt::ArrowCursor);

    unfoldStates.defineProperty(bUnfold, "toolTip",
                                tr("Hide docks"),
                                tr("show docks"));
    dockArea = new QScrollArea;
    dockArea->setCursor(Qt::ArrowCursor);
    dockArea->setFrameShape(QFrame::NoFrame);
    dockArea->setAutoFillBackground(true);


    Options &options(Options::options());
    setDockLength(options.load<int>(MAP, "DocksLength"));
    if(!options.load<bool>(MAP, "DocksVisible"))
        QTimer::singleShot(10,this, SLOT(swap()));
    connect(&unfoldStates, SIGNAL(swapped(bool)), this, SLOT(foldingChanged(bool)));
    connect(docks, SIGNAL(sizeChanged(int)), this, SLOT(docksSizeChanged()));

    connect(docks, SIGNAL(showPoint(int,int)), this, SLOT(showPoint(int,int)));

    bUnfold->setFixedSize(BUTTON-1,24);
    bUnfold->setIconSize(QSize(5,20));
    lay->addWidget(bUnfold, 0,0,1,1);
    connect(bUnfold, SIGNAL(clicked(bool)), &unfoldStates, SLOT(swap()));
    unfoldStates.defineProperty(bUnfold, "icon",
                             QIcon("/home/baptiste/ENS/Stage/Epidev/Icones/fermer5.png"),
                             QIcon("/home/baptiste/ENS/Stage/Epidev/Icones/fermer.png"));
    unfoldStates.defineProperty(this, "cursor", QCursor(Qt::SplitHCursor), QCursor(Qt::ArrowCursor));
    unfoldStates.defineProperty(this, "toolTip", "", "Double clic to extend");

    connect(dockArea->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(updateMousePos()));

    dockArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    dockArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    lay->addWidget(dockArea, 0,1,2,1);
    lay->setRowStretch(1,2);
    lay->setColumnStretch(4,1);
    //espace->setMinimumWidth(0);
    inLength->link(this, "currentLength");
    //connect(intaille, SIGNAL(changement(int)), this, SLOT(animer(int)));
    dockArea->setWidget(docks);
    docks->setOrientation(Qt::Vertical);
    /*QPalette p(espace->palette());
    p.setColor(QPalette::Window, QColor(150,150,150));
    espace->setPalette(p);*/
}


void BDocksZone::paintEvent(QPaintEvent *event){
    QWidget::paintEvent(event);
    QPainter p(this);
    p.setPen(QColor(100,100,100));
    p.drawLine(QPoint(0,0), QPoint(0,height()));
    //qDebug() << sizeHint();
}

int BDocksZone::currentLength() const{
    return height();
}

void BDocksZone::setCurrentLenght(int t){
    dockArea->setFixedWidth(Max(0,t-BUTTON));
    setFixedWidth(t);
}

void BDocksZone::setUnfold(bool u, bool anim){
    unfoldStates.setPositive(u);
}

void BDocksZone::swap(bool anim){
    unfoldStates.swap();
}

void BDocksZone::mouseDoubleClickEvent(QMouseEvent *){
    swap();
}
void BDocksZone::mousePressEvent(QMouseEvent *me){
    if(me->x() < BUTTON && unfoldStates.isPositive()){
        base = me->globalX();
        lBase = lId;
        resizing = true;
    }
}

void BDocksZone::mouseMoveEvent(QMouseEvent *me){
    if(resizing) setDockLength(MinMax(lMin, lBase - (me->globalX()-base), lMax), false);
}


void BDocksZone::mouseReleaseEvent(QMouseEvent *me){
    resizing = false;
}


void BDocksZone::foldingChanged(bool f){
    Options::options().save(MAP, "DocksVisible", f);
}

void BDocksZone::docksSizeChanged(){
    QTimer::singleShot(10,this,SLOT(updateDockLength()));
}

void BDocksZone::addDock(QString title, BDockWidget *dock){
    docks->insert(new BDock(title, dock, docks));
    update();
}

void BDocksZone::setScrollBarMode(ScrollBarMode m){
    scrm = m;
    // WARNING orient
    dockArea->setVerticalScrollBarPolicy(m == AlwaysVisible ? Qt::ScrollBarAlwaysOn : Qt::ScrollBarAsNeeded);
}

void BDocksZone::resizeEvent(QResizeEvent *event){
    QWidget::resizeEvent(event);
    if(event->oldSize().height() != event->size().height()){
        docks->resize(lId, docks->height()*4); // Pas très joli... désolé
        QTimer::singleShot(10,this,SLOT(updateDockLength()));
    }
}

void BDocksZone::updateDockLength(){
    setDockLength(lId);
}

const BinaryStateMachine *BDocksZone::states() const{
    return &unfoldStates;
}

int BDocksZone::length() const{
    return inLength->value();
}

void BDocksZone::setLength(int t){
    //qDebug() << "salut";
    inLength->setValue(t);
}

void BDocksZone::setDockLength(int le, bool inert){
    if(le<20) le = 20;
    int scLen = dockArea->verticalScrollBar()->isVisible() ? dockArea->verticalScrollBar()->width() : 0;
    lId = le;
    Options::options().save(MAP, "DocksLength", lId);
    int l = lId + BUTTON + scLen;
    docks->setLength(lId);
    unfoldStates.defineProperty(this, "length", l, BUTTON);
    if(!inert)
        inLength->setValue(l,false);
}

void BDocksZone::showPoint(int x, int y){
    dockArea->ensureVisible(x, y, 4, 4);
}
void BDocksZone::updateMousePos(){
    QCursor c(cursor());
    QPoint p(c.pos());
    c.setPos(p-QPoint(1,0));
    c.setPos(p);
}
