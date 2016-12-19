#ifndef TABACCES_H
#define TABACCES_H

#include "ui_tabacces.h"

/*!
 * \file tabacces.h
 * \brief Definition of the TabAcces class
 */

/*!
 * \brief The TabAcces class provides a clickable widget
 * to show the current tab.
 *
 * \see TabBar
 */
class TabAcces : public QWidget, private Ui::TabAcces
{
    Q_OBJECT

public:
    explicit TabAcces(int i, const QString& n, const QPixmap& p, QWidget *parent = 0);
    void setActive(bool a);

signals:
    void activated(int i);

private:
    void mousePressEvent(QMouseEvent *);
    // TODO : préactivation si la souris passe dessus.

    int id;
    QColor c;
};

#endif // TABACCES_H
