#include "options.h"

Options *Options::instance = NULL;

Options &Options::options(){
    if(!instance) instance = new Options;
    return *instance;
}


#define DefaultF(gr, id, val) defaultValues[gr][id] = QPair<QVariant, bool>(val, false)
#define Default(gr, id, val) defaultValues[gr][id] = QPair<QVariant, bool>(val, true)
Options::Options(){
    Default(WIN, "Position", QPoint(0,0)); // TODO pos...
    Default(WIN, "Size", QSize(1000,742));
    Default(WIN, "Maximized", false);

    Default(DIR, "New", QDir::homePath());
    Default(DIR, "Open", QDir::homePath());
    Default(DIR, "Save",QDir::homePath());
}

bool Options::isAdaptaive(QString group, QString opt, bool adapt){
    a.beginGroup(group);
    if(!a.contains(opt+ADAPT))
        a.setValue(opt+ADAPT, adapt);
    bool val = a.value(opt+ADAPT).toBool();
    a.endGroup();
    return val;
}

void Options::setAdaptaive(QString group, QString opt, bool adapt){
    a.beginGroup(group);
    a.setValue(opt+ADAPT,adapt);
    a.endGroup();
}



void Options::reinitialise(){
    a.clear();
}

