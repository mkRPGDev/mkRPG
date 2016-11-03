#include "bdockwidget.h"

BDockWidget::BDockWidget(QWidget *parent) :
    QWidget(parent),
    game(nullptr)
{

}

void BDockWidget::setGame(Game *g){
    game = g;
    updateGame();
}
