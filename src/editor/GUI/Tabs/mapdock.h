#ifndef MAPDOCK_H
#define MAPDOCK_H

#include "ui_mapdock.h"
#include "Docks/bdockwidget.h"

class MapDock : public BDockWidget, private Ui::MapDock
{
    Q_OBJECT

public:
    explicit MapDock(QWidget *parent = 0);
    void updateGame();

private slots:
    void on_angleX_valueChanged(int i);
    void on_angleY_valueChanged(int i);

private:
    Map* currentMap;
};

#endif // MAPDOCK_H
