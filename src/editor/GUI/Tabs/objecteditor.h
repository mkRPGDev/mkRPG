#ifndef OBJECTEDITOR_H
#define OBJECTEDITOR_H

#include "ui_objecteditor.h"
#include "tabwidget.h"
#include "Game/itemmodels.h"
#include "Game/mapslistmodel.h"
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
    ObjectParamTableModel *paramsModel;
    ObjectFlagTableModel *flagsModel;
    ObjectsTreeModel *objectsModel;

};

#endif // OBJECTEDITOR_H
