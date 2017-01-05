#ifndef XMLWRITTER_H
#define XMLWRITTER_H

#include "game.h"
#include <QFileDevice>
#include <QDir>
#include <QXmlStreamReader>
#include <QList>


/*!
 * \file xmlwritter.h
 * \brief Definition of the XmlWritter class.
 */

/*!
 * \brief The XmlWritter class provide a XML writter.
 *
 * Two types of XML can be outputed :
 * - Server XML : server-readable XML (Some modifications of the Game architecture
 * are made to fit server capacities)
 * - Editor XML : editor-readable XML (The elements are stored in a different way,
 * and additionnal information is written)
 */
class XmlWritter{
    enum Element{EndL, Eg, OpenMarkUp, CloseMarkUp, MarkUpParam};
    enum Mode{Default, newMarkUp, MarkUpNamed, ParamName,ParamValue};
public:
    XmlWritter(const QDir &path, Game &game, bool serverXml = true);
    ~XmlWritter();

    const QString fileName() const;


private:
    XmlWritter(const QDir &path, const QString &fileName, bool serverXml = true);
    XmlWritter(const QDir &path, const GameObject *obj, bool serverXml = true);
    XmlWritter(const QDir &path, World &world, bool serverXml = true);
    XmlWritter(const QDir &path, CellType &cellType, bool serverXml = true);
    XmlWritter(const QDir &path, MapType &mapType, bool serverXml = true);
    XmlWritter(const QDir &path, Map &map, bool serverXml = true);
    XmlWritter(const QDir &path, ObjectType &objectType, bool serverXml = true);
    XmlWritter(const QDir &path, Object &object, bool serverXml = true);
    XmlWritter(const QDir &path, EntityType &entityType, bool serverXml = true);
    XmlWritter(const QDir &path, Entity &entity, bool serverXml = true);
    XmlWritter(const QDir &path, const QList<Image *> &images, bool serverXml = true);
    XmlWritter &operator<<(const GameObject &obj);
    XmlWritter &operator<<(const QString &s);
    XmlWritter &operator<<(const int &i);
    XmlWritter &operator<<(const Element &e);
    XmlWritter &operator<<(const Action *a);

    void writeCell(Cell &c, int x, int y);
    void writeCellType(CellType &c);
    void writeMapType(MapType &m);
    void writeObjectType(ObjectType &o);
    void writeEntityType(EntityType &e);
    void writeInheritableObject(InheritableObject &o);


    void writeObjectAttributes(const GameObject &obj);


    void writeCreatedFiles(XmlWritter &wr);


    QDir path;
    QFile file;
    QTextStream stream;
    QStack<QString> markUps;


    QList<QPair<QString, QString>> createdFiles;
    bool newLine;
    Mode mode;


    bool serverXml;
};





#endif // XMLWRITTER_H
