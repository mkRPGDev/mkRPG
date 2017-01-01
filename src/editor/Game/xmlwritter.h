#ifndef XMLWRITTER_H
#define XMLWRITTER_H

#include "game.h"
#include <QFileDevice>
#include <QDir>
#include <QXmlStreamReader>
#include <QList>




class XmlWritter{
    enum Element{EndL, Eg, OpenMarkUp, CloseMarkUp, MarkUpParam};
    enum Mode{Default, newMarkUp, MarkUpNamed, ParamName,ParamValue};
public:
    XmlWritter(const QDir &path, Game &game);
    ~XmlWritter();

    const QString fileName() const;


private:
    XmlWritter(const QDir &path, const QString &fileName);
    XmlWritter(const QDir &path, const GameObject *obj);
    XmlWritter(const QDir &path, World &world);
    XmlWritter(const QDir &path, Map &map);
    XmlWritter(const QDir &path, CellType &cellType);
    XmlWritter &operator<<(const GameObject &obj);
    XmlWritter &operator<<(const QString &s);
    XmlWritter &operator<<(const int &i);
    XmlWritter &operator<<(const Element &e);
    XmlWritter &operator<<(const Action *a);

    void writeCell(Cell &c);
    void writeCellType(CellType &c);

    void writeObjectAttributes(const GameObject &obj);


    void writeCreatedFiles(XmlWritter &wr);


    QDir path;
    QFile file;
    QTextStream stream;
    QStack<QString> markUps;


    QMap<QString, QString> createdFiles;
    bool newLine;
    Mode mode;

};


#endif // XMLWRITTER_H
