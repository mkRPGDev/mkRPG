#ifndef MAPEDITOR_H
#define MAPEDITOR_H

/*!
 * \file mapeditor.h
 * \brief Definition of the MapEditor class.
 *
 */

#include "gameobjecteditor.h"
#include "ui_mapeditor.h"
#include "mapresizedialog.h"
#include "Game/ItemModels/mapslistmodel.h"


/*!
 * \brief The MapEditor class provides a widget to edit
 * \ref "maps" Map.
 */
class MapEditor : public GameObjectEditor, private Ui::MapEditor
{
    Q_OBJECT

public:
    explicit MapEditor(QWidget *parent = 0);
    explicit MapEditor(Map &map, QWidget *parent = 0);
    void setMap(Map *m);

private slots:
    void on_resizing_clicked();
    void on_angleX_valueChanged(int i);
    void on_angleY_valueChanged(int i);
    void on_ang_valueChanged(int i);
    void on_etir_valueChanged(int i);

signals:
    void mapModified();

private:
    void updateAngles();
    void updateParam();
    Map *map;
    MapTypeListModel *mtModel;
};



#endif // MAPEDITOR_H
