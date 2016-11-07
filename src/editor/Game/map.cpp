#include "map.h"



CellType::CellType(Game *g, BaseObject *parent) :
    BaseObject(g, parent)
{

}





Cell::Cell(Game* g, BaseObject *parent) :
    BaseObject(g, parent),
    select(false), nbSel(0), selectMod(false)
{
    setCellType(nullptr);
}

void Cell::setSelected(bool s){
    select = s;
    selectMod = true;
}

void Cell::invertSelected(){
    select = !select;
    selectMod = true;
}

bool Cell::isSelected() const{
    return select;
}



void Cell::addSelection(){
    ++nbSel;
}

bool Cell::isPreSelected() const{
    return nbSel%2 && !selectMod;
}

void Cell::confirmPreSelection(bool add){
    select = add ? select|(nbSel%2) : select&!(nbSel%2);
    clearPreSelection();
    selectMod = false;
}

void Cell::clearPreSelection(){
    nbSel = 0;
    selectMod = false;
}



Map::Map(Game *g, BaseObject *parent) :
    BaseObject(g, parent),
    cells(nullptr)
{
    resize(100,75);
    SetParam(angleX, 0);
    SetParam(angleY, 0);
}

void Map::resize(int w, int h){
    if(cells) delete[] cells;
    cells = new Cell[w*h];
    for(int i(0); i<w*h; cells[i++].init(game, parent));
    SetParam(width, w);
    SetParam(height, h);
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
    touch();
}

void Map::unSelectAll(){
    forCells cells[i].setSelected(false);
    touch();
}

void Map::clearPreSelection(){
    forCells cells[i].clearPreSelection();
}

void Map::confirmPreSelection(bool add){
    forCells cells[i].confirmPreSelection(add);
}
