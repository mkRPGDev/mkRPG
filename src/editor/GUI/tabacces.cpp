#include "tabacces.h"

TabAcces::TabAcces(int i, const QString &n, const QPixmap &p, QWidget *parent) :
    QWidget(parent), id(i)
{
    setupUi(this);
    c = palette().color(QPalette::Window);
    image->setPixmap(p);
    title->setText(n);
}

void TabAcces::setActive(bool a){
    QPalette p(palette());
    p.setColor(QPalette::Window, a ? c : QColor(0,0,0,0));
    setPalette(p);
    if(a) emit activated(id);
}


void TabAcces::mousePressEvent(QMouseEvent *){
    setActive(true);
}

