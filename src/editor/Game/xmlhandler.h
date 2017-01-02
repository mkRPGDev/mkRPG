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
    MU_Maps, MU_Map,
    MU_CellTypes, MU_CellType,MU_Background,
    MU_MapTypes, MU_MapType,
    MU_Cell,
    MU_Parent,

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
            RegMU(Maps),
            RegMU(Map),
            RegMU(CellType),
            RegMU(Parent),
            RegMU(Cell),
            RegMU(Image),
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
