#include "actiontab.h"

ActionTab::ActionTab(QWidget *parent) :
    TabWidget(parent), game(nullptr), act(nullptr)
{
    setupUi(this);

    emittersModel = new ObjectsTreeModel();
    emittersModel->setEditable(false);
    emitters->setModel(emittersModel);
    receiversModel = new ObjectsTreeModel();
    receiversModel->setEditable(false);
    receiversV->setModel(receiversModel);
    eventsModel = new EventTreeItemModel(this);
    events->setModel(eventsModel);
    ordersModel = new OrderTreeItemModel(this);
    orders->setModel(ordersModel);
    orders->setSelectionBehavior(QAbstractItemView::SelectRows);
    splitter_3->setEnabled(false);
    actionsModel = new ActionsListModel(this);
    actionsV->setModel(actionsModel);

    connect(actionsV->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this, SLOT(actionChanged(QModelIndex)));
    connect(emitters->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this, SLOT(emitterChanged(QModelIndex)));
    connect(receiversV->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this, SLOT(receiverChanged(QModelIndex)));
}

void ActionTab::setGame(Game *g){
    emittersModel->setGameObject(&g->world());
    game = g;
    emitters->expandAll();
    receiversModel->setGameObject(&g->world());
    receiversV->expandAll();
    actionsModel->setGame(g);
}


void ActionTab::actionChanged(const QModelIndex &ac){
    act = ac.isValid() ? game->action(ac.data().toString()) : nullptr;
    splitter_3->setEnabled(act != nullptr);
    qDebug() << act;
}

void ActionTab::emitterChanged(const QModelIndex &ind){
    emitter = static_cast<GameObject*>(ind.internalPointer());
    events->setEnabled(emitter != nullptr);
    eventsModel->setObject(emitter);
    if(dynamic_cast<InheritableObject*>(emitter) != nullptr)
        for(int i(0); i<eventsModel->rowCount(QModelIndex()); ++i){
            events->setFirstColumnSpanned(i, QModelIndex(), true);
            events->setExpanded(eventsModel->index(i,0,QModelIndex()), true);
        }
}

void ActionTab::receiverChanged(const QModelIndex &ind){
    receiver = static_cast<GameObject*>(ind.internalPointer());
    orders->setEnabled(receiver != nullptr);
    ordersModel->setObject(receiver);
    if(dynamic_cast<InheritableObject*>(receiver) != nullptr)
        for(int i(0); i<ordersModel->rowCount(QModelIndex()); ++i){
            orders->setFirstColumnSpanned(i, QModelIndex(), true);
            orders->setExpanded(ordersModel->index(i,0,QModelIndex()), true);
        }
}

void ActionTab::on_newAction_clicked(){
    actionsModel->addAction("new_action");
}
