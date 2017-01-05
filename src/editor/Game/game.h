#ifndef GAME_H
#define GAME_H

#include "object.h"
#include "map.h"
#include "entity.h"


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
    C0(DefaultType,e,E,ntityType)
};



/*!
 * \brief The GameObjectList class enables to gather
 * objects by types.
 *
 * It is only provided for edition purpose and does not
 * appear in XML files.
 */
template<class Type>
class GameObjectList : public GameObject
{
public:
    TypeName()
    GameObjectList(const QString &name, GameObject &parent);
    bool isEditable() const{return false;}

    void addGameObject(Type &go);
    void removeGameObject(int id);
    QList<Type*> gameObjects();
    Type &gameObject(int id);
private:
    QMap<int, Type*> aGameObjects;
};


template<class Type>
GameObjectList<Type>::GameObjectList(const QString &name, GameObject &parent) :
    GameObject(parent), aGameObjects(QMap<int,Type*>())
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




#define ListDef(Type, Types) private: GameObjectList<Type> *a##Types; public:                                                       /**<
 * Cool Macro ListDef
 */
#define ListAdd(type, Type, Types) void add##Type(Type *type) const{a##Types->addGameObject(*type);}                                /**<
 * Cool Macro ListAdd
 */
#define ListRemove(Type, Types) void remove##Type(int id) const{a##Types->removeGameObject(id);}                                    /**<
 * Cool Macro ListRemove
 */
#define ListSetter(type, Type, Types) ListAdd(type, Type, Types) ListRemove(Type,Types)                                             /**<
 * Cool Macro ListSetter
 */
#define ListGetAll(types, Type, Types) QList<Type*> types() const{return a##Types->gameObjects();}                                  /**<
 * Cool Macro ListGetAll
 */
#define ListGetOne(type, Type, Types) Type &type(int id) const{return a##Types->gameObject(id);}                                    /**<
 * Cool Macro ListGetOne
 */
#define ListGetter(type, types, Type, Types) ListGetAll(types, Type, Types) ListGetOne(type, Type, Types)                           /**<
 * Cool Macro ListGetter
 */
#define List(type, types, Type, Types) ListDef(Type, Types) ListSetter(type, Type, Types) ListGetter(type, types, Type, Types)      /**<
 * Cool Macro List
 */
#define ListD(init,Init,body,sg,pl) List(init##body##sg,init##body##pl,Init##body##sg,Init##body##pl)                               /**<
 * Cool Macro ListD
 */
#define ListInit(Type, Types) a##Types = new GameObjectList<Type>(QObject::tr(#Types),*this);                                       /**<
 * Cool Macro ListInit
 */
#define ListInitD(Body, sg,pl) ListInit(Body##sg, Body##pl)                                                                         /**<
 * Cool Macro ListInitD
 */


/*!
 * \brief The GameObjectInventory class gathers the different
 * \ref "GameObjectLists" GameObjectList.
 *
 * It is only provided for edition purpose and does not
 * appear in XML files.
 */
class GameObjectInventory : public GameObject
{
public:
    TypeName()
    GameObjectInventory(World &parent);
    bool isEditable() const{return false;}
    //ObjectList(type,Type,)

    ListD(m,M,ap,,s)
    ListD(o,O,bject,,s)
    ListD(e,E,ntit,y,ies)
};





/*!
 * \brief The World class
 */
class World : public GameObject
{
public:
    TypeName(World)
    World(Game *g, GameObject *aParent);

    /*const*/ DefaultTypes &types() /*const*/;
    /*const*/ GameObjectInventory &objects() /*const*/;
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
    GameObject *object(int id);                 /**<
     * Returns a new unused identifiers
     *
     * \note
     * It should only be used by GameObject methods \ref GameObject::init and GameObject::GameObject.
     */
    inline World &world(){return *w;}
    inline Map* currentMap(){return map;}
    void setCurrentMap(Map *m);

    //void addImage(Image *im);

    QList<QString> actions() const;
    Action *action(const QString &a);
    QString addAction(QString a, Action *act);
    void removeAction(const QString &a);
    QString renameAction(const QString &a, const QString &nv);

    ListD(i,I,mage,,s)

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
