#ifndef MAPVIEWER_H
#define MAPVIEWER_H

#include <Game/game.h>
#include <Game/mappainter.h>
#include <QtWidgets>
#include <cmath>




/*!
 * \brief The MapViewer class provides a widget to display and edit a Map
 * using a MapPainter
 *
 * Several \ref SelectionMode "selection modes" are available. Combined with
 * the ```Ctrl``` and ```Shift``` modifiers, a colossal amount of selection possibilities
 * is offered. See \ref MapViewer::SelectionMode "SelectionMode" for more information.
 */
class MapViewer : public QWidget
{
    enum MouseState{
        Rest,
        RClick,
        LClick,
        MClick,
        Moving,
        Selection
    };
    Q_OBJECT
public:
    /*!
     * \brief The SelectionMode enum describes the different behaviour the
     * cell selection can have.
     *
     * The selection's behaviour in based on to parameters :
     *  - The keyboard modifiers that are pressed during selection.
     *  - The current selection mode
     *
     * If the ```Ctrl``` modifier is pressed, the past selected cells stay selected
     * otherwise, they are all unselected
     *
     * If the ```Shift``` modifier is pressed, the selection is inverted.
     *
     * Three modes of selection exists :
     */
    enum SelectionMode{
        PencilSelection,    /**< The \ref Cell "cells" under cursor are selected*/
        RectangleSelection, /**< The \ref Cell "cells" inside the rectangle defined by the clicked cell and the cell under the cursor are selected*/
        RegionSelection     /**< The \ref Cell "cells" inside the region drawn by cursor's moves are selected*/
    };



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
    void mouseReleaseEvent(QMouseEvent*me);
    bool updateMousePos(PtCoords p);
    void mouseOutEvent();
    void paintEvent(QPaintEvent* pe);
    void resizeEvent(QResizeEvent* re);
    void checkMousePos();


    void updateSelection(ClCoords pos);
    void selectCellBetween(ClCoords p0, ClCoords p1, ClCoords p2);

    MapPainter mp;

    Map* map;


    SelectionMode sm;
    MouseState ms;
    int wi,he;
    ClCoords clClick, clMove;
    QTimer *ti, *tiUp, *tiSel;
    QPoint clickPos, selectPos;
    QPointF cellClicked;
    QPointF center;
    bool mouseIn;


};

#endif // MAPVIEWER_H
