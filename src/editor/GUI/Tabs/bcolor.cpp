#include "bcolor.h"


BColor::BColor(QWidget *parent) :
    QFrame(parent), n(tr("Color chooser"))
{
    setAutoFillBackground(true);
    setMinimumHeight(25);
    setColor(QColor(255,255,255));
}

BColor::BColor(QColor c, QWidget *parent) :
    BColor(parent)
{
    setColor(c);
}


void BColor::setColorQuiet(const QColor &c){
    coul = c;
    QPalette p(palette());
    p.setColor(QPalette::Window, coul);
    setPalette(p);
    update();
}

void BColor::setColor(const QColor &c){
    setColorQuiet(c);
    emit colorChanged(c);
}

const QColor &BColor::color() const{
    return coul;
}

void BColor::setNameQuiet(const QString &s){
    n = s;
}
void BColor::setName(const QString &s){
    setNameQuiet(s);
    emit nameChanged(s);
}

const QString &BColor::name() const{
    return n;
}

void BColor::mousePressEvent(QMouseEvent *me){
    me->accept();
    QColor c = QColorDialog::getColor(coul, this, n, QColorDialog::ShowAlphaChannel);
    if(c.isValid()) setColor(c);
    emit colorChanged(coul);
}

