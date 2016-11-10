#include "map.h"



CellType::CellType(Game *g, GameObject *parent) :
    GameObject(g, parent)
{
    //aName = QObject::tr("Cell_type", "name of a CellType");
}





Cell::Cell(Game* g, GameObject *parent) :
    GameObject(g, parent),
    select(false), nbSel(0), selectMod(false)
{
    setCellType(nullptr);
    SetFlag(accessible, true);
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



Map::Map(Game *g, GameObject *parent) :
    GameObject(g, parent),
    cells(nullptr)
{
    resize(100,75);
    SetParam(angleX, 0);
    SetParam(angleY, 0);

    SetFlag(inutile, false);
}

Map::~Map(){
    forCells(i) cells[i].setParent(nullptr);
    delete[] cells;
}

void Map::resize(int w, int h){
    if(cells) delete[] cells;
    cells = new Cell[w*h];
    for(int i(0); i<w*h; cells[i++].init(game, this));
    SetParam(width, w);
    SetParam(height, h);
    touch();
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
    forCells(i) cells[i].setSelected(true);
    touch();
}

void Map::unSelectAll(){
    forCells(i) cells[i].setSelected(false);
    touch();
}

void Map::clearPreSelection(){
    forCells(i) cells[i].clearPreSelection();
}

void Map::confirmPreSelection(bool add){
    forCells(i) cells[i].confirmPreSelection(add);
}
