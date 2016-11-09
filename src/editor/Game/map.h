#ifndef MAP_H
#define MAP_H

#include "object.h"

/**
 * \file map.h
 * \brief Definition of the Map, Cell and CellType classes.
 *
 *
 */


/*!
 * \brief The CellType class
 */
class CellType : public GameObject
{
public:
    CellType(Game *g, GameObject *parent);
    C0(AttrT, i,I,mage)
private:


};




/*!
 * Usefull macro to set up a for on the cells
 */
#define forCells(i) int nbCell = width()*height(); for(int i(0); i<nbCell; ++i)

/*!
 * \brief The Cell class
 */
class Cell : public GameObject
{
public:
    Cell(Game* g = nullptr, GameObject *parent = nullptr);
    bool isSelected() const;
    void setSelected(bool s = true);
    void invertSelected();


    void addSelection();
    bool isPreSelected() const;
    void confirmPreSelection(bool add = true);
    void clearPreSelection();


    C0(Flag, a,A,ccessible)

    C0(AttrT,c,C,ellType)
    ObjectListD(o,O,bject,,s,Object)
//    ObjectsMap(c,o,O,bject,,s)
private:
    bool select;
    int nbSel;
    bool selectMod;
};



/*!
 * \brief The Map class
 */
class Map : public GameObject
{
public:
    Map(Game* g, GameObject *parent);
    ~Map();
    ParamGetter(width)
    ParamGetter(height)
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

    void confirmPreSelection(bool add = true);
    void clearPreSelection();

private:
    Cell* cells;

};






#endif // MAP_H