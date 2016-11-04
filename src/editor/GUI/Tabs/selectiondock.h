#ifndef SELECTIONDOCK_H
#define SELECTIONDOCK_H

#include "ui_selectiondock.h"
#include "Docks/bdockwidget.h"

class SelectionDock : public BDockWidget, private Ui::SelectionDock
{
    Q_OBJECT

public:
    explicit SelectionDock(QWidget *parent = 0);
};

#endif // SELECTIONDOCK_H
