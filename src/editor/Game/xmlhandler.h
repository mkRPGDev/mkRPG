#ifndef XMLHANDLER_H
#define XMLHANDLER_H

#include <QtXml>
#include "game.h"

/*!
 * \file xmlhandler.h
 * \brief Definition og the XmlHandler class and other related classes
 * to read XML game's files.
 */

enum MarkUp{
    MU_Invalid,
    MU_Files, MU_File,
    MU_Game, MU_World,
    MU_Background, MU_Parent,
    MU_MapType, MU_Map,
    MU_CellType, MU_Cell,
    MU_ObjectType, MU_Object,
    MU_EntityType, MU_Entity,

    MU_Image,
    MU_Parameter, MU_Flag,
    MU_Event, MU_Order,
};

#define RegMU(MU) std::pair<QString,MarkUp>(#MU, MU_##MU)

typedef std::pair<QString,MarkUp> Asso;
const QMap<QString, MarkUp>
markUps({
            RegMU(Files),
            RegMU(File),
            RegMU(Game),
            RegMU(World),
            RegMU(CellType),
            RegMU(MapType),
            RegMU(Map),
            RegMU(ObjectType),
            RegMU(Object),
            RegMU(EntityType),
            RegMU(Entity),
            RegMU(Parent),
            RegMU(Cell),
            RegMU(Image),
            RegMU(Order),
            RegMU(Event),
            RegMU(Background),
         });


class XmlTree
{
public:
    XmlTree(QXmlStreamReader &reader);

//private:
    MarkUp markUp;
    QList<XmlTree*> subTrees;
    QString content;
    QMap<QString, QString> attributes;
};


void readElement(const QDir &dir, const XmlTree &tree, Game *game, QMap<int, int> &identCV);
Game* importGame(const QString &fileName);


enum FileContent{FCUnknown, FCGame, FCRessources, FCWorld, FCMap, FCEntity, FCObject};






#endif // XMLHANDLER_H
