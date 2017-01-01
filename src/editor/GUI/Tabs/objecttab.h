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
#include "newgameobject.h"

#include <QSplitter>




/*!
 * \file objecttab.h
 * \brief Definition of the ObjectTab class
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
    void on_newEvent_clicked();
    void on_newOrder_clicked();
    void on_newObject_clicked();



private:
    EventTreeItemModel *eventsModel;
    ParamTreeItemModel *paramsModel;
    OrderTreeItemModel *ordersModel;
    FlagTreeItemModel *flagsModel;
    ObjectsTreeModel *objectsModel;
    TypeItemModel *typesModel;
    GameObject *currentObject;


    QWidget *edit;
    Game *game;
};

#endif // OBJECTEDITOR_H
