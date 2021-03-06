#ifndef TABBAR_H
#define TABBAR_H

#include "tabacces.h"
#include "Tabs/tabwidget.h"
#include <QPixmap>

/*!
 * \file tabbar.h
 * \brief Definition of the TabBar class
 */

/*!
 * \brief The TabBar class provides a widget to choose the
 * current \ref "tab" Tab.
 *
 * \see TabAcces
 */
class TabBar : public QWidget
{
    Q_OBJECT
public:
    explicit TabBar(QWidget *parent = 0);
    void addTabAcces(const QString &n, const QPixmap &p, TabWidget *w);
    inline int currentTab() const{return tab;}
    void setTabsEnabled(bool e);

signals:
    void currentTabChanged(int);

public slots:
    void setCurrentTab(int t);

private slots:
    void tabAccesActivated(int t);

private:

    int tab;
    int nbTab;
    QList<TabAcces*> tabs;
    QVBoxLayout* lay;
};

#endif // TABBAR_H
