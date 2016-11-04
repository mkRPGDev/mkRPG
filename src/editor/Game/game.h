#ifndef GAME_H
#define GAME_H

#include "object.h"
#include "map.h"




/*!
 * \brief The World class is an Object
 */
class World : public Object
{
public:
    World(Game *g, Object *parent);
    ObjectsMap(w,m,M,ap,,s) /**< Set of maps*/
    ObjectsMap(w,c,C,ellType,,s) /**< Set of cell types*/
private:
};


/*!
 * \brief The Game class gather the differents parts needed
 * to describe a game
 *
 * It contains mainly the World, and the ressources used by it
 * (images and strings)
 *
 * For editing purposes, it contains also the active map (the
 * one being editing)
 */
class Game : public Object
{
public:
    Game();
    inline int newIdent(){return ++idDisp;}
    inline World* world(){return &w;}
    inline Map* currentMap(){return map;}
    void setCurrentMap(Map *m);

    void addImage(Image *im);

private:


    int idDisp;

    World w;
    Map *map;
    QMap<int, Image*> picts;
    QMap<int, QString> strings;
};

#endif // GAME_H
