#ifndef CELLDOCK_H
#define CELLDOCK_H

#include "ui_celldock.h"
#include "Docks/bdockwidget.h"
#include "Game/ItemModels/mapslistmodel.h"


class CellDock : public BDockWidget, private Ui::CellDock
{
    Q_OBJECT

public:
    explicit CellDock(QWidget *parent = nullptr);

public slots:
    void updateGame();
    void selectionChanged();

private slots:
    void on_cellTypes_userChangedCurrentIndex(int i);

private:
    Map *map;
};

#endif // CELLDOCK_H
