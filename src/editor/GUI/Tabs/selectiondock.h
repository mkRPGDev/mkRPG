#ifndef SELECTIONDOCK_H
#define SELECTIONDOCK_H

#include "ui_selectiondock.h"
#include "Docks/bdockwidget.h"
#include "mapviewer.h"
class SelectionDock : public BDockWidget, private Ui::SelectionDock
{
    Q_OBJECT

public:
    explicit SelectionDock(MapViewer *mv, QWidget *parent = 0);

private slots:
    void on_pencil_toggled(bool b);
    void on_rectangle_toggled(bool b);
    void on_region_toggled(bool b);
    void on_selectColor_colorChanged(const QColor &c);
    void on_selectOpacity_valueChanged(int v);
    void on_autoPreSelect_toggled(bool b);
    void on_preSelectColor_colorChanged(const QColor &c);
    void on_preSelectOpacity_valueChanged(int v);

private:
    void updateSelectionMode();
    MapViewer *mv;
};

#endif // SELECTIONDOCK_H
