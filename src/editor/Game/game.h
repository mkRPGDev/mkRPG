#ifndef GAME_H
#define GAME_H

#include "object.h"
#include "map.h"

/**
 * \file game.h
 * \brief Definition of the Game and World classes.
 */



/*!
 * \brief The World class
 */
class World : public GameObject
{
public:
    World(Game *g, GameObject *parent);
    ObjectListD(m,M,ap,,s, Map) /**< Set of maps*/
    //ObjectsMap(w,c,C,ellType,,s) /**< Set of cell types*/
    ObjectListD(o,O,bject,,s, Object)
    ObjectListD(c,C,ellType,,s, CellType)

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
class Game : public GameObject
{
public:
    Game();
    inline int newIdent(){return ++idDisp;}
    /**<
     * Returns a new unused identifiers
     *
     * \note
     * It should only be used by GameObject methods \ref GameObject::init and GameObject::GameObject.
     */
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
