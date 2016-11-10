#ifndef MAPSEDITOR_H
#define MAPSEDITOR_H

#include "ui_mapseditor.h"
#include "celltypesdock.h"
#include "selectiondock.h"
#include "celldock.h"
#include "mapdock.h"


/*!
 * \brief The MapsEditor class is the tab offering map editing
 * facilities
 *
 *
 */
class MapsEditor : public QWidget, private Ui::MapsEditor
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

#endif // MAPSEDITOR_H
