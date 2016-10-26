#ifndef MAPVIEWER_H
#define MAPVIEWER_H


#include <Game/game.h>
#include <QtWidgets>
#include <cmath>

#define MINMAX(a,x,b) std::min(std::max(a,x),b)

enum MouseState{Rest, RClick, LClick, MClick, Moving};


class MapViewer : public QWidget
{
    enum MouseTrackingPolicy{Adsdj};
        Q_OBJECT
    public:
        explicit MapViewer(QWidget *parent = 0);
        void setMap(Map* m);
        //void adjustView();
        void updateMap();




        QPointF currentViewCenter() const;
        void setCurrentViewCenter(QPointF relativeCenter);
        void setCurrentViewCenter(double relativeXCenter, double relativeYCenter);

        double currentScale() const;
        void setCurrentScale(double scale);
        void setScaleDomain(double scaleMin, double scaleMax);

    signals:
        void currentViewCenterChanged();

    public slots:
        void updateRequest();

    private slots:
        void mousePosChecking();

    private:
        QSize virtualSize() const;
        void setVirtualSize(QSize s);
        void setVirtualSize(int w, int h);

        void paint(QPainter &p);
        void wheelEvent(QWheelEvent* we);
        void mousePressEvent(QMouseEvent* me);
        void mouseMoveEvent(QMouseEvent* me);
        void mouseReleaseEvent(QMouseEvent* me);
        bool updateMousePos(QPointF p);
        void mouseOutEvent();
        void updateViewParameters();
        void paintEvent(QPaintEvent* pe);
        void resizeEvent(QResizeEvent* re);
        void checkMousePos();

        inline QPointF ptToPxl(QPointF p) const;
        inline QPointF pxlToPt(QPointF p) const;
        inline QPointF cooToPt(QPointF p) const;
        inline QPointF ptToCoo(QPointF p) const;
        inline QPointF indToPt(int i, int j) const;

        bool zomming;
        QPointF *intersec;
        Map* map;
        int nbCellX, nbCellY;
        double angX, angY;
        int iSelCell,jSelCell;

        MouseState ms;
        int wi,he;

        QTimer *ti, *tiUp;
        QPoint clickPos;
        QPointF center;
        bool mouseIn;
        double aScale;
        double xCenter, yCenter;

        bool zooming;
        double vWi, vHe;
        double f;
        double xMD, yMD;

        QImage herbe, herbeR;

};

#endif // MAPVIEWER_H
