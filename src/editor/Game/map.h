#ifndef MAP_H
#define MAP_H

#include "object.h"
#include <QMutex>
#include <QMutexLocker>

/**
 * \file map.h
 * \brief Definition of the Map, Cell and CellType classes.
 *
 *
 */


/*!
 * \brief The CellType class
 */
class CellType;
class CellType : public Type<CellType>
{
public:
    TypeName(CellType)
    CellType(CellType &ancestor);
    CellType(GameObject &parent);


    C0(AttrT, i,I,mage)
    C0(Flag,w,W,alkable)
private:

};




/*!
 * Usefull macro to set up a for on the cells
 */
#define forCells(i) int nbCell = width()*height(); for(int i(0); i<nbCell; ++i)

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


/*!
 * \brief The Map class
 */
class Map : public GameObject
{
public:
    TypeName(Map)
    Map(GameObject &parent);
    ~Map();

    int width() const;
    int height() const;
    QSize size() const{return QSize(width(),height());}
    void setWidth(int w);
    void setHeight(int h);
    void resize(int w, int h);
    C0(Param,a,A,ngleX)
    C0(Param,a,A,ngleY)
    Cell& cell(int i, int j) const;
    Cell& cell(const QPoint &p) const;
    void selectAll();
    void unSelectAll();

    C0(Flag, i,I,nutile)


    void confirmPreSelection(bool add = true);
    void clearPreSelection();
    QList<GameObject*> children() const;

private:
    Cell* cells;
    int wi,he;

};






#endif // MAP_H
