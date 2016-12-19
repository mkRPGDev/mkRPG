#ifndef MAPEDITORS_H
#define MAPEDITORS_H

#include "gameobjecteditor.h"
#include "ui_mapeditor.h"




class MapEditor : public GameObjectEditor, private Ui::MapEditor
{
    Q_OBJECT

public:
    explicit MapEditor(QWidget *parent = 0);
    explicit MapEditor(Map &map, QWidget *parent = 0);
    void setMap(Map &m);

signals:
    void mapModified();
};

#endif // MAPEDITORS_H
