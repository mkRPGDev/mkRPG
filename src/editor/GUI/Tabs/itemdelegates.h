#ifndef ITEMDELEGATES_H
#define ITEMDELEGATES_H

#include <QtWidgets>
//#include <QStyledItemDelegate>
//#include <QItemEditorFactory>
//#include <QSpinBox>
//#include <QLineEdit>
#include "Game/game.h"


/*!
 * \file itemdelegates.h
 *
 *\brief Definition of the delegates to render different
 * items in view.
 */


class ItemEditor : public QWidget
{
    Q_OBJECT
public:
    explicit ItemEditor(QWidget *edit, bool redef, QWidget* parent);
    void setEditorProperty(const char *name, const QVariant &value);
    QVariant editorProperty(const char *name) const;
    bool resetRequested() const;
    QWidget* editor() const;

signals:
    void dataChanged(QWidget*);
    void dataReset(QWidget*);

public slots:
    void dataReseted();
    void changeData();
    void setResetable(bool r);

private slots:
    void resetData();

private:
    QWidget *edit;
    bool resetRequest;
    QToolButton *tb;
};


/*!
 * \brief The ParamItemDelegate class provides to view classes
 * the way to represent parameters and modify them with QSpinBox.
 *
 * \see FlagItemDelegate
 */
class ParamItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ParamItemDelegate(QObject *parent = nullptr);

    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    //void sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    bool reseting;
};



/*!
 * \brief The FlagItemDelegate class provides to view classes
 * the way to represent flags and modify them with QCheckBox.
 *
 * \see ParamItemDelegate
 */
class FlagItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit FlagItemDelegate(QObject *parent = nullptr);

    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    //void sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
};



/*!
 * \brief The ObjectNameItemDelegate class provides to view classes
 * the way to modify object's name.
 */
class ObjectNameItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ObjectNameItemDelegate(QObject *parent = nullptr);
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
};




#endif // ITEMDELEGATES_H
