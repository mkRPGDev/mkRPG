#ifndef MAPEDITOR_H
#define MAPEDITOR_H

#include "ui_mapseditor.h"
#include "tabwidget.h"
#include "celltypesdock.h"
#include "selectiondock.h"
#include "celldock.h"
#include "mapdock.h"


/*!
 * \file mapeditor.h
 * \brief Definition of the MapsEditor class
 *
 */


/*!
 * \brief The ObjectEditor class is the TabWidget that
 * provides map edition features
 */
class MapsEditor : public TabWidget, private Ui::MapsEditor
{
    Q_OBJECT

public:
    explicit MapsEditor(QWidget *parent = 0);
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
