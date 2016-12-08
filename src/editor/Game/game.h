#ifndef GAME_H
#define GAME_H

#include "object.h"
#include "map.h"


/*!
 * \file game.h
 * \brief Definition of the Game and World classes.
 */

#define DefaultType(type,Type) private: Type *default##Type = new Type(*this); public: Type &type() const{return *default##Type;}
/**<
 * Todoc
 */


class World; /*!
 * \brief The DefaultTypes class represents the object that
 * will contains all types.
 *
 * This DefaultTypes object has to be given for the construction
 * of the empty type of each type.
 */
class DefaultTypes : public GameObject
{
public:
    TypeName(Types)

    DefaultTypes(World &parent);
    C0(DefaultType,c,C,ellType)
    C0(DefaultType,m,M,apType)
    C0(DefaultType,o,O,bjectType)
};

/*!
 * \brief The World class
 */
class World : public GameObject
{
public:
    TypeName(World)
    World(Game *g, GameObject *aParent);
    ObjectListD(m,M,ap,,s, Map) /**< Set of maps*/
    ObjectListD(o,O,bject,,s, Object)
    ObjectListD(c,C,ellType,,s, CellType)

    const DefaultTypes &types() const;
private:
    DefaultTypes *aTypes;
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
    TypeName(Game)
    Game();
    ~Game();
    int newIdent(GameObject *obj);
    void aboutToDestroy(GameObject *obj);
    GameObject *object(int id);
    /**<
     * Returns a new unused identifiers
     *
     * \note
     * It should only be used by GameObject methods \ref GameObject::init and GameObject::GameObject.
     */
    inline World &world(){return *w;}
    inline Map* currentMap(){return map;}
    void setCurrentMap(Map *m);

    void addImage(Image *im);

private:
    int idDisp;
    QMap<int, GameObject*> objects;

    World *w;
    Map *map;
    QMap<int, Image*> picts;
    QMap<int, QString> strings;

};




#endif // GAME_H
