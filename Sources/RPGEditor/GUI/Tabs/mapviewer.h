#ifndef MAPVIEWER_H
#define MAPVIEWER_H

#include <Game/game.h>
#include <Game/mappainter.h>
#include <QtWidgets>
#include <cmath>




/*!
 * \brief The MapViewer class provides a widget to display and edit a Map
 * using a MapPainter
 */
class MapViewer : public QWidget
{
    enum MouseState{Rest, RClick, LClick, MClick, Moving, ContinuousSelection};
    Q_OBJECT
public:
    explicit MapViewer(QWidget *parent = 0);
    void setMap(Map* m);
    //void adjustView();
    void updateMap();

    inline MapPainter& mapPainter() {return mp;}

signals:
    void viewSizeChanged(QSize);
    void selectionChanged();

public slots:
    void updateRequest();

private slots:
    void mousePosChecking();
    void selectionOut();

private:
// NOTE currently, Update IS NOT private !

    void wheelEvent(QWheelEvent* we);
    void mousePressEvent(QMouseEvent* me);
    void mouseMoveEvent(QMouseEvent* me);
    void mouseReleaseEvent(QMouseEvent* me);
    bool updateMousePos(PtCoords p);
    void mouseOutEvent();
    void paintEvent(QPaintEvent* pe);
    void resizeEvent(QResizeEvent* re);
    void checkMousePos();


    MapPainter mp;

    Map* map;


    MouseState ms;
    int wi,he;

    QTimer *ti, *tiUp, *tiSel;
    QPoint clickPos, selectPos;
    QPointF center;
    bool mouseIn;


};

#endif // MAPVIEWER_H
