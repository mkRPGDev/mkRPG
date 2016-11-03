#include "tabbar.h"

TabBar::TabBar(QWidget *parent) :
    QWidget(parent), tab(-1), nbTab(0)
{
    lay = new QVBoxLayout;
    lay->setContentsMargins(0,0,0,0);
    lay->addItem(new QSpacerItem(0,2000,QSizePolicy::Minimum, QSizePolicy::Expanding));
    setLayout(lay);
}

void TabBar::addTabAcces(const QString &n, const QPixmap &p){
    TabAcces* acc = new TabAcces(nbTab++, n, p);
    tabs.append(acc);
    lay->insertWidget(nbTab-1, acc);
    connect(acc, SIGNAL(activated(int)), this, SLOT(tabAccesActivated(int)));
    if(tab==-1) setCurrentTab(0);
}

void TabBar::setCurrentTab(int t){
    if(t>=0 && t<nbTab && t!=tab) tabs[t]->setActive(true);
}

void TabBar::tabAccesActivated(int t){
    if(t==tab) return;
    if(tab+1) tabs[tab]->setActive(false);
    tab = t;
    emit currentTabChanged(tab);
}

void TabBar::setTabsEnabled(bool e){
    for(auto i : tabs) i->setEnabled(e);
    tabs.first()->setEnabled(true);
}
