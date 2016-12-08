#ifndef XMLWRITTER_H
#define XMLWRITTER_H

#include "game.h"
#include <QFileDevice>

class XmlWritter{
    enum Element{EndL, Eg};
public:
    XmlWritter(const QString &fileName);
    ~XmlWritter();

    XmlWritter &operator<<(const Game &game);

private:
    XmlWritter &operator<<(const QString &s);
    XmlWritter &operator<<(const int &i);
    XmlWritter &operator<<(const Element &e);

    void openMarkUp(const QString &m);
    void openParamMarkUp(const QString &m, const QString &end = " ");
    void endNewMarkUp();
    void closeMarkUp();
    void beginLine();

    QFile file;
    QTextStream stream;
    QStack<QString> markUps;

    int indent;

    bool newLine;

};


#endif // XMLWRITTER_H
