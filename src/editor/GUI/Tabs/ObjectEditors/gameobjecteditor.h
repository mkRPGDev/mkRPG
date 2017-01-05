#ifndef GAMEOBJECTEDITOR_H
#define GAMEOBJECTEDITOR_H

/*!
 * \file gameobjecteditor.h
 * \brief Definition of the GameObjectEditor class.
 *
 */

#include <QWidget>
#include "Game/game.h"

/*!
 * \brief The GameObjectEditor class defines the base class for
 * GameObject specific editors.
 *
 * Using the Factory design-pattern, the \ref editor static method
 * provide a universal constructor for registed editors.
 */
class GameObjectEditor : public QWidget
{
    Q_OBJECT
public:
    explicit GameObjectEditor(QWidget *parent = 0);


    static GameObjectEditor* editor(GameObject &obj, QWidget *parent = 0);
signals:

public slots:


};



#endif // GAMEOBJECTEDITOR_H
