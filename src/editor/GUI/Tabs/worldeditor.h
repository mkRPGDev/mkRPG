#ifndef WORLDEDITOR_H
#define WORLDEDITOR_H

#include "ui_worldeditor.h"
#include "tabwidget.h"
#include "Game/mapslistmodel.h"

class WorldEditor : public TabWidget, private Ui::WorldEditor
{
    Q_OBJECT

public:
    explicit WorldEditor(QWidget *parent = 0);
    ~WorldEditor();

    void setGame(Game* g);

signals:
    void editMap();

private slots:
    void on_mapsView_doubleClicked(const QModelIndex &index);

private:
    Game* game;
};

#endif // WORLDEDITOR_H
