#ifndef OBJECTEDITOR_H
#define OBJECTEDITOR_H

#include "ui_objecttab.h"
#include "tabwidget.h"
#include "Game/ItemModels/itemmodels.h"
#include "Game/ItemModels/typeitemmodel.h"
#include "Game/ItemModels/mapslistmodel.h"
#include "Game/ItemModels/attrtreeitemmodel.h"
#include "itemdelegates.h"
#include "ObjectEditors/gameobjecteditor.h"


#include <QSplitter>




/*!
 * \file objecttab.h
 * \brief Definition of the ObjectEditor class
 *
 */


/*!
 * \brief The ObjectTab class is the TabWidget that
 * provides object edition features
 */
class ObjectTab : public TabWidget, private Ui::ObjectTab
{
    Q_OBJECT

public:
    explicit ObjectTab(QWidget *parent = 0);
    void setGame(Game *g);

private slots:
    void currentElementChanged(const QModelIndex &ind);
    void on_newParam_clicked();
    void on_newFlag_clicked();



private:

    ParamTreeItemModel *paramsModel;
    FlagTreeItemModel *flagsModel;
    ObjectsTreeModel *objectsModel;
    TypeItemModel *typesModel;
    GameObject *currentObject;

    QWidget *edit;

};

#endif // OBJECTEDITOR_H
