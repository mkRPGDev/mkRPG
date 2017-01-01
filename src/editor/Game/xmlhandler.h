#ifndef XMLHANDLER_H
#define XMLHANDLER_H

#include <QtXml>
#include "game.h"

/*!
 * \file xmlhandler.h
 * \brief Definition og the XmlHandler class and other related classes
 * to read XML game's files.
 */



class XmlTree
{
public:
    XmlTree(QXmlStreamReader &reader);

//private:
    QString name;
    QList<XmlTree*> subTrees;
    QString content;
    QMap<QString, QString> attributes;
};


void readElement(const QDir &dir, const XmlTree &tree, Game *game);
Game* importGame(const QString &fileName);


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




#endif // XMLHANDLER_H
