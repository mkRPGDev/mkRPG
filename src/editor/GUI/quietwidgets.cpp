#include "quietwidgets.h"


QuietComboBox::QuietComboBox(QWidget *parent) :
    QComboBox(parent),
    waiting(false)
{
    connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(signalFilter(int)));
}

void QuietComboBox::setCurrentIndex(int index){
    waiting = true;
    QComboBox::setCurrentIndex(index);
}

void QuietComboBox::setModel(QAbstractItemModel *model){
    waiting = true;
    QComboBox::setModel(model);
}

void QuietComboBox::signalFilter(int index){
    if(waiting) waiting = false;
    else emit userChangedCurrentIndex(index);
}


