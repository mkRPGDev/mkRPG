#ifndef CELLTYPESLIST_H
#define CELLTYPESLIST_H

#include "ui_celltypesdock.h"
#include "Docks/bdockwidget.h"
#include "Game/ItemModels/mapslistmodel.h"



/*!
 * \file celltypesdock.h
 * \brief Definition of the CellTypesDock class.
 *
 */


/*!
 * \brief The CellTypeDock class provides a BDockWidget to edit
 * \ref "CellTypes" CellType.
 */
class CellTypesDock : public BDockWidget, private Ui::CellTypesDock
{
    Q_OBJECT

public:
    explicit CellTypesDock(QWidget *parent = 0);

    void updateGame();
};

#endif // CELLTYPESLIST_H
