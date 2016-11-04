#ifndef MAP_H
#define MAP_H

#include "object.h"


class CellType : public Object
{
public:
    CellType(Game *g, Object *parent);
    ParamObj(image,Image,ct)
private:


};




/*!
 * Usefull macro to set up a for on the cells
 */
#define forCells int nbCell = width()*height(); for(int i(0); i<nbCell; ++i)

/*!
 * \brief The Cell class
 */
class Cell : public Object
{
public:
    Cell(Game* g = nullptr, Object *parent = nullptr);
    bool isSelected() const;
    void setSelected(bool s = true);
    void invertSelected();


    void addSelection();
    bool isPreSelected() const;
    void confirmPreSelection(bool add = true);
    void clearPreSelection();

    ParamObj(cellType, CellType, c)
    ObjectsMap(c,o,O,bject,,s)
private:
    bool select;
    int nbSel;
    bool selectMod;
};




class Map : public Object
{
public:
    Map(Game* g, Object *parent);
    Getter(width)
    Getter(height)
    QSize size() const{return QSize(width(),height());}
    void setWidth(int w);
    void setHeight(int h);
    void resize(int w, int h);
    Param(angleX,AngleX)
    Param(angleY,AngleY)
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
