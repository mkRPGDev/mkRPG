#ifndef ACTIONTAB_H
#define ACTIONTAB_H

/*!
 * \file actiontab.h
 * \brief Definition of the ActionTab class.
 */

#include "ui_actiontab.h"
#include "tabwidget.h"

#include "Game/ItemModels/itemmodels.h"
#include "Game/ItemModels/attrtreeitemmodel.h"

class ActionTab : public TabWidget, private Ui::ActionTab
{
    Q_OBJECT

public:
    explicit ActionTab(QWidget *parent = 0);
    void setGame(Game *g);

private slots:
    void actionChanged(const QModelIndex &ac);
    void emitterChanged(const QModelIndex &em);
    void receiverChanged(const QModelIndex &re);

    void on_newAction_clicked();

private:
    ObjectsTreeModel *emittersModel;
    ObjectsTreeModel *receiversModel;
    EventTreeItemModel *eventsModel;
    OrderTreeItemModel *ordersModel;
    ActionsListModel *actionsModel;
    GameObject *emitter;
    GameObject *receiver;
    Game *game;
    Action *act;
};

#endif // ACTIONTAB_H
