#include "tabwidget.h"

TabWidget::TabWidget(QWidget *parent) :
    QWidget(parent), ind(-1)
{

}


void TabWidget::setIndex(int i){
    ind = i;
}

int TabWidget::index() const{
    return ind;
}

