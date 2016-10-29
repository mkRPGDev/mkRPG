#ifndef OBJECT_H
#define OBJECT_H

#include <QtCore>
#include <QtGui>
#include <assert.h>

class Game;
#define Editing lastEdit = QDateTime::currentDateTime()
#define Getter(name) inline int name() const{return params[#name];}
#define Param(name,Name) Getter(name) \
    inline void set##Name(int name##Value){params[#name] = name##Value;}
#define ParamDef(name,value) params[#name] = value; Editing
#define ParamObj(name,Name,pref) \
    Name* name() const{return pref##Name;} \
    void set##Name(Name* name##Obj){pref##Name = name##Obj; \
        ParamDef(name,name##Obj ? name##Obj->ident() : 0);} \
private: \
    Name* pref##Name; \
public:
#define ObjectsMapC(name,names,Type,Types,pref,arg) \
    void add##Type(Type* arg){pref##Types[arg->ident()] = arg; Editing;} \
    void remove##Type(Type* arg){if(pref##Types.contains(arg->ident()))pref##Types.remove(arg->ident()); Editing;} \
    inline Type* name(int id) const{return pref##Types.contains(id) ? pref##Types[id] : nullptr;} \
    inline QList<Type*> names() const{return pref##Types.values();} \
private: \
    QMap<int, Type*> pref##Types; \
public:
#define ObjectsMap(pref,ini,Ini,body,sg,pl) \
    ObjectsMapC(ini##body##sg, ini##body##pl, Ini##body##sg, Ini##body##pl, pref,ini)


class Object
{
public:
    Object(Game *g = nullptr);
    void init(Game *g);
    int ident() const{return id;}
    virtual bool isValid() const{return id;}
    inline const QDateTime& lastModification() const{return lastEdit;}
    inline int getParam(const QString &p){return params[p];}

protected:
    Game *game;
    int id;
    QMap<QString, int> params;
    QString fileName;
    QDateTime lastEdit;
};




class Image : public Object
{
public:
    Image(Game*g, const QString &fileName);
    inline bool isValid() const{Object::isValid() && !im.isNull();}
    inline const QImage& image() const{return im;}
    inline const QSize& size() const{return im.size();}
private:
    QImage im;
};




#endif // OBJECT_H
