#ifndef MAP_H
#define MAP_H

#include "object.h"


class CellType : public BaseObject
{
public:
    CellType(Game *g, BaseObject *parent);
    C0(AttrT, i,I,mage)
private:


};




/*!
 * Usefull macro to set up a for on the cells
 */
#define forCells int nbCell = width()*height(); for(int i(0); i<nbCell; ++i)

/*!
 * \brief The Cell class
 */
class Cell : public BaseObject
{
public:
    Cell(Game* g = nullptr, BaseObject *parent = nullptr);
    bool isSelected() const;
    void setSelected(bool s = true);
    void invertSelected();


    void addSelection();
    bool isPreSelected() const;
    void confirmPreSelection(bool add = true);
    void clearPreSelection();

    C0(AttrT,c,C,ellType)
    ObjectsMap(c,o,O,bject,,s)
private:
    bool select;
    int nbSel;
    bool selectMod;
};




class Map : public BaseObject
{
public:
    Map(Game* g, BaseObject *parent);
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
