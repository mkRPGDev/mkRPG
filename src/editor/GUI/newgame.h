#ifndef NEWGAME_H
#define NEWGAME_H

#include "ui_newgame.h"
#include <QtWidgets>



/*!
 * \file newgame.h
 * \brief Definition of the NewGame class
 *
 */


/*!
 * \brief The NewGame class provides a widget that
 * defines the starting parameter for a new project.
 */
class NewGame : public QDialog, private Ui::NewGame
{
    Q_OBJECT

public:
    explicit NewGame(QWidget *parent = 0);

    QString name() const;
    QString folder() const;
    bool createFolder() const;

private slots:
    void on_browse_clicked();
};

#endif // NEWGAME_H
