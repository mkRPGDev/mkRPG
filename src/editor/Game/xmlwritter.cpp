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



XmlWritter::XmlWritter(const QDir &path, Game &game) :
    XmlWritter(path, &game)
{
    *this << OpenMarkUp << "Files" << EndL;
    XmlWritter world(path, game.world());
    *this << OpenMarkUp << "World" << path.relativeFilePath(world.fileName()) << CloseMarkUp;
    writeCreatedFiles(world);
    *this << CloseMarkUp;
}

XmlWritter::XmlWritter(const QDir &path, World &world) :
    XmlWritter(path, &world)
{
    *this << OpenMarkUp << "Maps" << EndL;
    XmlWritter::path.mkdir("Maps");
    XmlWritter::path.cd("Maps");
    for(Map *m : world.maps()){
        *this << OpenMarkUp << "Map" << MarkUpParam << "id" << m->ident() << CloseMarkUp;
        XmlWritter map(XmlWritter::path, *m);
        createdFiles.insertMulti("Map", map.fileName());
    }
    XmlWritter::path.cdUp();

    *this << CloseMarkUp;
}

XmlWritter::XmlWritter(const QDir &path, Map &map) :
    XmlWritter(path, &map)
{

    *this << map.cell(0,0) << CloseMarkUp;
}


XmlWritter::~XmlWritter(){
    *this << CloseMarkUp;
    file.close();
}


void XmlWritter::writeCreatedFiles(XmlWritter &wr){
    for(QString type : wr.createdFiles.uniqueKeys())
        for(QString file : wr.createdFiles.values(type))
            *this << OpenMarkUp << type << path.relativeFilePath(file) << CloseMarkUp;
}

const QString XmlWritter::fileName() const{
    return file.fileName();
}

XmlWritter &XmlWritter::operator << (const Element &elem){
    switch (elem) {
    case EndL:
        *this << "\n";
        newLine = true;
        break;
    case Eg:
        return *this << "=";
    case OpenMarkUp:
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

