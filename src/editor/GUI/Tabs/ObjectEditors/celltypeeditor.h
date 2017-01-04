#ifndef CELLTYPEEDITOR_H
#define CELLTYPEEDITOR_H

#include "ui_celltypeeditor.h"

class CellTypeEditor : public QWidget, private Ui::CellTypeEditor
{
    Q_OBJECT

public:
    explicit CellTypeEditor(QWidget *parent = 0);
};

#endif // CELLTYPEEDITOR_H
