#ifndef MAPEDITOR_H
#define MAPEDITOR_H

#include "ui_maptab.h"
#include "../tabwidget.h"
#include "celltypesdock.h"
#include "selectiondock.h"
#include "celldock.h"
#include "mapdock.h"


/*!
 * \file maptab.h
 * \brief Definition of the MapsEditor class
 *
 */


/*!
 * \brief The MapTab class is the TabWidget that
 * provides map edition features
 */
class MapsTab : public TabWidget, private Ui::MapsTab
{
    Q_OBJECT

public:
    explicit MapsTab(QWidget *parent = 0);
    void setGame(Game* g);

public slots:
    void updateGame();

private slots:
    void mapSizeChanged(QSize s);
    void viewCenterChanged(QPoint p);
    void viewSizeChanged(QSize s);
    void on_mapHScrollBar_valueChanged(int);
    void on_mapVScrollBar_valueChanged(int);


private:
    void checkScrollBarUtility();
    void updateViewCenterPosition();
    Game *game;
    Map *currentMap;
    QList<BDockWidget*> docksW;
};

#endif // MAPEDITOR_H
