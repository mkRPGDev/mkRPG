#ifndef CELLDOCK_H
#define CELLDOCK_H

#include "ui_celldock.h"
#include "Docks/bdockwidget.h"

class CellDock : public BDockWidget, private Ui::CellDock
{
    Q_OBJECT

public:
    explicit CellDock(QWidget *parent = 0);
};

#endif // CELLDOCK_H