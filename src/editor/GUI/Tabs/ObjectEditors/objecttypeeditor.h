#ifndef OBJECTTYPEEDITOR_H
#define OBJECTTYPEEDITOR_H

#include "ui_objecttypeeditor.h"
#include "Game/ItemModels/itemmodels.h"
#include "gameobjecteditor.h"

/*!
 * \file objecttypeeditor.h
 * \brief Definition of the ObjectTypeEditor class
 *
 */


/*!
 * \brief The ObjectTypeEditor class provides a widget to edit
 * ObjectType objects.
 */
class ObjectTypeEditor : public GameObjectEditor, private Ui::ObjectTypeEditor
{
    Q_OBJECT

public:
    explicit ObjectTypeEditor(QWidget *parent = 0);
    explicit ObjectTypeEditor(ObjectType &t, QWidget *parent = 0);

    void setObjectType(ObjectType &o);

private slots:
    void on_im_currentIndexChanged(int i);

private:
    ObjectType *objectType;
    ImageListModel *imModel;
};

#endif // OBJECTTYPEEDITOR_H
