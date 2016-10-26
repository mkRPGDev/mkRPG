#ifndef GAME_H
#define GAME_H

#include "object.h"
#include "map.h"




/*!
 * \brief The World class
 */
class World : public Object
{
public:
    World(Game *g = nullptr);
    ObjectsMap(w,m,M,ap,,s)
    ObjectsMap(w,c,C,ellType,,s)
private:
};


/*!
 * \brief The Game class gather the differents parts needed
 * to describe a game
 *
 * It contains mainly the World, and the ressources used by it
 * (images and strings)
 */
class Game
{
public:
    Game();
    inline World* world(){return &w;}
    inline int newIdent(){return ++idDisp;}

private:


    int idDisp;

    World w;
    QMap<int, Image> picts;
    QMap<int, QString> strings;
};

#endif // GAME_H
