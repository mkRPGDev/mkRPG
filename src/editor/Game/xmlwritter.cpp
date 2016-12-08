#include "xmlwritter.h"





XmlWritter::XmlWritter(const QString &fileName) :
    file(fileName), stream(&file), newLine(true), mode(Default)
{
    file.open(QIODevice::WriteOnly);
}

XmlWritter::~XmlWritter(){
    file.close();
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




XmlWritter &XmlWritter::operator << (Game &game){
    *this << OpenMarkUp << "Game" << EndL;
    *this << OpenMarkUp << "World" << EndL;
    *this << OpenMarkUp << "World" << game.world().name() << CloseMarkUp;
    *this << OpenMarkUp << "World2" << MarkUpParam << "la" << "reprise" << game.world().name() << CloseMarkUp;
    *this << OpenMarkUp << "tentative" << MarkUpParam << "hum" << 42 << CloseMarkUp;
    *this << CloseMarkUp << CloseMarkUp;
    return *this;
}
