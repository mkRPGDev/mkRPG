#include "options.h"

Options *Options::instance = NULL;

Options &Options::options(){
    if(!instance) instance = new Options;
    return *instance;
}


Options::Options(){
    Default(WIN, "Position", QPoint(0,0)); // TODO pos...
    Default(WIN, "Size", QSize(1000,742));
    Default(WIN, "Maximized", false);
    Default(MAP, "DocksVisible", true);
    Default(MAP, "DocksLength", 200);
    Default(MAP, "SelColor", QColor(255,180,0,60));
    Default(MAP, "PreSelColor", QColor(255,0,0,60));

    Default(DIR, "New", QDir::homePath());
    Default(DIR, "Open", QDir::homePath());
    Default(DIR, "Save",QDir::homePath());
}

bool Options::isAdjustable(QString group, QString opt, bool adjust){
    a.beginGroup(group);
    if(!a.contains(opt+ADAPT))
        a.setValue(opt+ADAPT, adjust);
    bool val = a.value(opt+ADAPT).toBool();
    a.endGroup();
    return val;
}

void Options::setAdjustable(QString group, QString opt, bool adjust){
    a.beginGroup(group);
    a.setValue(opt+ADAPT,adjust);
    a.endGroup();
}



void Options::reinitialise(QString group){
    // TODO par group !
    a.clear();
}

