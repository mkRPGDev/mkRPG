#ifndef BDOCKWIDGET_H
#define BDOCKWIDGET_H

#include <QWidget>
#include "Game/game.h"

/*!
 * \brief The BDockWidget class is the base class for
 * game-related docks.
 *
 * It provides common functions for set game, update, ...
 */
class BDockWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BDockWidget(QWidget *parent = 0);

    void setGame(Game *g);
    virtual void updateGame(){}

signals:
    void gameModified();
    void changeDockName(QString);

public slots:

protected:
    Game *game;
};

#endif // BDOCKWIDGET_H
