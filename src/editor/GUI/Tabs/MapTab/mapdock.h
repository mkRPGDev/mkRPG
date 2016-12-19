#ifndef MAPDOCK_H
#define MAPDOCK_H

#include "ui_mapdock.h"
#include "../Docks/bdockwidget.h"
#include "../ObjectEditors/mapresizedialog.h"


/*!
 * \file mapdock.h
 * \brief Definition of the MapDock class.
 *
 */


/*!
 * \brief The MapDock class provides a BDockWidget to edit
 * \ref "Maps" Maps.
 */
class MapDock : public BDockWidget, private Ui::MapDock
{
    Q_OBJECT

public:
    explicit MapDock(QWidget *parent = 0);
    void updateGame();

private slots:
    void on_angleX_valueChanged(int i);
    void on_angleY_valueChanged(int i);
    void on_resizing_pressed();

private:
    Map* currentMap;
};

#endif // MAPDOCK_H
