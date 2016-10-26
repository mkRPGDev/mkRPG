#ifndef MAPSEDITOR_H
#define MAPSEDITOR_H

#include "ui_mapseditor.h"

class MapsEditor : public QWidget, private Ui::MapsEditor
{
    Q_OBJECT

public:
    explicit MapsEditor(QWidget *parent = 0);
    void setGame(Game* g);
};

#endif // MAPSEDITOR_H
