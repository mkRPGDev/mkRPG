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
#include "itemdelegates.h"


/*!
 * \brief The ActionTab class is the TabWidget that
 * provides Action edition features
 */
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
    void eventChanged(const QModelIndex &ev);
    void orderChanged(const QModelIndex &ord);

    void removeRow(const QPersistentModelIndex &r);
    void editRow(const QPersistentModelIndex &r);

    void on_newAction_clicked();
    void on_selectEmitter_clicked();
    void on_addReceiver_clicked();

private:
    FilteredObjectsTreeModel *tem;
    FilteredObjectsTreeModel *trc;
    ObjectsTreeModel *emittersModel;
    ObjectsTreeModel *receiversModel;
    EventTreeItemModel *eventsModel;
    OrderTreeItemModel *ordersModel;
    ReceiverListModel *rcvModel;
    ActionsListModel *actionsModel;
    GameObject *emitter;
    GameObject *receiver;
    QString event;
    QString order;
    Game *game;
    Action *act;
};

#endif // ACTIONTAB_H
