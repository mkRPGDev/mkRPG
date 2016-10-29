#ifndef MAPSEDITOR_H
#define MAPSEDITOR_H

#include "ui_mapseditor.h"
#include "Game/mapslistmodel.h"

class MapsEditor : public QWidget, private Ui::MapsEditor
{
    Q_OBJECT

public:
    explicit MapsEditor(QWidget *parent = 0);
    void setGame(Game* g);

private slots:
    void on_angleX_valueChanged(int i);
    void on_angleY_valueChanged(int i);

private:
    Game *game;
    Map *currentMap;
};

#endif // MAPSEDITOR_H
