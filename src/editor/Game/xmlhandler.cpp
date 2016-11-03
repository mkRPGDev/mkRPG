#include "xmlhandler.h"

XmlHandler::XmlHandler(Game* g) :
    QXmlDefaultHandler(), g(g), fc(FCUnknown)
{
}


bool XmlHandler::startElement(const QString &, const QString &localName, const QString &, const QXmlAttributes &atts){
    QMap<QString, QString> attrs;
    for(int i(0); i<atts.count(); ++i) attrs[atts.uri(i)] = atts.value(i);
    if(fc == FCUnknown) return recogniseFileContent(localName, attrs);
}

bool XmlHandler::endElement(const QString &, const QString &localName, const QString &){
    return true;
}


bool XmlHandler::recogniseFileContent(const QString &name, const QMap<QString, QString> &atts){

    if(overHead.contains(name)){
        fc = overHead[name];
    }
    else return false;
    return true;
}
