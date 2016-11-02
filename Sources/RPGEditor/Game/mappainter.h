#ifndef MAPPAINTER_H
#define MAPPAINTER_H



/*! \file mappainter.h
 * \brief Definition of the class used to render maps
 *
 * This file defines four types of coordinates : RlCoords, ClCoords,
 * PtCoords and PxCoords. They all inherit from QPointF, and
 * give a static type checking for the consistency of the
 * coordinates which are used.
 */


#include "map.h"

#define MINMAX(a,x,b) std::min(std::max(a,x),b)

/*!
 * \brief The RlCoords class describe positions with relative coordinates.
 *
 * Theses coordinates have values in \f$[0,1]\f$, for every point in the view.
 *
 * See also ClCoords, PtCoords and PxCoords
 */
class RlCoords : public QPointF
{
public:
    explicit RlCoords(qreal x, qreal y) : QPointF(x,y){}
    explicit RlCoords(const QPointF &p) : QPointF(p){}
};

/*!
 * \brief The ClCoords class describe positions with cell coordinates.
 *
 * Theses coordinates describe each point relatively to the cell grid. They
 * correspond to the isometric 3D world.
 *
 * See also RlCoords, PtCoords and PxCoords
 */
class ClCoords : public QPointF
{
public:
    explicit ClCoords(qreal x, qreal y) : QPointF(x,y){}
    explicit ClCoords(const QPointF &p) : QPointF(p){}
};

/*!
 * \brief The PtCoords class describe positions with virtual point coordinates.
 *
 * Theses coordinates describe each point relatively to the view. They
 * correspond to a point in the image containing the entire map.
 *
 * See also RlCoords, ClCoords and PxCoords
 */
class PtCoords : public QPointF
{
public:
    explicit PtCoords() : QPointF(){}
    explicit PtCoords(qreal x, qreal y) : QPointF(x,y){}
    explicit PtCoords(const QPointF &p) : QPointF(p){}
};

/*!
 * \brief The PxCoords class describe positions with real pixel coordinates.
 *
 * Theses coordinates describe the pixel position.
 *
 * See also RlCoords, ClCoords and PtCoords
 */
class PxCoords : public QPointF
{
public:
    explicit PxCoords(qreal x, qreal y) : QPointF(x,y){}
    explicit PxCoords(const QPointF &p) : QPointF(p){}
    PxCoords(const QPoint &p) : QPointF(p){}
    PxCoords(int x, int y) : QPointF(x,y){}
};



/*!
 * \brief The MapPainter class that can paint a Map using a QPainter.
 *
 * The class take charge of the different ratios of the \ref Map "map" rendering and the
 * area in which it will be rendered.
 *
 * \note The view is kept updated with the associated \ref Map "map" at
 * each \ref paint or \ref render call. It is thus just needed to call one of these
 * functions to update the view after a modification.
 *
 *
 * To ensure a type checking security about the different types of coordinates that are
 * used, four different types that inherit from QPointF are used : RlCoords, ClCoords, PtCoords
 * and PxCoords
 *
 */
class MapPainter : public QObject
{
    // TODO Mode lecture seule.
    Q_OBJECT
public:
    /*!
     * \brief The Element enum discribes the different elements that
     * can be render.
     *
     * This includes both map's objects and user interaction
     * and editing elements.
     *
     * Element value can be used as flags using the operators
     * \ref operator| "|", \ref operator| "&", \ref operator| "^".
     *
     * See also Cell, CellType
     */
    enum Element{
        Nothing = 0,            /**< Represent no elements*/
        CellBackground = 1,     /**< The bacground associated to the \ref CellType "cell type" */
        Grid = 2,               /**< A thin grid that separate \ref Cell "cells"*/
        CellSelection = 4,      /**< Graphical information about the selection state*/
        CellHighlighting = 8,   /**< Graphical visualisation of the \ref Cell "cells" the mouse is over*/
        Objects = 16,           /**< The objects that lay on the \ref Cell "cells"*/
        All = 31                /**< Represent all elements*/
    };
    /**
     * Constructs a new MapPainter with a default size of (42,42).
     */
    MapPainter(QObject *parent = 0);
    /**
     * Constructs a new MapPainter with a default size of (42,42), and loads the \ref Map "map" m.
     */
    MapPainter(Map *m, QObject *parent = 0);

    void setPaintedElement(Element e, bool painted = true); /**< Enables or disables the render of an \ref Element "element". \n\n See also \ref setPaintedElements*/
    void setPaintedElements(Element e); /**< set the rendered \ref Element "elements". \n\n See also \ref setPaintedElement*/

    /**
     * Loads the \ref Map "map", computing the new size of the view area.
     */
    void setMap(Map* m);
    void paint(QPainter& p);    /**< Draws the map in the QPaintDevice. \n\n See also \ref render.*/
    const QImage& render();     /**< Provides a QImage with a view of the map.\n\n See also \ref paint.*/

    /**
     * Return the relative coordinates of the current view center.
     *
     * See also \ref setViewCenter.
     */
    RlCoords viewCenter() const;
    /**
     * Change the view center, using relative coordinates.
     *
     * If the new center is invalid (the view exceed the map area), the closest valid center is used.
     *
     * See also \ref viewCenter.
     */
    void setViewCenter(RlCoords relativeCenter);
    void setViewCenter(double relativeCenterX, double relativeCenterY); /**< This is an overload function, see \ref setViewCenter.*/
    void setViewCenterQuiet(double x, double y); /**< does the same as \ref setViewCenter, without emitting the signal viewCenterChanged to avoid event loop.*/

    /**
     * Returns the current scale of the view.
     *
     * See also \ref setScale.
     */
    double scale() const;
    void setScale(double scale); /**< Set the current view scale. This closest value in the scale domain will be used. \n\n See also \ref scale and \ref setScaleDomain.*/
    void setScaleDomain(double scaleMin, double scaleMax); /**< Set the valid values for the scale. \n\n See also \ref scale and \ref setScale.*/

    bool setHighlightedCell(const ClCoords& p); /**< Set the highligthed \ref Cell "cell" to the one at the ClCoords p \n\n See also \ref highlightedCell and \ref hasHighlightedCell.*/
    bool setHighlightedCell(int i, int j); /**< This is an overload function, see \ref setViewCenter.*/
    QPoint highlightedCell() const; /**< Returns the integer index of the \ref Cell "cell" the is highlighted. See also \ref sethighlightedCell and \ref hasHighlightedCell.*/
    bool hasHighlightedCell() const; /**< Returns true if a \ref Cell "cell" is highligthed.\n\n See also \ref highlightedCell and \ref setHighlightedCell.*/

    /**
     * Change the size of the view, <i> ie </i> the rectangle in which
     * the map will be render.
     *
     * See also \ref size.
     */
    void resize(QSize s);
    void resize(int wi, int he);/**< This is an overload function, see \ref resize*/
    /**
     * Return the size of the rectangle in which the map is render.
     * This is also the size of the image returned by \ref render.
     *
     * See also \ref resize.
     */
    QSize size() const;
    /**
     * Multiplying the scale of the view by factor, trying to leave the
     * point center at the same position.
     * \note It is not always possible to keep this point fixed, in particulary
     * when the view is resulting view would exceed the map region.
     * In that case, the center is adapt to minimise the difference.
     */
    void zoom(double factor, QPointF fixedPoint);
    /**
     * Change the center position from the given center and a pixel difference.
     *
     * The return value indicate if the expected center was valid (regarding x or y
     * coordinate).
     *
     * See also setViewCenter.
     */
    QPair<bool,bool> move(PxCoords delta, QPointF center);

    QSize virtualSize() const; /**< Computes the total size of the image of the map*/

    PxCoords ptToPxl(PtCoords p) const; /**< Converts virtual point to real pixel coordinates*/
    PtCoords pxlToPt(PxCoords p) const; /**< Converts real pixel to virtual point coordinates*/
    PtCoords cooToPt(ClCoords p) const; /**< Converts cells indice to virtual point coordinates*/
    ClCoords ptToCoo(PtCoords p) const; /**< Converts virtual point to cell indice*/
    PxCoords cooToPxl(ClCoords p) const; /**< Convenient function equivalent to \ref ptToPxl(\ref cooToPt "cooToPt"(p))*/
    ClCoords pxlToCoo(PxCoords p) const; /**< Convenient function equivalent to \ref ptToCoo(\ref pxlToPt "pxlToPt"(p))*/
    PtCoords indToPt(int i, int j) const; /**< Converts to coordinates*/

signals:
    /**
     * This signal is emitted when the total size of the \ref Map "map"'s view change.
     *
     * It appends mainly during scale change and modification on the \ref Map "map" (resize,
     * angles setting, ...).
     */
    void mapSizeChanged(QSize);
    /**
     * This signal is emitted when the center of the \ref Map "map" change.
     *
     * It appends mainly during moving on the view and zooming.
     */
    void viewCenterChanged(QPoint);


private slots:
    void updateMap();           /**< Checks if the map has changed*/

private:
    inline void globalViewChanged();
    void updateBackground();
    void updateViewParameters();

    inline void changeBackgroundSize();
    inline void changeBackgroundDistortion();
    QImage &getBackground(CellType *ct);

    Map* map;
    Element displayed;
    QMatrix isometricTransform; // Matrix of the image transformation
    int pWidth, pHeight;
    int nbCellsX, nbCellsY;
    int mapWidth, mapHeight;
    int selCellX, selCellY;
    double angleX, angleY;
    double centerX, centerY;
    double centerVarX, centerVarY;
    double viewScale;
    double ratioFactor;
    QSize cellSize;


    PtCoords *intersec;
    bool changesOccured;
    QDateTime lastMapUpdate;
    QMap<int, QImage> cellBackgrounds; // Contains the distorted images
    QMap<int, QImage> scaledCellBackgrounds;

    QImage im;

    int iMin, iMax, jMin, jMax;


};
/*! \brief The operator | is the flag OR operation. La suite*/
inline MapPainter::Element operator|(MapPainter::Element a, MapPainter::Element b){return static_cast<MapPainter::Element>(static_cast<int>(a)|static_cast<int>(b));}
/*! \brief The operator & is the flag AND operation*/
inline MapPainter::Element operator&(MapPainter::Element a, MapPainter::Element b){return static_cast<MapPainter::Element>(static_cast<int>(a)&static_cast<int>(b));}
/*! \brief The operator ^ is the flag substraction operation. \warning This is not a XOR operation, it corresponds to a&!b*/
inline MapPainter::Element operator^(MapPainter::Element a, MapPainter::Element b){return static_cast<MapPainter::Element>(static_cast<int>(a)&!static_cast<int>(b));}


#endif // MAPPAINTER_H
