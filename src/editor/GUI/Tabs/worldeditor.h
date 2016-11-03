#ifndef WORLDEDITOR_H
#define WORLDEDITOR_H

#include "ui_worldeditor.h"
#include "Game/mapslistmodel.h"

class WorldEditor : public QWidget, private Ui::WorldEditor
{
    Q_OBJECT

public:
    explicit WorldEditor(QWidget *parent = 0);

    void setGame(Game* g);

private:
    Game* game;
};

#endif // WORLDEDITOR_H
