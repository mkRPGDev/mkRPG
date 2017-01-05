#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QWidget>

#include <QDebug>

/*!
 * \file tabwidget.h
 * \brief Definition of the TabWidget class.
 *
 */



/*!
 * \brief The TabWidget class is the base class for
 * editing tabs.
 *
 * It provides common methods to update the currently edited game.
 */
class TabWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TabWidget(QWidget *parent = 0);
    int index() const;
    void setIndex(int i);
    virtual void updateGame(){}

signals:

public slots:

private:

    int ind;

};

#endif // TABWIDGET_H
