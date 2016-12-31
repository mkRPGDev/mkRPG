#include "itemdelegates.h"


ItemEditor::ItemEditor(QWidget *edit, bool redef, QWidget *parent) :
    QWidget(parent), edit(edit), resetRequest(false)
{
    setLayout(new QHBoxLayout);
    layout()->setContentsMargins(0,0,0,0);
    layout()->setSpacing(0);
    layout()->addWidget(edit);
    tb = new QToolButton();
    tb->setIcon(QIcon::fromTheme("edit-delete"));
    tb->setFixedWidth(15);
    tb->setVisible(redef);
    connect(tb, SIGNAL(clicked(bool)), this, SLOT(resetData()));
    layout()->addWidget(tb);
}

QWidget* ItemEditor::editor() const{
    return edit;
}

void ItemEditor::setEditorProperty(const char *name, const QVariant &value){
    edit->setProperty(name, value);
}

QVariant ItemEditor::editorProperty(const char *name) const{
    return edit->property(name);
}

bool ItemEditor::resetRequested() const{
    return resetRequest;
}

void ItemEditor::resetData(){
    resetRequest = true;
    emit dataChanged(this);
    emit dataReset(this);
}

void ItemEditor::changeData(){
    emit dataChanged(this);
}

void ItemEditor::dataReseted(){
    resetRequest = false;
}

void ItemEditor::setResetable(bool r){
    tb->setVisible(r);
}





ParamItemDelegate::ParamItemDelegate(QObject *parent) :
    QStyledItemDelegate(parent),
    reseting(false)
{
    setItemEditorFactory(new QItemEditorFactory);
}


QWidget* ParamItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &UNUSED(option), const QModelIndex &index) const{
    QSpinBox *sp = new QSpinBox();
    sp->setAutoFillBackground(true);
    sp->setButtonSymbols(QSpinBox::NoButtons);
    QPoint dom = index.data(Qt::SizeHintRole).toPoint();
    sp->setMinimum(dom.x());
    sp->setMaximum(dom.y());
    sp->setFrame(false);
    ItemEditor * ie = new ItemEditor(sp, index.data(Qt::UserRole).toBool(), parent);
    connect(sp, SIGNAL(valueChanged(int)), ie, SLOT(changeData()));
    connect(ie, SIGNAL(dataChanged(QWidget*)), this, SIGNAL(commitData(QWidget*)));
    connect(ie, SIGNAL(dataReset(QWidget*)), this, SIGNAL(closeEditor(QWidget*)));
    sp->selectAll();
    return ie;
}



void ParamItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const{
    ItemEditor *ed(static_cast<ItemEditor*>(editor));
    ed->setEditorProperty("value", index.data(Qt::DisplayRole));
    ed->setResetable(index.data(Qt::UserRole).toBool());
}

void ParamItemDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &UNUSED(index)) const{
    editor->setGeometry(option.rect);
}

void ParamItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const{
    ItemEditor *edit = static_cast<ItemEditor*>(editor);
    if(edit->resetRequested())
        model->setData(index, QVariant(), Qt::UserRole);
    else
        model->setData(index, edit->editorProperty("value"));
    edit->dataReseted();
    edit->setResetable(index.data(Qt::UserRole).toBool());
}

void ParamItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const{
    QStyledItemDelegate::paint(painter, option, index);
    if(0){
        painter->save();
        painter->setBrush(Qt::gray);
        painter->drawRect(option.rect);
        painter->restore();
    }

//    painter->drawText(option.rect, index.data().toString());
    /*QLabel l;
    l.setText(index.data());
    l.setGeometry(option.rect);
    painter->draw*/

    return; // EN COURS
    QStyleOptionButton button;
    button.palette = option.palette;
    button.rect = option.rect;
    button.rect.setLeft(option.rect.left()+2);
    button.text = index.data().toString();
    QApplication::style()->drawControl(QStyle::CE_CheckBoxLabel, &button, painter, 0);
    button.palette = option.palette;
    button.rect = option.rect;
    button.rect.setLeft(button.rect.right()-20);
    button.icon = QIcon(":/Icons/edit.png");
    button.iconSize = QSize(16,16);
    QApplication::style()->drawControl(QStyle::CE_PushButton, &button, painter, 0);

}










FlagItemDelegate::FlagItemDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
    setItemEditorFactory(new QItemEditorFactory);
}


QWidget* FlagItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &UNUSED(option), const QModelIndex &index) const{
    QCheckBox *cb = new QCheckBox(parent);
    cb->setAutoFillBackground(true);
    ItemEditor * ie = new ItemEditor(cb, index.data(Qt::UserRole).toBool(), parent);
    connect(cb, SIGNAL(toggled(bool)), ie, SLOT(changeData()));
    connect(ie, SIGNAL(dataChanged(QWidget*)), this, SIGNAL(commitData(QWidget*)));
    connect(ie, SIGNAL(dataReset(QWidget*)), this, SIGNAL(closeEditor(QWidget*)));
    return ie;
}

void FlagItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const{
    static_cast<ItemEditor*>(editor)->setEditorProperty("checked", index.data(Qt::DisplayRole));
    static_cast<ItemEditor*>(editor)->setResetable(index.data(Qt::UserRole).toBool());
}

void FlagItemDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &UNUSED(index)) const{
    editor->setGeometry(option.rect);
}

void FlagItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const{
    ItemEditor *edit = static_cast<ItemEditor*>(editor);
    if(edit->resetRequested())
        model->setData(index, QVariant(), Qt::UserRole);
    else
        model->setData(index, edit->editorProperty("checked"));
    edit->dataReseted();
    edit->setResetable(index.data(Qt::UserRole).toBool());
}

void FlagItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const{
    //QStyledItemDelegate::paint(painter, option, index);
//    painter->drawText(option.rect, index.data().toString());
    /*QLabel l;
    l.setText(index.data());
    l.setGeometry(option.rect);
    painter->draw*/
    if(!(index.flags() & Qt::ItemIsEditable)) return;
    QStyleOptionButton check;
    check.state = check.state & ~QStyle::State_HasFocus;
    check.state |= index.data().toBool() ? QStyle::State_On : QStyle::State_Off;
    check.rect = option.rect;
    check.rect.setWidth(check.rect.height()-1);
    check.rect.setHeight(check.rect.width());
    QApplication::style()->drawPrimitive(QStyle::PE_IndicatorCheckBox, &check, painter);
    return; // EN COURS
    QStyleOptionButton button;
    button.palette = option.palette;
    button.rect = option.rect;
    button.rect.setLeft(option.rect.left()+2);
    button.text = index.data().toString();
    QApplication::style()->drawControl(QStyle::CE_CheckBoxLabel, &button, painter);
    button.palette = option.palette;
    button.rect = option.rect;
    button.rect.setLeft(button.rect.right()-20);
    button.icon = QIcon(":/Icons/edit.png");
    button.iconSize = QSize(16,16);
    QApplication::style()->drawControl(QStyle::CE_PushButton, &button, painter);

}







ObjectNameItemDelegate::ObjectNameItemDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
    setItemEditorFactory(new QItemEditorFactory);
}


QWidget* ObjectNameItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &UNUSED(option), const QModelIndex &UNUSED(index)) const{
    QLabel *le = new QLabel(parent);
    return le;
}

void ObjectNameItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const{
    editor->setProperty("text", index.data(Qt::EditRole));
}

void ObjectNameItemDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &UNUSED(index)) const{
    editor->setGeometry(option.rect);
}

//void ObjectNameItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const{
//    model->setData(index, editor->property("text"));
//}

void ObjectNameItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const{
    QStyledItemDelegate::paint(painter, option, index);
    QStyleOptionButton button;
    button.palette = option.palette;
    button.rect = QRect(option.rect.right()-16, option.rect.top(), 17, 17);
    button.icon = QIcon(":/Icons/supprimer.png");
    button.iconSize = QSize(12,12);
    button.state = QStyle::State_Enabled;
    QApplication::style()->drawControl(QStyle::CE_PushButton, &button, painter);

}




ActionReceiverEditor::ActionReceiverEditor(QWidget *parent) :
    QWidget(parent)
{
    QHBoxLayout *lay = new QHBoxLayout();
    setLayout(lay);
    lay->setContentsMargins(0,0,0,0);
    lay->setSpacing(1);
    lay->addItem(new QSpacerItem(2000,0));
    edit = new QToolButton();
    edit->setIcon(QIcon(":/Icons/editer.png"));
    edit->setFixedSize(23,23);
    lay->addWidget(edit);
    remove = new QToolButton();
    remove->setIcon(QIcon(":/Icons/supprimer.png"));
    remove->setFixedSize(23,23);
    lay->addWidget(remove);
}




ActionReceiverItemDelegate::ActionReceiverItemDelegate(QObject *parent):
    QStyledItemDelegate(parent)
{}

QWidget* ActionReceiverItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const{
    return new ActionReceiverEditor(parent);
}

void ActionReceiverItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const{

}

void ActionReceiverItemDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const{
    editor->setGeometry(option.rect);
}

void ActionReceiverItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const{

}

void ActionReceiverItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const{
    QStyledItemDelegate::paint(painter, option, index);

    QStyleOptionButton button;
    button.palette = option.palette;
    button.rect = QRect(option.rect.right()-22, option.rect.top(), 23, 23);
    button.icon = QIcon(":/Icons/supprimer.png");
    button.iconSize = QSize(16,16);
    button.state = QStyle::State_Enabled;
    QApplication::style()->drawControl(QStyle::CE_PushButton, &button, painter);
    button.rect = QRect(option.rect.right()-46, option.rect.top(), 23, 23);
    button.icon = QIcon(":/Icons/editer.png");
    button.state = QStyle::State_Enabled;
    QApplication::style()->drawControl(QStyle::CE_PushButton, &button, painter);
}


