#ifndef WELCOME_H
#define WELCOME_H

#include "ui_welcome.h"
#include "tabwidget.h"

class Welcome : public TabWidget, private Ui::Welcome
{
    Q_OBJECT

public:
    explicit Welcome(QWidget *parent = 0);
};

#endif // WELCOME_H
