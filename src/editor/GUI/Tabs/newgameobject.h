#ifndef NEWGAMEOBJECT_H
#define NEWGAMEOBJECT_H

/*!
 * \file newgameobject.h
 * \brief Definition of the NewGameObject class.
 *
 */


#include "ui_newgameobject.h"
#include "Game/game.h"
#include "Game/ItemModels/itemmodels.h"
#include <QPushButton>

class NewGameObject : public QDialog, private Ui::NewGameObject
{
    Q_OBJECT

public:
    explicit NewGameObject(Game &g, QWidget *parent = 0);
    int selectedType() const;
    bool createObject() const;


private slots:
    void on_gameObject_toggled(bool t);

    void typeChanged(const QModelIndex &ty);
    void preciseTypeChanged(const QModelIndex &ty);

private:

    ObjectsTreeModel *types;
    ObjectsTreeModel *preciseTypes;
    FilteredObjectsTreeModel *preciseType;
    FilteredObjectsTreeModel *allowedType;
    int selType;
};

#endif // NEWGAMEOBJECT_H
