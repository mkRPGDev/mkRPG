#include "map.h"





Cell::Cell(Game* g) :
    Object(g),
    select(false)
{
    setCellType(nullptr);
}

void Cell::setSelected(bool s){
    select = s;
}

void Cell::invertSelected(){
    select = !select;
}

bool Cell::isSelected() const{
    return select;
}





Map::Map(Game *g) :
    Object(g),
    cells(nullptr)
{
    resize(100,75);
    ParamDef(angleX, 0);
    ParamDef(angleY, 0);
}

void Map::resize(int w, int h){
    if(cells) delete[] cells;
    cells = new Cell[w*h];
    for(int i(0); i<w*h; cells[i++].init(game));
    ParamDef(width, w);
    ParamDef(height, h);
}

void Map::setWidth(int w){
    resize(w, height());
}

void Map::setHeight(int h){
    resize(width(), h);
}

Cell& Map::cell(int i, int j) const{
    assert(i>=0 && i<width() && j>=0 && j<height());
    return cells[i+j*width()];
}
Cell& Map::cell(const QPoint &p) const{
    return cell(p.x(), p.y());
}
