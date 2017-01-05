#ifndef ENTITYTYPEEDITOR_H
#define ENTITYTYPEEDITOR_H

#include "ui_entitytypeeditor.h"
#include "Game/ItemModels/itemmodels.h"
#include "gameobjecteditor.h"

/*!
 * \file entitytypeeditor.h
 * \brief Definition of the EntityTypeEditor class
 *
 */


/*!
 * \brief The EntityTypeEditor class provides a widget to edit
 * EntityType objects.
 */
class EntityTypeEditor : public GameObjectEditor, private Ui::EntityTypeEditor
{
    Q_OBJECT

public:
    explicit EntityTypeEditor(QWidget *parent = 0);
    explicit EntityTypeEditor(EntityType &t, QWidget *parent = 0);

    void setEntityType(EntityType &o);

private slots:
    void on_im_currentIndexChanged(int i);

private:
    EntityType *entityType;
    ImageListModel *imModel;
};

#endif // ENTITYTYPEEDITOR_H
