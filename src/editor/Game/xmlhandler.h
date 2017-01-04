#ifndef XMLHANDLER_H
#define XMLHANDLER_H

#include <QtXml>
#include "game.h"

/*!
 * \file xmlhandler.h
 * \brief Definition og the XmlHandler class and other related classes
 * to read XML game's files.
 */


enum FileContent{FCUnknown, FCGame, FCRessources, FCWorld, FCMap, FCEntity, FCObject};

typedef std::pair<QString,FileContent> Asso;
const QMap<QString, FileContent>
overHead({
             Asso("Game",FCGame),
             Asso("Ressources",FCRessources),
             Asso("World",FCWorld),
             Asso("Map",FCMap),
             Asso("Entity",FCEntity),
             Asso("Object",FCObject)
         });


class XmlHandler : public QXmlDefaultHandler
{
public:
    XmlHandler(Game *g);
    bool startElement(const QString &, const QString &localName, const QString &, const QXmlAttributes &atts);
    bool endElement(const QString &, const QString &localName, const QString &);


private:
    bool recogniseFileContent(const QString &name, const QMap<QString, QString>& atts);

    QString err;
    Game *g;
    FileContent fc;

};

#endif // XMLHANDLER_H
