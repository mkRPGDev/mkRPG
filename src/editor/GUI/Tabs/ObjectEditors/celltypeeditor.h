#ifndef CELLTYPEEDITOR_H
#define CELLTYPEEDITOR_H

#include "gameobjecteditor.h"
#include "ui_celltypeeditor.h"

class CellTypeEditor : public GameObjectEditor, private Ui::CellTypeEditor
{
    Q_OBJECT

public:
    explicit CellTypeEditor(QWidget *parent = 0);
    explicit CellTypeEditor(CellType &t, QWidget *parent = 0);

    void setCellType(CellType &t);
};

#endif // CELLTYPEEDITOR_H
