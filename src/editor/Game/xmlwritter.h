#ifndef XMLWRITTER_H
#define XMLWRITTER_H

#include "game.h"
#include <QFileDevice>

class XmlWritter{
    enum Element{EndL, Eg, OpenMarkUp, CloseMarkUp, MarkUpParam};
    enum Mode{Default, newMarkUp, MarkUpNamed, ParamName,ParamValue};
public:
    XmlWritter(const QString &fileName);
    ~XmlWritter();

    XmlWritter &operator<<(Game &game);
    XmlWritter &operator<<(const World &World);

private:
    XmlWritter &operator<<(const QString &s);
    XmlWritter &operator<<(const int &i);
    XmlWritter &operator<<(const Element &e);



    QFile file;
    QTextStream stream;
    QStack<QString> markUps;



    bool newLine;
    Mode mode;

};


#endif // XMLWRITTER_H
