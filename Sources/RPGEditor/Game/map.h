#ifndef MAP_H
#define MAP_H

#include "object.h"

class CellType : public Object
{
public:
    CellType(Game *g);
    ParamObj(image,Image,ct)
private:
};

class Cell : public Object
{
public:
    Cell(Game* g = nullptr);
    bool isSelected() const;
    void setSelected(bool s = true);
    void invertSelected();

    ParamObj(cellType, CellType, c)
    ObjectsMap(c,o,O,bject,,s)
private:
    bool select;
};


class Map : public Object
{
public:
    Map(Game* g = nullptr);
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

private:
    Cell* cells;

};


#endif // MAP_H
