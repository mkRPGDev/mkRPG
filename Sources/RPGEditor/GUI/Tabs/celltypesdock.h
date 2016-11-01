#ifndef CELLTYPESLIST_H
#define CELLTYPESLIST_H

#include "ui_celltypesdock.h"
#include "bdockwidget.h"
#include "Game/mapslistmodel.h"

class CellTypesDock : public BDockWidget, private Ui::CellTypesDock
{
    Q_OBJECT

public:
    explicit CellTypesDock(QWidget *parent = 0);

    void updateGame();
};

#endif // CELLTYPESLIST_H
