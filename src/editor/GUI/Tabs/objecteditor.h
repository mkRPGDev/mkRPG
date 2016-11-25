#ifndef OBJECTEDITOR_H
#define OBJECTEDITOR_H

#include "ui_objecteditor.h"
#include "tabwidget.h"
#include "Game/ItemModels/itemmodels.h"
#include "Game/ItemModels/mapslistmodel.h"
#include "Game/ItemModels/paramtreeitemmodel.h"
#include "itemdelegates.h"


#include <QSplitter>




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
    ObjectFlagTableModel *flagsModel;
    ObjectsTreeModel *objectsModel;

};

#endif // OBJECTEDITOR_H
