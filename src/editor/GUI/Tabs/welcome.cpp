#include "welcome.h"

Welcome::Welcome(QWidget *parent) :
    TabWidget(parent)
{
    setupUi(this);
    connect(newGame, SIGNAL(clicked(bool)), this, SIGNAL(newGameS()));
    connect(openGame, SIGNAL(clicked(bool)), this, SIGNAL(openGameS()));
}
