#ifndef WELCOME_H
#define WELCOME_H

#include "ui_welcome.h"
#include "tabwidget.h"






/*!
 * \file welcome.h
 * \brief Definition of the Welcome class
 *
 */


/*!
 * \brief The Welcome class is the TabWidget that
 * that is used as start screen.
 */
class Welcome : public TabWidget, private Ui::Welcome
{
    Q_OBJECT

public:
    explicit Welcome(QWidget *parent = 0);
};

#endif // WELCOME_H
