#include "map.h"



CellType::CellType(CellType &ancestor) :
    Type(ancestor)
{
    aImage = ancestor.image();
    if(aImage)
        aImage->addReference();
}


CellType::CellType(DefaultTypes &parent) :
    Type(parent)
{
    setName(QObject::tr("CellType"));
    setName(typeName());
    aImage = nullptr;
    //aName = QObject::tr("Cell_type", "name of a CellType");
    SetFlag(walkable,true);

    setParam("humidity", 42);
    setParam("boue", 23);
    SetFlag(accessible, false);
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


void Cell::operator=(Cell &c){
    setCellType(c.cellType());
    copy(c);
}

CellType &Cell::cellType(){
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





MapType::MapType(MapType &ancestor) :
    Type(ancestor)
{}

MapType::MapType(DefaultTypes &parent) :
    Type(parent)
{
    setName(typeName());
    SetParam(angleX, 0);
    SetParam(angleY, 0);
    setAngleXMax(900);
    setAngleYMax(900);
    SetFlag(inutile, false);
}







Map::Map(MapType &type, GameObject &parent) :
    TypedObject(type, parent),
    cells(nullptr), wi(0), he(0)
{
    resize(100,75);
    ProtectParam(height);
    ProtectParam(width);
    //SetParam(znull,42);
}

Map::~Map(){
    forCells(i) cells[i].setParent(nullptr);
    delete[] cells;
}


// TODO temporaire
#include "game.h"
void Map::resize(int w, int h, int xOffset, int yOffset){
    Cell* oCells = cells;
    cells = Cell::cellArray(game->world().types().cellType(),*this, w*h);
    for(int i(0); i<w; ++i)
        for(int j(0); j<h; ++j)
            cells[i+w*j].setName(QObject::tr("Cell_<")+QString::number(i)+","+QString::number(j)+">");
    for(int i(std::max(0, xOffset)); i<std::min(wi, xOffset+w); ++i)
        for(int j(std::max(0, yOffset)); j<std::min(he, yOffset+h); ++j)
            cells[i-xOffset + (j-yOffset)*w] = oCells[i+j*wi];
    SetParam(width, w);
    SetParam(height, h);
    wi = w;
    he = h;
    if(oCells) delete[] oCells;
    touch();
}

void Map::resize(const QRect &newSize){
    resize(newSize.width(), newSize.height(), newSize.left(), newSize.top());
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
