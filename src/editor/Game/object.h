#ifndef OBJECT_H
#define OBJECT_H

#include <QtCore>
#include <QtGui>
#include <assert.h>
#include <algorithm>

/*!
 * \file object.h
 *
 * \brief Definition of the base class GameObject, and some inherited classes.
 *
 * ## The objects structure
 *
 * ### Inheritance and parent
 *
 * Each class which is part of game representation has to inherit from GameObject.
 * It gives the class standard params and flags mechanisms that are used for edition
 * purposes.
 *
 * Each instance must have a parent (except the Game one) which will keep an eye on
 * his children. If the parent cannot be given at the construction time (case or array)s,
 * the \ref init function must then be called as soon as possible.
 *
 *
 * ### Reference count
 *
 * ### %Objects destructors
 *
 * A default implementation of GameObject destructor is provided. It destroy every
 * children the instance has. This avoid fastidious destructor implementation.
 *
 *
 * ## The Macro System
 *
 * To add conveniently attributes and flags to GameObject subclassed objects,
 * a set of macro is provided.
 *
 * ### Name conventions
 *
 * For a attribute named \c attr, the following conventions are observed :
 * - \c attr() is the getter method
 * - \c setAttr() is the setter method
 * - \c aAttr is the name of the attribut (if any)
 *
 * A specific convention is applied for flags (boolean attributes) :
 * - isAttr() is the getter method
 *
 * ### Macros
 *
 * To define a new attribute, a global macro can be used in the class declaration.
 * The provided basic implementations keep the object edition synchronization.
 *
 * If a cleverer process is needed, custom getter or setter can be implemented,
 * and the getter and setter macros can be used separately to define the obvious methods
 *
 * **Provided macros**
 *
 * Attribute Type               | Complete declaration  | Getter      | Setter
 * -----------------------------|-----------------------|-------------|------------
 * Flags (bool)                 | #Flag                 |#FlagGetter  |#FlagSetter
 * Parameters (int)             | #Param                |#ParamGetter |#ParamSetter
 * GameObject based Attributes  | #Attr                 |#AttrGetter  |#AttrSetter
 *
 *
 * **The case of attributes**
 *
 * An additionnal #AttrT macro is provided, that deduce a default name from the type.
 *
 *
 * ### Name tools
 *
 * To make the definition easier and avoid the name repetition that is introduced by
 * the name convention, a #C macro is provided to construct the names with lower and upper
 * initial letter from theses letter and the end of the name.
 *
 *
 * \author Baptiste Pauget
 */


#define TypeName(Type) virtual QString typeName() const{return QString(#Type);}

#define ObjectListDef(Objects,Type) private: QMap<int, Type*> a##Objects; public:
#define ObjectListAdd(Object,Objects, Type) void add##Object(Type* new##Object){a##Objects[new##Object->ident()] = new##Object; touch();}
#define ObjectListTake(Object, Objects, Type) Type* take##Object(int id){touch(); return a##Objects.take(id);}
#define ObjectListGetter(object,Objects, Type) inline Type* object(int id) const{return a##Objects.value(id, nullptr);}
#define ObjectListValues(objects,Objects, Type) inline QList<Type*> objects() const{return a##Objects.values();}
#define ObjectListGetters(object,Object,objects,Objects,Type) ObjectListGetter(object,Objects, Type) ObjectListValues(objects,Objects, Type)
#define ObjectListModifiers(Object, Objects, Type) ObjectListAdd(Object,Objects, Type) ObjectListTake(Object, Objects, Type)
#define ObjectList(object,Object,objects,Objects,Type) \
    ObjectListDef(Objects,Type) ObjectListGetters(object,Object,objects,Objects,Type) ObjectListModifiers(Object, Objects, Type)
#define ObjectListD(init,Init,body,sg,pl,Type) ObjectList(init##body##sg,Init##body##sg,init##body##pl,Init##body##pl,Type)

#define C(Macro, init,Init,body, ...) Macro(init##body, Init##body, ##__VA_ARGS__) /*!<
 * The C macro calls the Macro argument with argument tokens formed by the concatenation
 * of init and body, and Init and body.
 *
 * This enables to call a macro with the same argument, with the initial letter in
 * lower and upper case.
 *
 * A custom number of arguments can be added after the \c body one.
 *
 * \note
 * This use of variadic arguments follow the <a href="https://gcc.gnu.org/onlinedocs/cpp/Variadic-Macros.html">
 * gcc specification</a>, but can be not supported by some compilers.
 *
 * \note
 * As some IDE does not fully support variadic macro expansion, the #C0 and #C1 macros can be used
 * to avoid some inconvenience due to uncomplete code undersanding.
 *
 * \b Example
 * \code{.cpp}
 * C(Param,w,W,idth)
 *     --> Param(width, Width)
 *
 * C(Attr, p,P,arent, GameObject)
 *     --> Attr(parent, Parent, GameObject)
 * \endcode
 *
 * \see \ref object.h
 */
#define C0(Macro, init,Init,body) Macro(init##body, Init##body) /*!<
 * The C0 macro is equivalent to the #C macro, with no additional argument.
 *
 * This macro is provided to avoid the use of variadic arguments that are currently
 * not totally supported by some IDE.
 * \b Example
 * \code{.cpp}
 * C(Flag, v,V,isible)
 *     --> Flag(visible, Visible)
 * \endcode
 *
 * \see C1
 */
#define C1(Macro, init,Init,body, arg) Macro(init##body, Init##body, arg) /*!<
 * The C1 macro is equivalent to the #C macro, with one additional argument.
 *
 * This macro is provided to avoid the use of variadic arguments that are currently
 * not totally supported by some IDE.
 * \b Example
 * \code{.cpp}
 * C(Attr, p,P,arent, GameObject)
 *     --> Attr(parent, Parent, GameObject)
 * \endcode
 *
 * \see C0
 */
#define ProtectFlag(flag) reserved.insert(QString(#flag));/*!<
 * The ProtectFlag macro registers the flag named \c flag as protected
 * \i ie it cannot be modified directly in a flag editor (it will not appear).
 *
 * \see flag, ProtectParam
 */
#define SetFlag(flag, value) setFlag(#flag,value) /*!<
 * Conveniant macro to set a flag directly.
 *
 * This is usefull in custom setters, to avoid call loops.
 *
 * \warning
 * The \ref GameObject::touch "touch" function isn't called. After this macro use,
 * the \ref GameObject "object" is no longer synchronised.
 *
 * \b Example
 * \code{.cpp}
 * SetFlag(visible, false)
 *     --> aFlags["visible"] = false
 * \endcode
 * \see Flag, FlagSetter, \ref object.h
 */
#define FlagGetter(flag, Flag) inline bool is##Flag() const{return getFlag(#flag);} /*!<
 * The FlagGetter macro defines a generic getter method for the flag named \c flag.
 *
 * With respect to the \ref object.h "name convention", this macro needs the flag's name with lower and upper
 * initial letter case.
 *
 * \warning
 * The default getter method does not check wether the \c flag named flag really exist.
 * To avoid runtime access error, it is strongly advice to initialize the flag in the
 * object's contructor, using the setter method or the #SetFlag macro.
 *
 * \b Example
 * \code{.cpp}
 * FlagGetter(visible,Visible)
 *     --> inline bool isVisible() const{return aFlags["visible"];}
 * \endcode
 * \see Flag, FlagSetter, C
 */
#define FlagSetter(flag, Flag) inline void set##Flag(bool flag){SetFlag(flag,flag); touch();} /*!<
 * The FlagSetter macro defines a generic setter method for the flag named \c flag.
 *
 * With respect to the \ref object.h "name convention", this macro needs the flag's name with lower and upper
 * initial letter case.
 *
 * \b Example
 * \code{.cpp}
 * FlagSetter(visible,Visible)
 *     --> inline void setVisible(bool visible){aFlags["visible"] = visible; touch()}
 * \endcode
 * \see Flag, FlagGetter, C
 */
#define Flag(flag, Flag) FlagGetter(flag, Flag) FlagSetter(flag, Flag) /*!<
 * The Flag macro defines generic getter and setter methods for the flag named \c flag.
 *
 * With respect to the \ref object.h "name convention", this macro needs the flag's name with lower and upper
 * initial letter case.
 *
 * \b Example
 * \code{.cpp}
 * Flag(visible,Visible)
 *     --> inline bool isVisible() const{return aFlags["visible"];}
 *         inline void setVisible(bool visible){aFlags["visible"] = visible; touch()}
 * \endcode
 * \see FlagGetter, FlagSetter, C
 */
#define ProtectParam(param) reserved.insert(QString(#param));/*!<
 * The ProtectParam macro registers the flag named \c param as protected
 * \i ie it cannot be modified directly in a param editor (it will not appear).
 *
 * \see param, ProtectFlag
 */
#define SetParam(param, value) setParam(#param,value) /*!<
 * Conveniant macro to set a param directly.
 *
 * This is usefull in custom setters, to avoid call loops.
 *
 * \warning
 * The \ref GameObject::touch "touch" function isn't called. After this macro use,
 * the \ref GameObject "object" is no longer synchronised.
 *
 * \b Example
 * \code{.cpp}
 * SetParam(width, 42)
 *     --> aParams["width"] = 42
 * \endcode
 *
 * \see Param, ParamSetter, \ref object.h
 */
#define ParamGetter(param) inline int param() const{return getParam(#param);} /*!<
 * The ParamGetter macro defines a generic getter method for the parameter named \c param.
 *
 *
 * \warning
 * The default getter method does not check wether the \c param named parameter really exist.
 * To avoid runtime access error, it is strongly advice to initialize the parameter in the
 * object's contructor, using the setter method or the #SetParam macro.
 *
 * \b Example
 * \code{.cpp}
 * ParamGetter(width)
 *     --> inline int width() const{return aParams["width"];}
 * \endcode
 * \see Param, ParamSetter
 */
#define ParamSetter(param, Param) inline void set##Param(int param##Value){SetParam(param,param##Value); touch();} /*!<
 * The ParamSetter macro defines a generic setter method for the parameter named \c param.
 *
 * With respect to the \ref object.h "name convention", this macro needs the parameter's name with lower and upper
 * initial letter case.
 *
 * \b Example
 * \code{.cpp}
 * ParamSetter(width,Width)
 *     --> inline void setWidth(bool widthValue){aParams["width"] = widthValue; touch();}
 * \endcode
 * \see Param, ParamGetter, C
 */
#define Param(param, Param) ParamGetter(param) ParamSetter(param, Param) /*!<
 * The Param macro defines generic getter and setter methods for the parameter named \c param.
 *
 * With respect to the \ref object.h "name convention", this macro needs the parameter's name with lower and upper
 * initial letter case.
 *
 * \b Example
 * \code{.cpp}
 * Param(width,Width)
 *     --> inline int width() const{return aParams["width"];}
 *         inline void setWidth(int widthValue){aParams["width"] = widthValue; touch();}
 * \endcode
 * \see ParamGetter, ParamSetter, C
 */
#define AttrGetter(attr, Attr, Type) inline Type* attr() const{return a##Attr;} /*!<
 * The AttrGetter macro defines a generic getter method for the attribute named \c attr of type \c Type.
 *
 * With respect to the \ref object.h "name convention", this macro needs the attribute's name with lower and upper
 * initial letter case.
 *
 * \b Example
 * \code{.cpp}
 * AttrGetter(parent,Parent,GameObject)
 *     --> inline GameObject* parent() const{return aParent;}
 * \endcode
 * \see Attr, AttrSetter, C
 */
#define AttrFree(Attr) if(a##Attr) a##Attr->removeReference();/*!<
 * The AttrFree macro decrease the number of references of the GameObject attribut attr.
 * It should be used before the deletion/modifcation of the pointer.
 *
 * \see AttrLink
 */
#define AttrLink(Attr) if(a##Attr) a##Attr->addReference();/*!<
 * The AttrLink macro increase the number of references of the GameObject attribut attr.
 * It should be used before the saving of a pointer as an attribute.
 *
 * \see AttrFree
 */
#define AttrSetter(attr, Attr, Type) inline void set##Attr(Type* new##Attr){AttrFree(Attr); a##Attr = new##Attr; AttrLink(Attr); touch();} /*!<
 * The AttrSetter macro defines a generic setter method for the attribute named \c attr of type \c Type.
 *
 * With respect to the \ref object.h "name convention", this macro needs the attribute's name with lower and upper
 * initial letter case.
 *
 * \b Example
 * \code{.cpp}
 * AttrSetter(parent,Parent,GameObject)
 *     --> inline void setParent(GameObject* &parentObject){aParent = parentObject; touch();}
 * \endcode
 * \see Attr, AttrGetter, C
 */
#define AttrDef(Attr, Type) private: Type* a##Attr = nullptr; public: /*!<
 * The AttrDef macro defines a private attribute name <a\c Attr>.
 *
 * \note
 * To avoid redefinition error, no attribute or method name <a\c Attr > must exist.
 *
 * \warning
 * This macro is designed to be used in a public part of the class. Please note that
 * inserting this macro in a private or protected part will change the visibility of
 * the next declaration to public.
 *
 * \b Example
 * \code{.cpp}
 * Attr(Parent,GameObject)
 *     -->
 *     private:
 *         GameObject *aParent;
 *     public:
 * \endcode
 *
 * \see Attr
 */
#define Attr(attr, Attr, Type) AttrDef(Attr, Type) AttrGetter(attr,Attr,Type) AttrSetter(attr, Attr, Type) /*!<
 * The Attr macro defines a new <a\c Attr> named attribute of type \c Type, with its generic getter and setter methods.
 *
 * With respect to the \ref object.h "name convention", this macro needs the parameter's name with lower and upper
 * initial letter case.
 *
 * \b Example
 * \code{.cpp}
 * Attr(parent,Parent, GameObject)
 *     -->
 *     private:
 *         GameObject *aParent;
 *     public:
 *         inline GameObject* parent() const{return aParent;}
 *         inline void setParent(GameObject* parentObject){aParent = parentObject; touch();}
 * \endcode
 *
 * \see AttrT, AttrDef, AttrSetter, AttrGetter, C
 */
#define AttrT(type,Type) Attr(type, Type, Type) /*!<
 * The AttrT macro defines a new attribute of type \c Type, named after the type name, with its generic getter and setter methods.
 *
 * With respect to the \ref object.h "name convention", this macro needs the parameter's type with lower and upper
 * initial letter case.
 *
 * \b Example
 * \code{.cpp}
 * AttrT(cellType,CellType)
 *     -->
 *     private:
 *         CellType *aCellType;
 *     public:
 *         inline CellType* cellType() const{return aCellType;}
 *         inline void setCellType(CellType* cellTypeObject){aCellType = cellTypeObject; touch();}
 * \endcode
 *
 * \see Attr, AttrDef, AttrSetter, AttrGetter, C
 */


class Parameter
{
public:
    Parameter(int v = 0) : min(0), max(100){setValue(v);}
    Parameter(int min, int max, int v = 0): min(min), max(max){setValue(v);}
    void setValue(int v){pValue = std::min(std::max(min,v),max);}
    inline int value() const {return pValue;}

private:
    int min, max;
    int pValue;
};


class Game;

/*!
 * \brief The GameObject class is the base class for every part
 * of games.
 *
 * Each instance is identified by a game-wide unique
 * identifier.
 *
 *
 *
 * ## %Object edition notification mechanism
 *
 * To make the edition easier, each GameObject contains two QDateTime values :
 * - The most recent edition time, which is updated by the \ref touch method
 * - The most recent chidl edition time, also updated by the \ref touch method
 *
 * \note
 * If the changes that are made in the object have to be detected by display/edition
 * widgets, the \ref touch function should be called.
 * \note
 * To prevent the notification chain to be broken, the existing objects should always
 * have a parent (except for the root object). This can be acheived using the \ref init
 * or \ref setParent method, when the parent have not been given in the constructor.
 * (see \ref object.h for details)
 *
 *
 * ## References count
 *
 *
 * \todo
 *
 *
 */
class GameObject
{
public:
    TypeName(GameObject)

    GameObject(Game *g = nullptr, GameObject *aParent = nullptr); /**<
     * Constructs a new GameObject with parent \c parent and the reference to the game \c g.
     *
     * \note
     * If these objects cannot be given to the constructor (case of an array of objects), the
     * \ref init method must be called after the creation to make the GameObject valid.
     */
    virtual ~GameObject(); /**<
     * The default destructor destroy every children of the instance
     */

    void init(Game *g, GameObject *p); /**<
     * Initialises the object in case it had been construct with a NULL pointer (array of objects)
     *
     * \see isValid, \ref GameObject::GameObject "GameObject"
     */
    virtual bool isValid() const{return id;} /**<
     * Returns true if the object has been initialised
     *
     * \see init, \ref GameObject::GameObject "GameObject"
     */
    int ident() const{return id;} /**<
     * Returns the name wide unique identifier of the object.
     *
     * \see init, \ref GameObject::GameObject "GameObject"
     */

    inline const QDateTime& lastInternalEdition() const{return lastEdit;} /**<
     * Returns the last edition time.
     *
     * \see lastEdition, lastChildrenEdition
     */
    inline const QDateTime& lastChildrenEdition() const{return lastChildEdit;} /**<
     * Returns the last time one of the object's children has been modified.
     *
     * \see lastEdition, lastInternalEdition
     */
    inline const QDateTime& lastEdition() const{return lastEdit > lastChildEdit ? lastEdit : lastChildEdit;} /**<
     * Returns the last time a modification was made on the object or one of its children.
     *
     * \see lastInternalEdition, lastChildrenEdition
     */

    virtual int getParam(const QString &p) const {return aParams.value(p, 0);} /**<
     * Returns the value of the \c p parameter.
     *
     * \note
     * If the requested parameter does not exists, a null value is returned, and the parameters
     * map stay unchanged
     *
     * \see params, hasParam, setParam, getFlag
     */
    virtual void setParam(const QString &p, int v) {aParams[p] = v; touch();} /**<
     * Set the value of the \c p parameter.
     *
     * \note
     * If the requested parameter does not exists, it is created.
     *
     * \see params, hasParam, getParam, setFlag
     */
    virtual bool hasParam(const QString &p) const {return  aParams.contains(p);} /**<
     * Returns true if the parameter \p is register in the object's parameters.
     *
     * \see getParam, setParam, hasFlag
     */
    virtual QList<QString> params() const {return filter(aParams.keys());} /**<
     * Returns the list of the registered paramters
     *
     * \see getParam, setParam, flags
     */
    virtual bool getFlag(const QString &f) const{return aFlags.value(f,false);} /**<
     * Returns the value of the \c f flag.
     *
     * \note
     * If the requested parameter does not exists, a \c false value is returned, and the flags
     * map stay unchanged
     *
     * \see flags, hasFlag, setFlag, getParam
     */
    virtual void setFlag(const QString &f, bool v) {aFlags[f] = v; touch();} /**<
     * Set the value of the \c f flag.
     *
     * \note
     * If the requested flag does not exists, it is created.
     *
     * \see flags, hasFlag, getFlag, setParam
     */
    virtual bool hasFlag(const QString &f) const {return aFlags.contains(f);} /**<
     * Returns true if the falg \c f is register in the object's flags.
     *
     * \see getFlag, setFlag, hasParam
     */
    virtual QList<QString> flags() const {return filter(aFlags.keys());} /**<
     * Returns the list of the registered flags
     *
     * \see getFlag, setFlag, params
     */

    void touch(); /**<
     * Notify the object and its parent that it has been modified.
     *
     * \see lastInternalEdition, lastChildrenEdition, lastEdition.
     */
    void addReference();/**<
     * Increases the references counter
     *
     * \see removeReference
     */
    void removeReference();/**<
     * Decreases the references counter
     *
     * \see addReference
     */

    void setParent(GameObject *p); /**<
     * Defines the parent of the object.
     *
     * \see parent
     */
    inline GameObject* parent() const{return aParent;} /**<
     * Returns the parent of the object.
     *
     * \see setParent
     */

    void setName(const QString &n); /**<
     * Defines the name of the object.
     *
     * \see name
     */
    const QString& name() const{return aName;} /**<
     * Returns the name of the object.
     *
     * \see setName
     */
    virtual GameObject* child(const int &i) const{return aChildren.value(i, nullptr);} /**<
     * Returns the child with identifier \c i if any.
     *
     * \see children;
     */
    virtual QList<GameObject*> children() const{return aChildren.values();} /**<
     * Returns the list of the instance's children.
     *
     * \see child
     */


protected:
    void addChild(GameObject *c); /**<
     * Registers a new child.
     *
     * \see removeChild, child, children
     */
    void removeChild(GameObject *c); /**<
     * Removes a child from the children list.
     *
     * \note
     * This method does not destroy the child.
     *
     * \see addChild, child, children
     */
    void childrenTouched(const QDateTime &d);
    QList<QString> filter(QList<QString> l) const;

    GameObject *aParent;
    QMap<int, GameObject*> aChildren;
    Game *game;
    int nbRef;
    QMap<QString, int> aParams;
    QMap<QString, bool> aFlags;
    QString aName;
    QString fileName;
    QDateTime lastEdit, lastChildEdit;
    QSet<QString> reserved;

private:
    int id;

};


/*!
 * \brief The Type class
 */
class Type : public GameObject
{
public:
    Type(Game*g, GameObject *aParent) : GameObject(g, aParent){} // temporaire

    virtual int getParam(const QString &p) const {return aParams.value(p, 0);}
    virtual void setParam(const QString &p, int v) {aParams[p] = v; touch();}
    virtual bool hasParam(const QString &p) const {return  aParams.contains(p) || (aAncestor && aAncestor->hasParam(p));}
    virtual QList<QString> params() const {return filter(aParams.keys());}
    virtual bool getFlag(const QString &f) const{return aFlags.value(f,false);}
    virtual void setFlag(const QString &f, bool v) {aFlags[f] = v; touch();}
    virtual bool hasFlag(const QString &f) const {return aFlags.contains(f) || (aAncestor && aAncestor->hasFlag(f));}
    virtual QList<QString> flags() const {return filter(aFlags.keys());}

protected:
    Type *aAncestor;

};




/*!
 * \brief The TypedObject class
 *
 * The \c class \c T template argument has to be inherited from Type.
 */
template<class T>
class TypedObject : public GameObject
{
public:
    TypedObject(Game*g, GameObject *aParent) : GameObject(g, aParent){} // temporaire
// Appeler une fonction spécifique à Type ! (pour forcer les type de T
    const T *objectType() const{return aType;}

protected:
    T *aType;

};









/*!
 * \brief The Image class stores an external file in a QImage, and
 * gives each image ressources a unique identifier.
 */
class Image : public GameObject
{
public:
    TypeName(Image)
    Image(Game*g, GameObject *aParent, const QString &fileName);
    inline bool isValid() const{return GameObject::isValid() && !im.isNull();} /**<
     * Checks if the GameObject is valid and if the image really exists.
     */
    inline const QImage& image() const{return im;}
    inline const QSize size() const{return im.size();}
    void update(){} // TODO
private:
    QImage im;
};


/*!
 * \brief The Object class
 */
class Object : public GameObject
{
public:
    TypeName(Object)
    Object(Game *g, GameObject *aParent);

    C0(Flag, v,V,isible)
    C0(Flag, m,M,ovable)
    C0(Flag, i,I,nteractive)
    C0(AttrT,i,I,mage)


};




#endif // OBJECT_H
