#ifndef MAP_H
#define MAP_H

#include "object.h"
#include <QMutex>
#include <QMutexLocker>

/*!
 * \file map.h
 * \brief Definition of the Map, Cell and CellType classes.
 *
 *
 */


class CellType;/*!
 * \brief The CellType class defines the properties that are
 * mandatory in any CellType object.
 */
class CellType : public Type<CellType>
{
public:
    TypeName(CellType)
    CellType(CellType &ancestor);
    CellType(DefaultTypes &parent);


    C0(AttrT, i,I,mage)
    C0(Flag,w,W,alkable)
private:

};






/*!
 * \brief The Cell class
 */
class Cell : public TypedObject<CellType>
{
public:
    TypeName(Cell)
    Cell(CellType &type, GameObject &parent);

    bool isSelected() const;
    void setSelected(bool s = true);
    void invertSelected();


    void addSelection();
    bool isPreSelected() const;
    void confirmPreSelection(bool add = true);
    void clearPreSelection();

    const CellType &cellType() const;
    void setCellType(CellType &type);

    C0(Flag, a,A,ccessible)

    ObjectListD(o,O,bject,,s,Object)

//    ObjectsMap(c,o,O,bject,,s)

    // TODO destruction
    static Cell* cellArray(CellType &type, GameObject &parent, int n);
private:
    Cell();
    bool select;
    int nbSel;
    bool selectMod;

    static QMutex sync;
    static CellType *defaultCellType;
    static GameObject *defaultParent;
};

/*class MapType;
class MapType : public Type<MapType>
{

};*/




class MapType;
class MapType : public Type<MapType>
{
public:
    TypeName(MapType)
    MapType(MapType &ancestor);
    MapType(DefaultTypes &parent);
    C0(Param,a,A,ngleX)
    C0(Param,a,A,ngleY)
};




/*!
 * Usefull macro to set up a for on the cells
 */
#define forCells(i) int nbCell = width()*height(); for(int i(0); i<nbCell; ++i)

/*!
 * \brief The Map class
 */
class Map : public TypedObject<MapType>
{
public:
    TypeName(Map)
    Map(MapType &type, GameObject &parent);
    ~Map();

    int width() const;
    int height() const;
    QSize size() const{return QSize(width(),height());}
    void setWidth(int w);
    void setHeight(int h);
    void resize(int w, int h);
    C0(Param,a,A,ngleX)
    C0(Param,a,A,ngleY)
    Cell& cell(int i, int j) const;                     /**<
     * Returns a reference to the cell at pos (x,y).
     */
    Cell& cell(const QPoint &p) const;                  /**<
     * \overload
     */
    void selectAll();                                   /**<
     * Unselects all the \ref Cell "cells" of the map.
     *
     * \see unSelectAll
     */
    void unSelectAll();                                 /**<
     * Selects all the \ref Cell "cells" of the map.
     *
     * \see selectAll
     */



    void confirmPreSelection(bool add = true);          /**<
     * Converts the pre-selection into selection for every \ref Cell "cell".
     *
     * \see clearPreSelection
     */
    void clearPreSelection();                           /**<
     * Resets the pre-selection information on every \ref Cell "cell".
     *
     * \see confirmPreSelection;
     */
    QList<GameObject*> children() const;                /**<
     * Reimplemented from GameObject to remove the \ref Cell "cells"
     * from the children list.
     *
     * \see GameObject::children
     *
     */

private:
    Cell* cells;
    int wi,he;

};






#endif // MAP_H
