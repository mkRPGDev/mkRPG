#include "actiontab.h"


ActionTab::ActionTab(QWidget *parent) :
    TabWidget(parent), game(nullptr), act(nullptr)
{
    setupUi(this);

    emittersModel = new ObjectsTreeModel();
    tem = new FilteredObjectsTreeModel(this);
    tem->setSourceModel(emittersModel);
    tem->setMode(true);
    tem->setDisplayedItem(0);
    emittersModel->setEditable(false);
    emitters->setModel(tem);
    receiversModel = new ObjectsTreeModel();
    trc = new FilteredObjectsTreeModel(this);
    trc->setSourceModel(receiversModel);
    trc->setMode(true);
    trc->setDisplayedItem(0);
    receiversModel->setEditable(false);
    receiversV->setModel(trc);
    eventsModel = new EventTreeItemModel(this, true);
    events->setModel(eventsModel);
    ordersModel = new OrderTreeItemModel(this, true);
    orders->setModel(ordersModel);
    splitter_3->setEnabled(false);
    rcvModel = new ReceiverListModel(this);
    rcv->setModel(rcvModel);
    actionsModel = new ActionsListModel(this);
    actionsV->setModel(actionsModel);

    ActionReceiverItemDelegate *arid = new ActionReceiverItemDelegate(this);
    rcv->setItemDelegate(arid);
    connect(arid, SIGNAL(removeRow(QPersistentModelIndex)), this, SLOT(removeRow(QPersistentModelIndex)));
    connect(arid, SIGNAL(editRow(QPersistentModelIndex)), this, SLOT(editRow(QPersistentModelIndex)));

    QPalette p(newEmitter->palette());
    p.setColor(QPalette::Window, p.color(QPalette::AlternateBase));
    newEmitter->setPalette(p);
    p = newReceiver->palette();
    p.setColor(QPalette::Window, p.color(QPalette::AlternateBase));
    newReceiver->setPalette(p);

    connect(actionsV->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this, SLOT(actionChanged(QModelIndex)));
    connect(emitters->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this, SLOT(emitterChanged(QModelIndex)));
    connect(receiversV->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this, SLOT(receiverChanged(QModelIndex)));
    connect(events->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this, SLOT(eventChanged(QModelIndex)));
    connect(orders->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this, SLOT(orderChanged(QModelIndex)));
}

void ActionTab::setGame(Game *g){
    emittersModel->setGameObject(g);
    game = g;
    emitters->expandAll();
    receiversModel->setGameObject(g);
    receiversV->expandAll();
    actionsModel->setGame(g);
}


void ActionTab::actionChanged(const QModelIndex &ac){
    act = ac.isValid() ? game->action(ac.data().toString()) : nullptr;
    splitter_3->setEnabled(act != nullptr);
    rcvModel->setAction(act);
    if(act != nullptr){
        lineEdit->setText(act->emitter() ? act->emitter()->name() + "::" + act->event() : "");
    }
}

void ActionTab::emitterChanged(const QModelIndex &ind){
    emitter = static_cast<GameObject*>(tem->mapToSource(ind).internalPointer());
    eventChanged(QModelIndex());
    events->setEnabled(emitter != nullptr);
    eventsModel->setObject(emitter);
    if(dynamic_cast<InheritableObject*>(emitter) != nullptr)
        for(int i(0); i<eventsModel->rowCount(QModelIndex()); ++i){
            events->setFirstColumnSpanned(i, QModelIndex(), true);
            events->setExpanded(eventsModel->index(i,0,QModelIndex()), true);
        }
}

void ActionTab::receiverChanged(const QModelIndex &ind){
    receiver = static_cast<GameObject*>(trc->mapToSource(ind).internalPointer());
    orderChanged(QModelIndex());
    orders->setEnabled(receiver != nullptr);
    ordersModel->setObject(receiver);
    if(dynamic_cast<InheritableObject*>(receiver) != nullptr)
        for(int i(0); i<ordersModel->rowCount(QModelIndex()); ++i){
            orders->setFirstColumnSpanned(i, QModelIndex(), true);
            orders->setExpanded(ordersModel->index(i,0,QModelIndex()), true);
        }
}

void ActionTab::eventChanged(const QModelIndex &ev){
    if(ev.isValid())
        event = eventsModel->index(ev.row(),0,ev.parent()).data(Qt::DisplayRole).toString();
    else
        event = "";
    newEmitter->setText(emitter->name() + "::" + event);
    selectEmitter->setEnabled(ev.isValid());
}

void ActionTab::orderChanged(const QModelIndex &ord){
    if(ord.isValid())
        order = ordersModel->index(ord.row(),0,ord.parent()).data(Qt::DisplayRole).toString();
    else
        order = "";
    newReceiver->setText(receiver->name() + "::" + order);
    addReceiver->setEnabled(ord.isValid());
}



void ActionTab::on_newAction_clicked(){
    actionsModel->addAction("new_action");
}

void ActionTab::on_selectEmitter_clicked(){
    act->setEmitter(emitter);
    act->setEvent(event);
    lineEdit->setText(emitter->name() + "::" + event);
}

void ActionTab::on_addReceiver_clicked(){
    rcvModel->addReceiver(receiver, order);
}


void ActionTab::removeRow(const QPersistentModelIndex &r){
    rcvModel->removeRow(r.row());
}

void ActionTab::editRow(const QPersistentModelIndex &r){
    QModelIndex sel(receiversModel->find(rcvModel->receiver(r.row()).first->ident()));
    if(sel.isValid()){
        receiversV->selectionModel()->clearSelection();
        receiversV->selectionModel()->setCurrentIndex(trc->mapFromSource(sel),QItemSelectionModel::Select | QItemSelectionModel::Current);
        receiversV->selectionModel()->setCurrentIndex(trc->mapFromSource(sel.parent().child(sel.row(),1)),QItemSelectionModel::Select);
    }

    QModelIndex sem(ordersModel->findOrder(rcvModel->receiver(r.row()).second));
    if(sem.isValid()){
        orders->selectionModel()->clearSelection();
        orders->selectionModel()->setCurrentIndex(sem,QItemSelectionModel::Select | QItemSelectionModel::Current);
        orders->selectionModel()->setCurrentIndex(sem.parent().child(sel.row(),1),QItemSelectionModel::Select);
    }
    removeRow(r);
}
