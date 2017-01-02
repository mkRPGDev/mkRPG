#include "xmlwritter.h"




XmlWritter::XmlWritter(const QDir &path, const GameObject * obj, bool serverXml) :
    path(path), stream(&file), newLine(true), mode(Default), serverXml(serverXml)
{
    file.setFileName(path.filePath(obj->name()+".xml"));
    int k(1);
    while(file.exists())
        file.setFileName(path.filePath(obj->name()+QString::number(++k)+".xml"));
    file.open(QIODevice::WriteOnly);
    *this << *obj;
}

XmlWritter::XmlWritter(const QDir &path, const QString &fileName, bool serverXml) :
    path(path), stream(&file), newLine(true), mode(Default), serverXml(serverXml)
{
    file.setFileName(path.filePath(fileName+".xml"));
    int k(1);
    while(file.exists())
        file.setFileName(path.filePath(fileName+QString::number(++k)+".xml"));
    file.open(QIODevice::WriteOnly);
}




XmlWritter::XmlWritter(const QDir &path, Game &game, bool serverXml) :
    XmlWritter(path, &game, serverXml)
{
    if(serverXml){
        *this << OpenMarkUp << "Files" << EndL;
        XmlWritter world(path, game.world(), serverXml);
        *this << OpenMarkUp << "World" << path.relativeFilePath(world.fileName()) << CloseMarkUp;
        writeCreatedFiles(world);
        *this << CloseMarkUp;


        XmlWritter actions(path, "actions", serverXml);
        actions << OpenMarkUp << "Actions";
        createdFiles.insertMulti("Actions", actions.fileName());
        for(QString &a : game.actions())
            actions << game.action(a);
        *this << OpenMarkUp << "Actions";
        writeCreatedFiles(*this);
        *this << CloseMarkUp;
    }
    else{

        *this << OpenMarkUp << "Files" << EndL;
        XmlWritter images(path, game.images(), serverXml);
        *this << OpenMarkUp << "File" << path.relativeFilePath(images.fileName()) << CloseMarkUp;


        XmlWritter world(path, game.world(), serverXml);
        *this << OpenMarkUp << "File" << path.relativeFilePath(world.fileName()) << CloseMarkUp;
        writeCreatedFiles(world);

        XmlWritter actions(path, "actions", serverXml);
        actions << OpenMarkUp << "Actions";
        createdFiles.insertMulti("Actions", actions.fileName());
        for(QString &a : game.actions())
            actions << game.action(a);
        writeCreatedFiles(*this);
        *this << CloseMarkUp;

    }
}

XmlWritter::XmlWritter(const QDir &path, const QList<Image *> &images, bool serverXml) :
    XmlWritter(path, "ressources", serverXml)
{
    path.mkdir("Ressources");
    *this << OpenMarkUp << "Images" << EndL;
    for(Image *i : images){
        QFileInfo save(path.path() + "/Ressources/" + i->name()+".png");
        for(int k(2); save.exists(); save.setFile(path.path() + "/Ressources/" + i->name()+"_"+QString::number(k++)+".png"));
        *this << *i << OpenMarkUp << "Pict" << path.relativeFilePath(save.filePath()) << CloseMarkUp << CloseMarkUp;
        i->image().save(save.filePath());
    }

}


XmlWritter::XmlWritter(const QDir &path, World &world, bool serverXml) :
    XmlWritter(path, &world, serverXml)
{
    writeObjectAttributes(world);
    *this << OpenMarkUp << "Maps" << EndL;
    XmlWritter::path.mkdir("Maps");
    XmlWritter::path.cd("Maps");
    for(Map *m : world.objects().maps()){
        *this << OpenMarkUp << "Map" << MarkUpParam << "id" << m->ident() << CloseMarkUp;
        XmlWritter map(XmlWritter::path, *m, serverXml);
        createdFiles.insertMulti("Map", map.fileName());
    }
    XmlWritter ct(XmlWritter::path, world.types().cellType(), serverXml);
    createdFiles.insertMulti("CellType", ct.fileName());

    XmlWritter::path.cdUp();

    *this << CloseMarkUp;
}

XmlWritter::XmlWritter(const QDir &path, Map &map, bool serverXml) :
    XmlWritter(path, &map, serverXml)
{
    *this << OpenMarkUp << "Width" << MarkUpParam << "value" << map.width() << CloseMarkUp;
    *this << OpenMarkUp << "Height" << MarkUpParam << "value" << map.height() << CloseMarkUp;
    writeInheritableObject(map);
    int w = map.width();
    int h = map.height();
    for(int i(0); i<w; ++i)
        for(int j(0); j<h; ++j)
            writeCell(map.cell(i,j),i,j);
}

XmlWritter::XmlWritter(const QDir &path, CellType &cellType, bool serverXml) :
    XmlWritter(path, "celltypes", serverXml)
{
    *this << OpenMarkUp << "CellTypes" << EndL;
    writeCellType(cellType);
}


XmlWritter::~XmlWritter(){
    *this << CloseMarkUp;
    file.close();
}

void XmlWritter::writeCell(Cell &c, int x, int y){
    *this << c;
    *this << OpenMarkUp << "Pos" << MarkUpParam << "x" << x << MarkUpParam << "y" << y << CloseMarkUp;
    writeInheritableObject(c);
    *this << CloseMarkUp;
}

void XmlWritter::writeCellType(CellType &c){
    *this << c;
    writeInheritableObject(c);
    if(c.image())
        *this << OpenMarkUp << "Background" << MarkUpParam << "id" << c.image()->ident() << CloseMarkUp;
    *this << CloseMarkUp;
    for(CellType *ct : c.descendants())
        writeCellType(*ct);
}

void XmlWritter::writeInheritableObject(InheritableObject &o){
    if(o.hasAncestor())
        *this << OpenMarkUp << "Parent" << MarkUpParam << "id" << o.ancestor()->ident() << CloseMarkUp;
    writeObjectAttributes(o);
}

void XmlWritter::writeObjectAttributes(const GameObject &obj){
    if(serverXml){
        *this << OpenMarkUp << "Params" << EndL;
        for(QString f : obj.flags())
            *this << OpenMarkUp << f << (obj.getFlag(f) ? "True" : "False") << CloseMarkUp;
        for(QString p : obj.params())
            *this << OpenMarkUp << p << MarkUpParam << "min" << obj.getParamMin(p)
                                << MarkUpParam << "max" << obj.getParamMax(p)
                                << obj.getParam(p) << CloseMarkUp;
        *this << CloseMarkUp;
    }
    else{
        for(QString f : obj.GameObject::flags())
            *this << OpenMarkUp << "Flag" << MarkUpParam << "name" << f << MarkUpParam << "value" << obj.getFlag(f) << CloseMarkUp;
        for(QString p : obj.GameObject::params())
            *this << OpenMarkUp << "Flag" << MarkUpParam << "name" << p << MarkUpParam << "value" << obj.getParam(p)
                                << MarkUpParam << "min" << obj.getParamMin(p)
                                << MarkUpParam << "max" << obj.getParamMax(p) << CloseMarkUp;
        for(QString e : obj.GameObject::events())
            *this << OpenMarkUp << "Event" << MarkUpParam << "name" << e
                                << CloseMarkUp;
        for(QString o : obj.GameObject::orders())
            *this << OpenMarkUp << "Order" << MarkUpParam << "name" << o
                                << CloseMarkUp;
    }
}


void XmlWritter::writeCreatedFiles(XmlWritter &wr){
    for(QString type : wr.createdFiles.uniqueKeys())
        for(QString file : wr.createdFiles.values(type))
            *this << OpenMarkUp << (serverXml ? type : "File") << path.relativeFilePath(file) << CloseMarkUp;
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
    if(serverXml)
        return *this << OpenMarkUp << obj.typeName() << MarkUpParam << "name" << obj.ident() << EndL;
    return *this << OpenMarkUp << obj.typeName() << MarkUpParam << "name" << obj.ident() << MarkUpParam << "userName" << obj.name() << EndL;
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
