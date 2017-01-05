#include "xmlreader.h"



XmlTree::XmlTree(QXmlStreamReader &reader) :
    markUp(markUps.value(reader.name().toString(),MU_Invalid))
{
    for(const QXmlStreamAttribute &a : reader.attributes())
        attributes[a.name().toString()] = a.value().toString();
    while(reader.tokenType() != QXmlStreamReader::EndElement){
        reader.readNext();
        switch (reader.tokenType()) {
        case QXmlStreamReader::StartElement:
            subTrees.append(new XmlTree(reader));
            reader.readNext();
            break;
        case QXmlStreamReader::Characters:
            content = reader.text().toString();
            break;
        case QXmlStreamReader::EndDocument:
            return;
        default:
            break;
        }
    }
}


XmlTree *loadFile(const QString &fileName){
    QFile f(fileName);
    f.open(QIODevice::ReadOnly);
    QXmlStreamReader reader(&f);
    reader.readNext();
    if(reader.tokenType() != QXmlStreamReader::StartDocument)
        return nullptr;
    reader.readNext();
    if(reader.tokenType() != QXmlStreamReader::StartElement)
        return nullptr;
    XmlTree *tree = new XmlTree(reader);

    if(reader.hasError() || reader.tokenType() == QXmlStreamReader::EndDocument){
        delete tree;
        return nullptr;
    }
    return tree;
}


void warning(const QString &s){
    qDebug() << "Erreur d'import :" << s;
}


#define Treat(t) readElement(dir,t,game, identCV)
#define ReadCase(elem) case elem: read##elem(dir, tree, game, identCV); break
#define FuncCase(elem) void read##elem(const QDir &dir, const XmlTree &tree, Game *game, QMap<int, int> &identCV)
#define FuncCase2(elem) void read##elem(const QDir &, const XmlTree &tree, Game *game, QMap<int, int> &identCV)


void readAttributes(GameObject &obj, const XmlTree &tree, QMap<int,int> &identCV){
    for(const XmlTree *t : tree.subTrees)
        switch (t->markUp) {
        case MU_Parameter:
            obj.setParam(t->attributes["name"], t->attributes["value"].toInt());
            obj.setParamDomain(t->attributes["name"], t->attributes["min"].toInt(), t->attributes["max"].toInt());
            break;
        case MU_Flag:
            obj.setFlag(t->attributes["name"], t->attributes["value"].toInt());
            break;
        case MU_Event:
            obj.addEvent(t->attributes["name"]);
            break;
        case MU_Order:
            obj.addOrder(t->attributes["name"]);
            break;
        default:
            break;
        }
    obj.setName(tree.attributes["userName"]);
    identCV[tree.attributes["name"].toInt()] = obj.ident();
}

FuncCase(MU_Image){
    Image *i = new Image(*game, dir.path() + "/" + tree.subTrees.first()->content);
    readAttributes(*i, tree, identCV);
    game->addImage(i);
}

FuncCase(MU_File){
    QFileInfo info(dir.path() + "/" + tree.content);
    XmlTree *t = loadFile(info.filePath());
    if(t == nullptr) return warning("Mauvais fichier");
    readElement(info.dir(), *t, game, identCV);
}

FuncCase2(MU_World){
    readAttributes(game->world(), tree, identCV);
}


FuncCase2(MU_CellType){
    XmlTree *parent = tree.subTrees.first();
    CellType *ct;
    if(parent->markUp == MU_Parent){
        CellType *p = static_cast<CellType*>(game->object(identCV[parent->attributes["id"].toInt()]));
        ct = new CellType(*p);
    }
    else
        ct = &game->world().types().cellType();
    readAttributes(*ct, tree, identCV);
    if(tree.subTrees.last()->markUp == MU_Background)
        ct->setImage(&game->image(identCV[tree.subTrees.last()->attributes["id"].toInt()]));

}

FuncCase2(MU_Cell){
    XmlTree *parent = tree.subTrees.at(1);
    Cell &c(game->currentMap()->cell(tree.subTrees.at(0)->attributes["x"].toInt(),
            tree.subTrees.at(0)->attributes["y"].toInt()));

    c.setCellType(*static_cast<CellType*>(game->object(identCV[parent->attributes["id"].toInt()])));
    readAttributes(c,tree, identCV);
}

FuncCase2(MU_MapType){
    XmlTree *parent = tree.subTrees.first();
    if(parent->markUp == MU_Parent){
        MapType *p = static_cast<MapType*>(game->object(identCV[parent->attributes["id"].toInt()]));
        MapType *mt = new MapType(*p);
        readAttributes(*mt, tree, identCV);
    }
    else
        readAttributes(game->world().types().mapType(), tree, identCV);

}

FuncCase(MU_Map){
    int w = tree.subTrees.at(0)->attributes["value"].toInt();
    int h = tree.subTrees.at(1)->attributes["value"].toInt();
    XmlTree *parent = tree.subTrees.at(2);
    Map *m = new Map(*static_cast<MapType*>(game->object(identCV[parent->attributes["id"].toInt()])), game->world());
    game->world().objects().addMap(m);

    m->resize(w, h);
    game->setCurrentMap(m);
    readAttributes(*m, tree, identCV);
    for(XmlTree *t : tree.subTrees)
        Treat(*t);
}


FuncCase2(MU_ObjectType){
    XmlTree *parent = tree.subTrees.first();
    if(parent->markUp == MU_Parent){
        ObjectType *p = static_cast<ObjectType*>(game->object(identCV[parent->attributes["id"].toInt()]));
        ObjectType *ot = new ObjectType(*p);
        readAttributes(*ot, tree, identCV);
    }
    else
        readAttributes(game->world().types().objectType(), tree, identCV);

}

FuncCase(MU_Object){
    XmlTree *parent = tree.subTrees.at(0);
    Object *o = new Object(*static_cast<ObjectType*>(game->object(identCV[parent->attributes["id"].toInt()])), game->world());
    game->world().objects().addObject(o);
    readAttributes(*o, tree, identCV);
    for(XmlTree *t : tree.subTrees)
        Treat(*t);
}


FuncCase2(MU_EntityType){
    XmlTree *parent = tree.subTrees.first();
    if(parent->markUp == MU_Parent){
        EntityType *p = static_cast<EntityType*>(game->object(identCV[parent->attributes["id"].toInt()]));
        EntityType *et = new EntityType(*p);
        readAttributes(*et, tree, identCV);
    }
    else
        readAttributes(game->world().types().entityType(), tree, identCV);

}

FuncCase(MU_Entity){
    XmlTree *parent = tree.subTrees.at(0);
    Entity *e = new Entity(*static_cast<EntityType*>(game->object(identCV[parent->attributes["id"].toInt()])), game->world());
    game->world().objects().addEntity(e);
    readAttributes(*e, tree, identCV);
    for(XmlTree *t : tree.subTrees)
        Treat(*t);
}

void readElement(const QDir &dir, const XmlTree &tree, Game *game, QMap<int, int> &identCV){
    qDebug() << "Lecture" << tree.markUp << tree.attributes;
    switch (tree.markUp) {
    ReadCase(MU_File);
    ReadCase(MU_World);
    ReadCase(MU_CellType);
    ReadCase(MU_Cell);
    ReadCase(MU_MapType);
    ReadCase(MU_Map);
    ReadCase(MU_ObjectType);
    ReadCase(MU_Object);
    ReadCase(MU_EntityType);
    ReadCase(MU_Entity);
    ReadCase(MU_Image);
    default:
        for(const XmlTree *t : tree.subTrees)
            Treat(*t);
    }
}




Game* importGame(const QString &fileName){
    XmlTree *t = loadFile(fileName);
    if(t == nullptr) return nullptr;
    XmlTree tree(*t);
    QFileInfo info(fileName);
    QDir dir(info.path());
    Game *game = new Game();
    QMap<int,int> identCV;
    Treat(tree);
    qDebug() << "fin";

    return game;
}




