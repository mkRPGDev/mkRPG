#ifndef BVDOCKSZONE_H
#define BVDOCKSZONE_H

#include "bdockszone.h"



class BVDocksZone : public BDocksZone
{
    Q_OBJECT

public:
    explicit BVDocksZone(QWidget* parent = 0);
    int currentLength() const;
    void setCurrentLenght(int t);

private slots:
    //void animer(int t);
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *me);
    void mouseMoveEvent(QMouseEvent *me);
};

#endif // BVDOCKSZONE_H
