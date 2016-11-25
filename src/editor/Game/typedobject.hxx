#ifndef TYPEDOBJECT_HXX
#define TYPEDOBJECT_HXX

#include "gameobject.h"

/*!
 * \file typedobject.hxx
 *
 * \brief This file define some templates base classes.
 */




typedef QList<QPair<QString,QList<QString>>> HierarchicalAttr;


/*!
 * \brief The Type class
 */
template<class T>
class Type : public GameObject
{
public:
    Type(T* ancestor, Game *g, GameObject *aParent) : GameObject(g, aParent), aAncestor(ancestor){}
    Type(Game*g, GameObject *aParent) : GameObject(g, aParent), aAncestor(nullptr){} // temporaire

    virtual bool isInheritedParam(const QString &p) const {
        return aAncestor ? aAncestor->hasParam(p) : false;
    }
    virtual bool isRedefiniedParam(const QString &p) const {
        return isRedefiniedParam(p) && GameObject::hasParam(p);
    }
    virtual int getParam(const QString &p) const {
        return GameObject::hasParam(p) || aAncestor==nullptr ?
                    GameObject::getParam(p) :
                    aAncestor->getParam(p);
    }
    virtual void setParam(const QString &p, int v) {aParams[p] = v; touch();}
    virtual bool hasParam(const QString &p) const {return  GameObject::hasParam(p) || (aAncestor && aAncestor->hasParam(p));}
    virtual QList<QString> params() const {return filter(aParams.keys());}
    HierarchicalAttr organisedParams() const{
        HierarchicalAttr p(aAncestor ? aAncestor->organisedParams() : HierarchicalAttr());
        p.append(QPair<QString,QList<QString>>(typeName(), GameObject::params()));
        removeLastRedondances(p);
        return p;
    }

    virtual bool getFlag(const QString &f) const{return aFlags.value(f,false);}
    virtual void setFlag(const QString &f, bool v) {aFlags[f] = v; touch();}
    virtual bool hasFlag(const QString &f) const {return aFlags.contains(f) || (aAncestor && aAncestor->hasFlag(f));}
    virtual QList<QString> flags() const {

        return (filter(GameObject::flags()));}

protected:
    T *const aAncestor;

private:
    static void removeLastRedondances(HierarchicalAttr &attr){
        if(attr.length()>1){
            auto &a = attr.last().second;
            for(auto i : attr)
                if(i != attr.last())
                    a.erase(std::remove_if(a.begin(), a.end(), [&i](QString a){return i.second.contains(a);}),
                            a.end());
        }
    }
};


/*!
 * \brief The TypedObject class
 *
 * This class inherited from Type, and provide the same behaviour,
 * except that the ancestor cannot be a null pointer, and it can be
 * modified after the construction.
 *
 * \note
 * The \c class \c T template argument has to be inherited from Type.
 */
template<class T>
class TypedObject : public Type<T>
{
public:
    TypedObject(Game*g, GameObject *aParent) : Type<T>(g, aParent)/*, aType(T::defaultType())*/{} // temporaire

    TypedObject(T &type, Game *g, GameObject *aParent) : Type<T>(&type, g, aParent) {}



    const T& objectType(){return *Type<T>::aAncestor;}
    void setObjectType(const T &type){Type<T>::aAncestor = &type;}

protected:


};

#endif // TYPEDOBJECT_HXX
