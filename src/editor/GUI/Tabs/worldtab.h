#ifndef WORLDTAB_H
#define WORLDTAB_H

#include "ui_worldtab.h"
#include "tabwidget.h"
#include "Game/ItemModels/mapslistmodel.h"


/*!
 * \file worldtab.h
 * \brief Definition of the WorldEditor class.
 *
 */


/*!
 * \brief The WorldTab class is the TabWidget that
 * provides world edition features.
 */
class WorldTab : public TabWidget, private Ui::WorldTab
{
    Q_OBJECT

public:
    explicit WorldTab(QWidget *parent = 0);
    ~WorldTab();

    void setGame(Game* g);

signals:
    void editMap();

private slots:
    void on_mapsView_doubleClicked(const QModelIndex &index);

private:
    Game* game;
};

#endif // WORLDTAB_H
