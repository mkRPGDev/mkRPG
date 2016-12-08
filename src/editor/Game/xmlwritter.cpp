#include "xmlwritter.h"





XmlWritter::XmlWritter(const QString &fileName) :
    file(fileName), stream(&file), indent(0), newLine(true)
{
    file.open(QIODevice::WriteOnly);
}

XmlWritter::~XmlWritter(){
    file.close();
}



XmlWritter &XmlWritter::operator << (const Element &elem){
    switch (elem) {
    case EndL:
        newLine = true;
        stream << "\n";
        break;
    case Eg:
        return *this << "=";
    default:
        break;
    }
    return *this;
}

XmlWritter &XmlWritter::operator << (const QString &s){
    beginLine();
    stream << s;
    return *this;
}


XmlWritter &XmlWritter::operator << (const int &i){
    beginLine();
    stream << "\"" << i << "\"";
    return *this;
}



void XmlWritter::beginLine(){
    if(newLine){
        for(int k(0); k<indent; ++k)
            stream << "  ";
        newLine = false;
    }
}

void XmlWritter::openMarkUp(const QString &m){
    openParamMarkUp(m, "");
    endNewMarkUp();
}

void XmlWritter::openParamMarkUp(const QString &m, const QString &end){
    markUps.push(m);
    *this << "<" << m << end;
}

void XmlWritter::endNewMarkUp(){
    *this << ">" << EndL;
    indent++;
}

void XmlWritter::closeMarkUp(){
    assert(!markUps.isEmpty());
    indent--;
    *this << "</" << markUps.pop() << ">" << EndL;
}


XmlWritter &XmlWritter::operator << (const Game &game){
    openMarkUp("Game");
    *this << "Bonjour" << EndL;
    openParamMarkUp("empty");
    *this << "ident" << Eg << 42;
    endNewMarkUp();
    closeMarkUp();
    closeMarkUp();
    return *this;
}
