#ifndef OBJECT_H
#define OBJECT_H

#include <QtCore>
#include <QtGui>
#include <assert.h>

/*! \file object.h
 * \brief This header defines the base class Object and Image.
 *
 * Blablabla.
 */


class Game;
/*! Facility to notify modifications.
 */
#define Editing lastEdit = QDateTime::currentDateTime()
/*! The macro Getter create a getter for the parameter \i name.
 */
#define Getter(name) inline int name() const{return params[#name];}
/*! The macro ParamDef is a convenient way to modify a parameter
 */
#define ParamDef(name,value) params[#name] = value; Editing
/*! The macro Param defines the getter and setter functions
 * for an object's parameter.
 */
#define Param(name,Name) Getter(name) \
    inline void set##Name(int name##Value){ParamDef(name, name##Value);}
/*! The macro ParamObj defines the getter and setter functions
 * for a member object of an Object.
 */
#define ParamObj(name,Name,pref) \
    Name* name() const{return pref##Name;} \
    void set##Name(Name* name##Obj){pref##Name = name##Obj; \
        ParamDef(name,name##Obj ? name##Obj->ident() : 0);} \
private: \
    Name* pref##Name; \
public:
/*! Internal used only
  */
#define ObjectsMapC(name,names,Type,Types,pref,arg) \
    void add##Type(Type* arg){pref##Types[arg->ident()] = arg; Editing;} \
    void remove##Type(Type* arg){if(pref##Types.contains(arg->ident()))pref##Types.remove(arg->ident()); Editing;} \
    inline Type* name(int id) const{return pref##Types.contains(id) ? pref##Types[id] : nullptr;} \
    inline QList<Type*> names() const{return pref##Types.values();} \
private: \
    QMap<int, Type*> pref##Types; \
public:
/*! The macro ObjectsMap define the methods necessary to
 * manipulate a set of objects
 */
#define ObjectsMap(pref,ini,Ini,body,sg,pl) \
    ObjectsMapC(ini##body##sg, ini##body##pl, Ini##body##sg, Ini##body##pl, pref,ini)

/*!
 * \brief The Object class is the base class for every part
 * of games.
 *
 * Each instance is identified by a game-wide unique
 * identifier.
 *
 * On each modification, the lastEdit attribute has to be updated, in order that
 * other \ref Object "objects" can see that modifications occured.
 */
class Object
{
public:
    Object(Game *g = nullptr);
    void init(Game *g); /**< initialise the object in case it had been construct with a NULL pointer (array of objects)*/
    int ident() const{return id;}
    virtual bool isValid() const{return id;} /**< return true if the object has been initialised*/
    inline const QDateTime& lastModification() const{return lastEdit;} /**< return the last time of modification*/
    inline int getParam(const QString &p){return params[p];}

protected:
    Game *game;
    int id;
    QMap<QString, int> params;
    QString fileName;
    QDateTime lastEdit;
};



/*!
 * \brief The Image class stores an external file in a QImage, and
 * gives each image ressources a unique identifier.
 */
class Image : public Object
{
public:
    Image(Game*g, const QString &fileName);
    inline bool isValid() const{return Object::isValid() && !im.isNull();}
    inline const QImage& image() const{return im;}
    inline const QSize size() const{return im.size();}
private:
    QImage im;
};




#endif // OBJECT_H
