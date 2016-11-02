#include "bdockszone.h"


BLayout::BLayout(Qt::Orientation o, QWidget *parent) :
    QWidget(parent), orient(o)
{
//    lay = orient == Qt::Vertical ? new QVBoxLayout : new QHBoxLayout;
//    setLayout(lay);
    len = width(); // TEMPORAIRE ?
    space = 2;
    resize(len,0);
    setCursor(Qt::SizeVerCursor);
}

void BLayout::insert(BDock *d, int ind){
    if(ind==-1) ind = docks.length();
    ind = Min(docks.length(), ind);
    docks.insert(ind, d);
    d->setParent(this);
    d->setCursor(Qt::ArrowCursor);
    adjust();
}

void BLayout::paintEvent(QPaintEvent *event){
    adjust();
}

void BLayout::adjust(){
    int i(1);
    if(orient == Qt::Vertical){
        for(BDock *d : docks){
            d->setGeometry(0,i,len, d->sizeHint().height());
            i+=d->sizeHint().height()+space;
            //qDebug() << i;
        }
        //qDebug() << docks;
        resize(len, i-space);
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
    for(BDock *d : docks) d->resize(len-1, d->height());


    //qDebug() << "largeur" << larg << re->size().height();
    // TODO
}

void BLayout::mouseDoubleClickEvent(QMouseEvent *){

}

void BLayout::setLength(int t){
    orient == Qt::Vertical ? setFixedWidth(t-1)
                           : setFixedHeight(t-1);
}




BDocksZone::BDocksZone(QWidget *parent) : QWidget(parent)
{
    lMin = 150;
    lMax = 400;
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
    lId = options.load<int>(MAP, "DocksLength");
    inLength->setValue(lId);
    if(!options.load<bool>(MAP, "DocksVisible"))
        QTimer::singleShot(10,this, SLOT(swap()));
    connect(&unfoldStates, SIGNAL(swapped(bool)), this, SLOT(foldingChanged(bool)));
    /*QPalette p(espace->palette());
    p.setColor(QPalette::Window, QColor(150,150,150));
    espace->setPalette(p);*/
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

void BDocksZone::mouseReleaseEvent(QMouseEvent *me){
    resizing = false;
}

void BDocksZone::baseLength(int pos){
    resizing = true;
    base = pos;
    lBase = lId;
}

void BDocksZone::newLength(int pos){
    lId = MinMax(lMin, lBase - (pos-base) - BUTTON, lMax) + BUTTON;
    inLength->setValue(lId, false);
    Options::options().save(MAP, "DocksLength", lId);
}

void BDocksZone::foldingChanged(bool f){
    Options::options().save(MAP, "DocksVisible", f);
}

void BDocksZone::addDock(QString title, BDockWidget *dock){
    docks->insert(new BDock(title, dock, docks));
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


