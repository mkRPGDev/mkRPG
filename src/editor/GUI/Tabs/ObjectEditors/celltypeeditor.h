#ifndef CELLTYPEEDITOR_H
#define CELLTYPEEDITOR_H

/*!
 * \file celltypeeditor.h
 * \brief Definition of the CellTypeEditor class.
 *
 */


#include "gameobjecteditor.h"
#include "ui_celltypeeditor.h"
#include "Game/ItemModels/itemmodels.h"


/*!
 * \brief The CellTypeEditor class provides a widget to edit
 * CellType objects.
 */
class CellTypeEditor : public GameObjectEditor, private Ui::CellTypeEditor
{
    Q_OBJECT

public:
    explicit CellTypeEditor(QWidget *parent = 0);
    explicit CellTypeEditor(CellType &t, QWidget *parent = 0);

    void setCellType(CellType &t);

private slots:
    void on_bg_currentIndexChanged(int i);

private:
    CellType *ct;
    ImageListModel *imModel;
};

#endif // CELLTYPEEDITOR_H
