#ifndef WORLDEDITOR_H
#define WORLDEDITOR_H

#include "ui_worldeditor.h"

class WorldEditor : public QWidget, private Ui::WorldEditor
{
    Q_OBJECT

public:
    explicit WorldEditor(QWidget *parent = 0);
};

#endif // WORLDEDITOR_H
