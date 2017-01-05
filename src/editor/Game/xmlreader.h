#ifndef XMLREADER_H
#define XMLREADER_H

#include <QtXml>
#include "game.h"

/*!
 * \file xmlreader.h
 * \brief Definition of the XmlTree class and other related functions
 * to read XML game's files.
 */

/*!
 * \brief The MarkUp enum describes the Mark-ups that can be find in
 * a XML game description file.
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
/*!
 * Association dictionnary to convert mark-ups.
 */
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


/*!
 * \brief The XmlTree class represents a XML file as a tree.
 *
 * To make the use easier, the attributes are public and can be
 * accessed directly.
 */
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


void readElement(const QDir &dir, const XmlTree &tree, Game *game, QMap<int, int> &identCV);    /**<
 * Internal use only. Generic function to read Marks-up.
 */
Game* importGame(const QString &fileName);                                                      /**<
 * Reading of a XML game file.
 */





#endif // XMLREADER_H
