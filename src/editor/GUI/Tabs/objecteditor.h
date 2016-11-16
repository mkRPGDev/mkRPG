#ifndef OBJECTEDITOR_H
#define OBJECTEDITOR_H

#include "ui_objecteditor.h"
#include "tabwidget.h"
#include "Game/game.h"
#include "Game/mapslistmodel.h"

#include <QStyledItemDelegate>
#include <QItemEditorFactory>
#include <QSpinBox>
#include <QSplitter>

class ParamItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ParamItemDelegate(QObject *parent = nullptr);

    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

};

class ObjectEditor : public TabWidget, private Ui::ObjectEditor
{
    Q_OBJECT

public:
    explicit ObjectEditor(QWidget *parent = 0);
    void setGame(Game *g);

private slots:
    void on_objects_clicked(const QModelIndex &ind);

private:
    ObjectParamTableModel *paramsModel;
    ObjectFlagTableModel *flagsModel;
    ObjectsTreeModel *objectsModel;

};

#endif // OBJECTEDITOR_H
