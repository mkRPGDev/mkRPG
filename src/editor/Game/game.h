#ifndef GAME_H
#define GAME_H

#include "object.h"
#include "map.h"

#include <QAbstractItemModel>

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
    World(Game *g, GameObject *aParent);
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


class ObjectsTreeItem
{
public:
    explicit ObjectsTreeItem(GameObject *obj, ObjectsTreeItem *parent = nullptr);


private:

    GameObject *obj;
    ObjectsTreeItem* parent;
};

class ObjectsTreeModel : public QAbstractItemModel
{
public:
    explicit ObjectsTreeModel(QObject *parent = nullptr);
    explicit ObjectsTreeModel(Game *g, QObject *parent = nullptr);
    void setGame(Game *g);
    int columnCount(const QModelIndex &parent) const;
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;

private:
    Game *game;
};


#endif // GAME_H
