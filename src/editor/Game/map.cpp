#include "map.h"



CellType::CellType(CellType &ancestor) :
    Type(ancestor)
{
    aImage = ancestor.image();
    if(aImage)
        aImage->addReference();
}


CellType::CellType(GameObject &parent) :
    Type(parent)
{
    aImage = nullptr;
    //aName = QObject::tr("Cell_type", "name of a CellType");
    SetFlag(walkable,true);

    setParam("humidity", 42);
    setParam("boue", 23);
    SetFlag(accessible, true);
}





CellType* Cell::defaultCellType = nullptr;
GameObject* Cell::defaultParent = nullptr;

Cell::Cell(CellType &type, GameObject &parent) :
    TypedObject(type, parent),
    select(false), nbSel(0), selectMod(false)
{}

Cell::Cell() :
    Cell(*defaultCellType, *defaultParent)
{}

const CellType &Cell::cellType() const{
    return objectType();
}

void Cell::setCellType(CellType &type){
    setObjectType(type);
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


Cell* Cell::cellArray(CellType &type, GameObject &parent, int n){
    QMutexLocker(&Cell::sync);
    defaultCellType = &type;
    defaultParent = &parent;
    return new Cell[n];
}










Map::Map(GameObject &parent) :
    GameObject(parent),
    cells(nullptr)
{
    resize(100,75);
    setAngleXMax(900);
    setAngleYMax(900);
    SetParam(angleX, 0);
    SetParam(angleY, 0);
    SetFlag(inutile, false);
    ProtectParam(height);
    //SetParam(znull,42);
    ProtectParam(width);
}

Map::~Map(){
    forCells(i) cells[i].setParent(nullptr);
    delete[] cells;
}


// TODO temporaire
#include "game.h"
void Map::resize(int w, int h){
    if(cells) delete[] cells;
    cells = Cell::cellArray(*game->world()->cellTypes().first(),*this, w*h);
    SetParam(width, w);
    SetParam(height, h);
    wi = w;
    he = h;
    touch();
}

void Map::setWidth(int w){
    resize(w, height());
}

void Map::setHeight(int h){
    resize(width(), h);
}

int Map::width() const{
    return wi;
}

int Map::height() const{
    return he;
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

QList<GameObject*> Map::children() const{
    QList<GameObject*> l = GameObject::children();
    l.erase(std::remove_if(l.begin(), l.end(), [this](GameObject* o){return o>=cells && o<cells+sizeof(Cell)*width()*height();}),l.end());
    return l;
}
