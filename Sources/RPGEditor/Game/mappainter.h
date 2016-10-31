#ifndef MAPPAINTER_H
#define MAPPAINTER_H

#include "map.h"
#include <QtGui>

#define MINMAX(a,x,b) std::min(std::max(a,x),b)

/*!
 * \brief The CellCoords class describe positions with cell coordinates.
 *
 * Theses coordinates describe each point relatively to the cell grid. They
 * correspond to the isometric 3D world.
 *
 * See also PtCoords and PxCoords
 */
class ClCoords : public QPointF
{
public:
    explicit ClCoords(qreal x, qreal y) : QPointF(x,y){}
    explicit ClCoords(QPointF &p) : QPointF(p){}
};

/*!
 * \brief The PtCoords class describe positions with virtual map coordinates.
 *
 * Theses coordinates describe each point relatively to the map image. They
 * correspond to a point in the image containing the entire map.
 *
 * See also ClCoords and PxCoords
 */
class PtCoords : public QPointF
{
public:
    explicit PtCoords() : QPointF(){}
    explicit PtCoords(qreal x, qreal y) : QPointF(x,y){}
    explicit PtCoords(QPointF &p) : QPointF(p){}
};

/*!
 * \brief The PxCoords class describe positions with real pixel coordinates.
 *
 * Theses coordinates describe the pixel position.
 *
 * See also ClCoords and PtCoords
 */
class PxCoords : public QPointF
{
public:
    explicit PxCoords(qreal x, qreal y) : QPointF(x,y){}
    explicit PxCoords(QPointF &p) : QPointF(p){}
    PxCoords(QPoint &p) : QPointF(p){}
    PxCoords(int x, int y) : QPointF(x,y){}
};



/*!
 * \brief The MapPainter class that can paint a Map using a QPainter.
 *
 * Each view is adapt to the size of the MapPainter and can be scale or moved.
 *
 */
class MapPainter : public QObject
{

    Q_OBJECT
public:
    enum Element{
        CellBackground = 1,
        Grid = 2,
        CellSelection = 4,
        CellHighlighting = 8,
        Object = 16,
        All = 31
    };
    MapPainter(QObject *parent = 0);
    MapPainter(Map *m, QObject *parent = 0);

    void setPaintedElement(Element e, bool painted = true);
    void setPaintedElements(Element e);

    void setMap(Map* m);
    void paint(QPainter& p);    /**< Draw the map in the QPaintDevice*/
    const QImage& render();     /**< Provide a QImage with a view of the map*/


    QPointF viewCenter() const;
    void setViewCenter(QPointF relativeCenter);
    void setViewCenter(double relativeCenterX, double relativeCenterY);
    void setRelativeCenterPosition(double x, double y);

    double scale() const;
    void setScale(double scale);
    void setScaleDomain(double scaleMin, double scaleMax);

    bool setHighlightedCell(const ClCoords& p);
    bool setHighlightedCell(int i, int j);
    QPoint highlightedCell() const;
    bool hasHighlightedCell() const;

    void resize(QSize s);
    void resize(int wi, int he);
    QSize size() const;

    void zoom(double factor, QPointF fixedPoint);
    QPair<bool,bool> move(QPoint delta, QPointF center);

    QSize virtualSize() const; /**< Compute the total size of the image of the map*/

    PxCoords ptToPxl(PtCoords p) const; /*< Convert virtual point to real pixel coordinates*/
    PtCoords pxlToPt(PxCoords p) const; /*< Convert real pixel to virtual point coordinates*/
    PtCoords cooToPt(ClCoords p) const; /*< Convert cells indice to virtual point coordinates*/
    ClCoords ptToCoo(PtCoords p) const; /*< Convert virtual point to cell indice*/
    PxCoords cooToPxl(ClCoords p) const;
    ClCoords pxlToCoo(PxCoords p) const;
    PtCoords indToPt(int i, int j) const; /*< Convert to coordinates*/

signals:
    void mapSizeChanged(QSize);
    void viewCenterChanged(QPoint);


private slots:
    void updateMap();           /**< Check if the map has changed*/

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
inline MapPainter::Element operator|(MapPainter::Element a, MapPainter::Element b){return static_cast<MapPainter::Element>(static_cast<int>(a)|static_cast<int>(b));}
inline MapPainter::Element operator&(MapPainter::Element a, MapPainter::Element b){return static_cast<MapPainter::Element>(static_cast<int>(a)&static_cast<int>(b));}
inline MapPainter::Element operator^(MapPainter::Element a, MapPainter::Element b){return static_cast<MapPainter::Element>(static_cast<int>(a)&!static_cast<int>(b));}
// Affichage de certains éléments et pas d'autres.

#endif // MAPPAINTER_H
