#include "xmlwritter.h"




XmlWritter::XmlWritter(const QDir &path, const GameObject * obj) :
    path(path), stream(&file), newLine(true), mode(Default)
{
    file.setFileName(path.filePath(obj->name()+".xml"));
    int k(1);
    while(file.exists())
        file.setFileName(path.filePath(obj->name()+QString::number(++k)+".xml"));
    file.open(QIODevice::WriteOnly);
    *this << *obj;
}

XmlWritter::XmlWritter(const QDir &path, const QString &fileName) :
    path(path), stream(&file), newLine(true), mode(Default)
{
    file.setFileName(path.filePath(fileName+".xml"));
    int k(1);
    while(file.exists())
        file.setFileName(path.filePath(fileName+QString::number(++k)+".xml"));
    file.open(QIODevice::WriteOnly);
}




XmlWritter::XmlWritter(const QDir &path, Game &game) :
    XmlWritter(path, &game)
{
    *this << OpenMarkUp << "Files" << EndL;
    XmlWritter world(path, game.world());
    *this << OpenMarkUp << "World" << path.relativeFilePath(world.fileName()) << CloseMarkUp;
    writeCreatedFiles(world);
    *this << CloseMarkUp;


    XmlWritter actions(path, "actions");
    actions << OpenMarkUp << "Actions";
    createdFiles.insertMulti("Action", actions.fileName());
    for(QString &a : game.actions())
        actions << game.action(a);
    *this << OpenMarkUp << "Actions";
    writeCreatedFiles(*this);
    *this << CloseMarkUp;
}

XmlWritter::XmlWritter(const QDir &path, World &world) :
    XmlWritter(path, &world)
{
    writeObjectAttributes(world);
    *this << OpenMarkUp << "Maps" << EndL;
    XmlWritter::path.mkdir("Maps");
    XmlWritter::path.cd("Maps");
    for(Map *m : world.objects().maps()){
        *this << OpenMarkUp << "Map" << MarkUpParam << "id" << m->ident() << CloseMarkUp;
        XmlWritter map(XmlWritter::path, *m);
        createdFiles.insertMulti("Map", map.fileName());
    }
    XmlWritter ct(XmlWritter::path, world.types().cellType());
    createdFiles.insertMulti("CellType", ct.fileName());

    XmlWritter::path.cdUp();

    *this << CloseMarkUp;
}

XmlWritter::XmlWritter(const QDir &path, Map &map) :
    XmlWritter(path, &map)
{
    writeObjectAttributes(map);
    int w = map.width();
    int h = map.height();
    for(int i(0); i<w; ++i)
        for(int j(0); j<h; ++j)
            writeCell(map.cell(i,j));
}

XmlWritter::XmlWritter(const QDir &path, CellType &cellType) :
    XmlWritter(path, "celltypes")
{
    *this << OpenMarkUp << "CellTypes" << EndL;
    writeCellType(cellType);
}


XmlWritter::~XmlWritter(){
    *this << CloseMarkUp;
    file.close();
}

void XmlWritter::writeCell(Cell &c){
    *this << c;
    *this << OpenMarkUp << "CellType" << MarkUpParam << "id" << c.cellType().ident() << CloseMarkUp;
    *this << CloseMarkUp;
}

void XmlWritter::writeCellType(CellType &c){
    *this << c;
    if(c.hasAncestor())
        *this << OpenMarkUp << "Inherits" << MarkUpParam << "id" << c.ancestor()->ident() << CloseMarkUp;
    writeObjectAttributes(c);
    *this << CloseMarkUp;
    for(GameObjectType *ct : c.descendants())
        writeCellType(*static_cast<CellType*>(ct));
}

void XmlWritter::writeObjectAttributes(const GameObject &obj){
    /*for(QString f : obj.flags())
        *this << OpenMarkUp << "Flag" << MarkUpParam << f << obj.getFlag(f) << CloseMarkUp;
    for(QString f : obj.params())
        *this << OpenMarkUp << "Flag" << MarkUpParam << f << obj.getParam(f)
                            << MarkUpParam << "min" << obj.getParamMin(f)
                            << MarkUpParam << "max" << obj.getParamMax(f) << CloseMarkUp;*/
    *this << OpenMarkUp << "Params" << EndL;
    for(QString f : obj.flags())
        *this << OpenMarkUp << f << (obj.getFlag(f) ? "True" : "False") << CloseMarkUp;
    for(QString p : obj.params())
        *this << OpenMarkUp << p << MarkUpParam << "min" << obj.getParamMin(p)
                            << MarkUpParam << "max" << obj.getParamMax(p)
                            << obj.getParam(p) << CloseMarkUp;
    *this << CloseMarkUp;
}


void XmlWritter::writeCreatedFiles(XmlWritter &wr){
    for(QString type : wr.createdFiles.uniqueKeys())
        for(QString file : wr.createdFiles.values(type))
            *this << OpenMarkUp << type << path.relativeFilePath(file) << CloseMarkUp;
    wr.createdFiles.clear();
}

const QString XmlWritter::fileName() const{
    return file.fileName();
}

XmlWritter &XmlWritter::operator << (const Element &elem){
    switch (elem) {
    case EndL:
        if(!newLine)
            *this << "\n";
        newLine = true;
        break;
    case Eg:
        return *this << "=";
    case OpenMarkUp:
        *this << EndL;
        mode = newMarkUp;
        return *this;
    case CloseMarkUp:{
        assert(!markUps.empty());
        Mode m = mode;
        mode = Default;
        if(m != Default){
            *this << "/>";
            markUps.pop();
        }
        else
            *this << "</" << markUps.pop() << ">";
        return *this << EndL;
    }
    case MarkUpParam:
        mode = ParamName;
        break;
    default:
        break;
    }
    return *this;
}

XmlWritter &XmlWritter::operator << (const QString &s){
    if(newLine){
        int n = markUps.length();
        for(int k(0); k<n; ++k)
            stream << "  ";
        newLine = false;
    }
    switch (mode) {
    case newMarkUp:
        stream << "<" << s;
        markUps.push(s);
        mode = MarkUpNamed;
        break;
    case MarkUpNamed:
        stream << ">";
        stream << s;
        mode = Default;
        break;
    case ParamName:
        mode = ParamValue;
        stream << " " << s;
        break;
    case ParamValue:
        stream << "=\"" << s << "\"";
        mode = MarkUpNamed;
        break;
    default:
        stream << s;
        break;
    }
    return *this;
}


XmlWritter &XmlWritter::operator << (const int &i){
    return *this << QString::number(i);
}



XmlWritter &XmlWritter::operator << (const GameObject &obj){
    return *this << OpenMarkUp << obj.typeName() << MarkUpParam << "name" << obj.ident() << EndL;
}


XmlWritter &XmlWritter::operator <<(const Action *a){
    *this << OpenMarkUp << "Action";
    *this << OpenMarkUp << "Event" << MarkUpParam << "val" << a->event() << CloseMarkUp;
    for(const QPair<GameObject*, QString> &rcv : a->receivers()){
        *this << OpenMarkUp << "Order" << MarkUpParam << "type";
        Order o = rcv.first->getOrder(rcv.second);
        *this << o.typeName() << CloseMarkUp;
    }
    return *this << CloseMarkUp;
}
