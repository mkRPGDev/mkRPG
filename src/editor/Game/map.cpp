#include "map.h"



CellType::CellType(Game *g, Object *parent) :
    Object(g, parent)
{

}





Cell::Cell(Game* g, Object *parent) :
    Object(g, parent),
    select(false), nbSel(0)
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



void Cell::addSelection(){
    ++nbSel;
}

bool Cell::isPreSelected() const{
    return nbSel%2;
}

void Cell::confirmPreSelection(bool add){
    select = add ? select|(nbSel%2) : select&!(nbSel%2);
    clearPreSelection();
}

void Cell::clearPreSelection(){
    nbSel = 0;
}



Map::Map(Game *g, Object *parent) :
    Object(g, parent),
    cells(nullptr)
{
    resize(100,75);
    ParamDef(angleX, 0);
    ParamDef(angleY, 0);
}

void Map::resize(int w, int h){
    if(cells) delete[] cells;
    cells = new Cell[w*h];
    for(int i(0); i<w*h; cells[i++].init(game, parent));
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


void Map::selectAll(){
    forCells cells[i].setSelected(true);
    Editing;
}

void Map::unSelectAll(){
    forCells cells[i].setSelected(false);
    Editing;
}

void Map::clearPreSelection(){
    forCells cells[i].clearPreSelection();
}

void Map::confirmPreSelection(bool add){
    forCells cells[i].confirmPreSelection(add);
}
