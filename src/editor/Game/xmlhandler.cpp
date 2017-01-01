#include "xmlhandler.h"



XmlTree::XmlTree(QXmlStreamReader &reader) :
    name(reader.name().toString())
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

    if(reader.hasError()){
        delete tree;
        return nullptr;
    }
    return tree;
}


#define Treat(t) readElement(dir,t,game)
#define ReadCase(elem) if(tree.name == #elem) read_##elem(dir, tree, game); else
#define FuncCase(elem) void read_##elem(const QDir &dir, const XmlTree &tree, Game *game)


void readAttributes(GameObject &obj, XmlTree &tree){

}



FuncCase(World){
    if(tree.content.isEmpty() || tree.content.startsWith("\n")){

    }
    else{
        XmlTree *t(loadFile(dir.path() + "/" + tree.content));
        if(t==nullptr) return;
        QDir d(dir);
        QFileInfo info(tree.content);
        d.cd(info.path());
        readElement(d, *t, game);
    }
}

FuncCase(Cell){

}

FuncCase(Map){

}

FuncCase(Entities){

}

FuncCase(ObjectTypes){

}

FuncCase(Objects){

}

FuncCase(Actions){

}

FuncCase(Action){

}



void readElement(const QDir &dir, const XmlTree &tree, Game *game){
    ReadCase(World)
    ReadCase(Cell)
    ReadCase(Map)
    ReadCase(Entities)
    ReadCase(ObjectTypes)
    ReadCase(Objects)

    ReadCase(Actions)
    ReadCase(Action)
        for(const XmlTree *t : tree.subTrees)
            Treat(*t);

}




Game* importGame(const QString &fileName){

    XmlTree *t = loadFile(fileName);
    if(t == nullptr) return nullptr;
    XmlTree tree(*t);
    QFileInfo info(fileName);
    QDir dir(info.path());
    Game *game = new Game();
    Treat(tree);
}




