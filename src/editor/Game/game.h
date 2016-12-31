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
    TypeName()
    DefaultTypes(World &parent);
    bool isEditable() const{return false;}
    C0(DefaultType,c,C,ellType)
    C0(DefaultType,m,M,apType)
    C0(DefaultType,o,O,bjectType)
};




class GameObjectInventory;
template<class Type>
class GameObjectList : public GameObject
{
public:
    TypeName()
    GameObjectList(const QString &name, GameObjectInventory &parent);
    bool isEditable() const{return false;}

    void addGameObject(Type &go);
    void removeGameObject(int id);
    QList<Type*> gameObjects();
    Type &gameObject(int id);
private:
    QMap<int, Type*> aGameObjects;
};


template<class Type>
GameObjectList<Type>::GameObjectList(const QString &name, GameObjectInventory &parent) :
    GameObject(parent)
{
    setName(name);
}

template<class Type>
void GameObjectList<Type>::addGameObject(Type &go){
    aGameObjects[go.ident()] = &go;
    go.setParent(this);
}

template<class Type>
void GameObjectList<Type>::removeGameObject(int id){
    assert(aGameObjects.contains(id));
    aGameObjects[id]->setParent(parent()->parent());
    aGameObjects.remove(id);
}

template<class Type>
Type &GameObjectList<Type>::gameObject(int id){
    assert(aGameObjects.contains(id));
    return *aGameObjects.value(id);
}

template<class Type>
QList<Type*> GameObjectList<Type>::gameObjects(){
    return aGameObjects.values();
}




#define ListDef(Type, Types) private: GameObjectList<Type> *a##Types; public:
#define ListAdd(type, Type, Types) void add##Type(Type &type) const{a##Types->addGameObject(type);}
#define ListRemove(Type, Types) void remove##Type(int id) const{a##Types->removeGameObject(id);}
#define ListSetter(type, Type, Types) ListAdd(type, Type, Types) ListRemove(Type,Types)
#define ListGetAll(types, Type, Types) QList<Type*> types() const{return a##Types->gameObjects();}
#define ListGetOne(type, Type, Types) Type &type(int id) const{return a##Types->gameObject(id);}
#define ListGetter(type, types, Type, Types) ListGetAll(types, Type, Types) ListGetOne(type, Type, Types)
#define List(type, types, Type, Types) ListDef(Type, Types) ListSetter(type, Type, Types) ListGetter(type, types, Type, Types)
#define ListD(init,Init,body,sg,pl) List(init##body##sg,init##body##pl,Init##body##sg,Init##body##pl)
#define ListInit(Type, Types) a##Types = new GameObjectList<Type>(QObject::tr(#Types),*this);
#define ListInitD(Body, sg,pl) ListInit(Body##sg, Body##pl)

class GameObjectInventory : public GameObject
{
public:
    TypeName()
    GameObjectInventory(World &parent);
    bool isEditable() const{return false;}
    //ObjectList(type,Type,)

    ListD(m,M,ap,,s)
    ListD(o,O,bject,,s)
};





/*!
 * \brief The World class
 */
class World : public GameObject
{
public:
    TypeName(World)
    World(Game *g, GameObject *aParent);
    //ObjectListD(m,M,ap,,s, Map) /**< Set of maps*/
    //ObjectListD(o,O,bject,,s, Object)
    //ObjectListD(c,C,ellType,,s, CellType)

    const DefaultTypes &types() const;
    const GameObjectInventory &objects() const;
private:
    DefaultTypes *aTypes;
    GameObjectInventory *aObjects;
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

    QList<QString> actions() const;
    Action *action(const QString &a);
    QString addAction(QString a, Action *act);
    void removeAction(const QString &a);
    QString renameAction(const QString &a, const QString &nv);

private:
    int idDisp;
    QMap<int, GameObject*> objects;

    World *w;
    Map *map;
    QMap<int, Image*> picts;
    QMap<int, QString> strings;
    QMap<QString, Action*> aActions;
};




#endif // GAME_H
