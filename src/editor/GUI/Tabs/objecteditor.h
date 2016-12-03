#ifndef OBJECTEDITOR_H
#define OBJECTEDITOR_H

#include "ui_objecteditor.h"
#include "tabwidget.h"
#include "Game/ItemModels/itemmodels.h"
#include "Game/ItemModels/typeitemmodel.h"
#include "Game/ItemModels/mapslistmodel.h"
#include "Game/ItemModels/attrtreeitemmodel.h"
#include "itemdelegates.h"


#include <QSplitter>




/*!
 * \file objecteditor.h
 * \brief Definition of the ObjectEditor class
 *
 */


/*!
 * \brief The ObjectEditor class is the TabWidget that
 * provides object edition features
 */
class ObjectEditor : public TabWidget, private Ui::ObjectEditor
{
    Q_OBJECT

public:
    explicit ObjectEditor(QWidget *parent = 0);
    void setGame(Game *g);

private slots:
    void currentElementChanged(const QModelIndex &ind);

private:
    ParamTreeItemModel *paramsModel;
    FlagTreeItemModel *flagsModel;
    ObjectsTreeModel *objectsModel;
    TypeItemModel *typesModel;

};

#endif // OBJECTEDITOR_H
