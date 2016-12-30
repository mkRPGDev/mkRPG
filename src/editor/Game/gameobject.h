#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H


#include <QtGui>
#include <assert.h>
#include <algorithm>


/*!
 * \file gameobject.h
 *
 * \brief Definition of the base class of the game internal model.
 */

bool cleverComp(const QString &na, const QString &nb);


#define UNUSED(p)

#define TypeName(Type) virtual QString typeName() const{return QObject::tr(#Type);}

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
#define ParamMGetter(param, Param, M) inline int param##M() const {return getParam##M(#param);}
#define ParamMSetter(param, Param, M) void set##Param##M(int param##M) {setParam##M(#param, param##M);}
#define ParamMin(param, Param) ParamMGetter(param, Param, Min) ParamMSetter(param, Param, Min)
#define ParamMax(param, Param) ParamMGetter(param, Param, Max) ParamMSetter(param, Param, Max)
#define ParamDom(param, Param) ParamMin(param,Param) ParamMax(param,Param)
// faire les Setter/Getter (min, max);

#define Param(param, Param) ParamGetter(param) ParamSetter(param, Param) ParamDom(param,Param) /*!<
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

/*!
 * \brief The Parameter class represents a parameter with a valid domain of
 * value.
 */
class Parameter
{
public:
    Parameter(int v = 0) : min(0), max(100){setValue(v);}
    Parameter(int min, int max, int v = 0): min(min), max(max){setValue(v);}
    void setValue(int v){pValue = std::min(std::max(min,v),max);}
    inline int value() const {return pValue;}
    void setMinimum(int m) {min = m; setValue(pValue);}
    inline int minimum() const {return min;}
    void setMaximum(int m) {max = m; setValue(pValue);}
    inline int maximum() const {return max;}
    void setDomain(int minVal, int maxVal) {min = minVal; max = maxVal; setValue(pValue);}

private:
    int min, max;
    int pValue;
};


/*!
 * \brief The Event class describes the
 */
class Event
{
public:
    /*!
     * \brief The EventType enum
     */
    enum EventType{
        ParamChanged,
        FlagChanged,
        ObjectRemoved,
        ObjectAdded
    };

    Event(){}

    QString typeName(){return "event";}
private:
    EventType aType;
};

/*!
 * \brief The Order class
 */
class Order
{
public:
    /*!
     * \brief The OrderType enum
     */
    enum OrderType{
        ChangeParam,
        ChangeFlag
    };

    Order(){}

    QString typeName(){return "order";}
private:
    OrderType aType;
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
protected:
    GameObject(GameObject &parent);
    GameObject(Game *g = nullptr, GameObject *parent = nullptr);                                /**<
     * Constructs a new GameObject with parent \c parent and the reference to the game \c g.
     *
     * \note
     * If these objects cannot be given to the constructor (case of an array of objects), the
     * \ref init method must be called after the creation to make the GameObject valid.
     */
    virtual ~GameObject();                                                                      /**<
     * The default destructor destroy every children of the instance
     */

public:
    TypeName(GameObject)

    inline int ident() const{return id;}                                                        /**<
     * Returns the name wide unique identifier of the object.
     *
     * \see init, \ref GameObject::GameObject "GameObject"
     */

    inline const QDateTime& lastInternalEdition() const{return lastEdit;}                       /**<
     * Returns the last edition time.
     *
     * \see lastEdition, lastChildrenEdition
     */
    inline const QDateTime& lastChildrenEdition() const{return lastChildEdit;}                  /**<
     * Returns the last time one of the object's children has been modified.
     *
     * \see lastEdition, lastInternalEdition
     */
    inline const QDateTime& lastEdition() const{return std::max(lastEdit, lastChildEdit);}      /**<
     * Returns the last time a modification was made on the object or one of its children.
     *
     * \see lastInternalEdition, lastChildrenEdition
     */

    void touch();                                                                               /**<
     * Notify the object and its parent that it has been modified.
     *
     * \see lastInternalEdition, lastChildrenEdition, lastEdition.
     */
    void addReference();                                                                        /**<
     * Increases the references counter
     *
     * \see removeReference
     */
    void removeReference();                                                                     /**<
     * Decreases the references counter
     *
     * \see addReference
     */

    void setParent(GameObject *p);                                                              /**<
     * Defines the parent of the object.
     *
     * \see parent
     */
    inline GameObject* parent() const{return aParent;}                                          /**<
     * Returns the parent of the object.
     *
     * \see setParent
     */

    virtual void setName(const QString &n);                                                     /**<
     * Defines the name of the object.
     *
     * \see name
     */
    const QString& name() const{return aName;}                                                  /**<
     * Returns the name of the object.
     *
     * \see setName
     */
    virtual GameObject* child(const int &id) const{return aChildren.value(id, nullptr);}        /**<
     * Returns the child with identifier \c i if any.
     *
     * \see children;
     */
    virtual QList<GameObject*> children() const{return aChildren.values();}                     /**<
     * Returns the list of the instance's children.
     *
     * \see child
     */

    virtual int getParamMin(const QString &param) const;                                        /**<
     * Returns the lower bound of the \c param parameter.
     *
     * \note
     * If the requested parameter does not exists, a default value is returned (0), and the parameters
     * stay unchanged.
     *
     * \see setParamMin, getParamMax
     */
    virtual int getParamMax(const QString &param) const;                                        /**<
     * Returns the upper bound of the \c param parameter.
     *
     * \note
     * If the requested parameter does not exists, a default value is returned (100), and the parameters
     * stay unchanged.
     *
     * \see setParamMax, getParamMin
     */
    virtual void setParamMin(const QString &param, int min);                                    /**<
     * Sets the lower bound of the \c param parameter.
     *
     * \note
     * If the requested parameter does not exists, the parameters
     * stay unchanged.
     *
     * \see getParamMin, setParamMax, setParamDomain
     */
    virtual void setParamMax(const QString &param, int max);                                    /**<
     * Sets the upper bound of the \c param parameter.
     *
     * \note
     * If the requested parameter does not exists, the parameters
     * stay unchanged.
     *
     * \see getParamMax, setParamMin, setParamDomain
     */
    virtual void setParamDomain(const QString &param, int min, int max);                        /**<
     * Sets the lower and upper bound of the \c param parameter.
     *
     * \note
     * If the requested parameter does not exists, the parameters
     * stay unchanged.
     *
     * \see getParamMin, getParamMax, setParamMin, setParamMax
     */
    virtual int getParam(const QString &param) const;                                           /**<
     * Returns the value of the \c param parameter.
     *
     * \note
     * If the requested parameter does not exists, a null value is returned, and the parameters
     * map stay unchanged
     *
     * \see params, hasParam, setParam, getFlag
     */
    virtual void setParam(const QString &param, int value);                                     /**<
     * Set the value of the \c param parameter.
     *
     * \note
     * If the requested parameter does not exists, it is created.
     *
     * \see params, hasParam, getParam, setFlag
     */
    virtual bool hasParam(const QString &param) const;                                          /**<
     * Returns true if the parameter \c param is register in the object's parameters.
     *
     * \see getParam, setParam, hasFlag
     */
    virtual QList<QString> params() const;                                                      /**<
     * Returns the list of the registered paramters
     *
     * \see getParam, setParam, flags
     */
    virtual void renameParam(const QString &param, const QString &newParam);                    /**<
     * Changes the name of the \c param parameter to \c newParam.
     *
     * \note
     * If the requested parameter does not exist, a new parameter is created.
     *
     * \see addParam, removeParam
     */
    virtual void addParam(const QString &param, int value = 0, int min = 0, int max = 100);     /**<
     * Inserts a new \c param parameter.
     *
     * \note
     * If the requested parameter already exist, it is replaced by the new one.
     *
     * \see renameParam, removeParam
     */
    virtual void removeParam(const QString &param);                                             /**<
     *  Erases the \c param parameter.
     *
     * \note
     * If the requested parameter does not exists, the parameters
     * stay unchanged.
     *
     * \see renameParam, addParam
     */

    virtual bool getFlag(const QString &flag) const;                                            /**<
     * Returns the value of the \c flag flag.
     *
     * \note
     * If the requested parameter does not exists, a \c false value is returned, and the flags
     * map stay unchanged
     *
     * \see flags, hasFlag, setFlag, getParam
     */
    virtual void setFlag(const QString &flag, bool enable);                                     /**<
     * Set the value of the \c flag flag.
     *
     * \note
     * If the requested flag does not exists, it is created.
     *
     * \see flags, hasFlag, getFlag, setParam
     */
    virtual bool hasFlag(const QString &flag) const;                                            /**<
     * Returns true if the falg \c flag is register in the object's flags.
     *
     * \see getFlag, setFlag, hasParam
     */
    virtual QList<QString> flags() const;                                                       /**<
     * Returns the list of the registered flags
     *
     * \see getFlag, setFlag, params
     */
    virtual void renameFlag(const QString &flag, const QString &newFlag);                       /**<
     * Changes the name of the \c flag flag to \c newFlag.
     *
     * \note
     * If the requested flag does not exist, a new flag is created.
     *
     * \see addFlag, removeFlag
     */
    virtual void addFlag(const QString &flag, bool enable = true);                              /**<
     * Inserts a new \c falg flag.
     *
     * \note
     * If the requested flag already exist, it is replaced by the new one.
     *
     * \see renameFlag, removeFlag
     */
    virtual void removeFlag(const QString &flag);                                               /**<
     *  Erases the \c flag flag.
     *
     * \note
     * If the requested flag does not exists, the falgs
     * stay unchanged.
     *
     * \see renameFlag, addFlag
     */


    virtual bool hasEvent(const QString &event) const;                                        /**<
     * Return \c true if the GameObject has a event named \c event.
     *
     * \see getEvent, getEvents, hasOrder
     */
    virtual Event& getEvent(const QString &event);                                           /**<
     * Returns the event named \c event.
     *
     * \note
     * If the requested event does not exist, an empty
     * Event is inserted.
     *
     * \see getEvents, getOrder
     */
    virtual Event& addEvent(const QString &event);                                           /**<
     * Create a new event named \c event.
     *
     * \note
     * If the a event name \c event already exists, it will be discarded.
     *
     * \see removeEvent, addOrder
     */
    virtual void removeEvent(const QString &event);                                           /**<
     * Delete the event named \c event, if exists.
     *
     * \see addEvent, removeOrder
     */
    virtual QList<QString> getEvents() const;                                                  /**<
     * Returns the list of event's names of the object.
     *
     * \see getEvent, getOrders
     */

    virtual bool hasOrder(const QString &order) const;                                            /**<
     * Return \c true if the GameObject has a order named \c order.
     *
     * \see getOrder, getOrders, hasEvent
     */
    virtual Order& getOrder(const QString &order);                                                 /**<
     * Returns the order named \c order.
     *
     * \note
     * If the requested order does not exist, an empty
     * Order object is inserted.
     *
     * \see getOrders, getEvent
     */
    virtual Order& addOrder(const QString &order);                                                 /**<
     * Create a new order named \c order.
     *
     * \note
     * If the a order name \c order already exists, it will be discarded.
     *
     * \see removeOrder, addEvent
     */
    virtual void removeOrder(const QString &order);                                               /**<
     * Delete the order named \c order, if exists.
     *
     * \see addOrder, removeEvent
     */
    virtual QList<QString> getOrders() const;                                                    /**<
     * Returns the list of order's names of the object.
     *
     * \see getOrder, getEvents
     */

protected:

    void init(Game *g, GameObject *parent);                                                     /**<
     * Initialises the object in case it had been construct with a NULL pointer (array of objects)
     *
     * \see isValid, \ref GameObject::GameObject "GameObject"
     */
    virtual bool isValid() const{return id;}                                                    /**<
     * Returns true if the object has been initialised
     *
     * \see init, \ref GameObject::GameObject "GameObject"
     */


    virtual void addChild(GameObject *c);                                                       /**<
     * Registers a new child.
     *
     * \see removeChild, child, children
     */
    virtual void removeChild(GameObject *c);                                                    /**<
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
    QMap<QString, Parameter> aParams;
    QMap<QString, bool> aFlags;
    QMap<QString, Event> aEvents;
    QMap<QString, Order> aOrders;
    QString aName;
    QString fileName;
    QDateTime lastEdit, lastChildEdit;
    QSet<QString> reserved;

private:
    int id;

};





/**
 * Convenient type that represent the attributes (flags/parameters) organized
 * by the oldest object that defines them.
 */
typedef QList<QPair<QString,QList<QString>>> HierarchicalAttr;


/*!
 * \brief The InheritableObject class is the base class for every objects
 * that can inherit attributes from an other InheritableObject.
 *
 * \note
 * The ancerstor of the object can be modified after instanciation.
 *
 * It is the base class of Type and TypedObject.
 *
 * \see Type, TypedObject
 */
class InheritableObject : public GameObject
{
protected:
    InheritableObject(GameObject &parent, InheritableObject *ancestor = nullptr);

public:
    bool hasAncestor() const;                                       /**<
     * Returns \c true if the object inherits from an other one.
     *
     * \see ancestor
     */
    InheritableObject *ancestor() const;                            /**<
     * Returns the object from with the current instance inherits
     *
     * \see hasAncestor
     */

    virtual bool isInheritedParam(const QString &param) const;      /**<
     * Returns true if the \c param parameter is define in one of
     * the ancestors of the object.
     *
     * \see isRedefiniedParam, isInheritedFlag
     */
    virtual bool isRedefiniedParam(const QString &param) const;     /**<
     * Returns true if the \c param parameter is an inherited parameter
     * with a object proper value.
     *
     * \see isInheritedParam, isRedefiniedFlag
     */
    virtual int getParamMin(const QString &param) const;            /**<
     * Returns the lower bound of the \c param parameter if
     * it exists (or is inherited).
     *
     * \note
     * If the \c param param is not defined, a default value
     * is returned (0).
     *
     * \see getParamMax
     */
    virtual int getParamMax(const QString &param) const;            /**<
     * Returns the upeer bound of the \c param parameter if
     * it exists (or is inherited).
     *
     * \note
     * If the \c param param is not defined, a default value
     * is returned (100).
     *
     * \see getParamMax
     */
    virtual void setParam(const QString &param, int value);         /**<
     * Reimplemented function.
     *
     * \see GameObject::setParam
     */
    virtual int getParam(const QString &param) const;               /**<
     * Returns the value of the \c param parameter, loocking
     * for it in the different ancestors if not found.
     *
     * \see hasParam, getFlag, GameObject::getParam
     */
    virtual bool hasParam(const QString &param) const;              /**<
     * Returns true if the \c param parameter is defined in the
     * object or one of its ancestors.
     *
     * \see getParam, hasFlag, GameObject::hasParam
     */
    virtual QList<QString> params() const;                          /**<
     * Returns the list of the parameters of the object,
     * both proper and inherited.
     *
     * \see properParams, paramTree, flags
     */
    virtual QList<QString> properParams() const;                    /**<
     * Returns the list of the parameters that are only defined
     * in the object (the uninherited parameters)
     *
     * \see params, paramTree, properFlags
     */
    HierarchicalAttr paramTree() const;                             /**<
     * Returns the hierarchy of parameters, that is the list of
     * ancestors and wich parameters they define.
     *
     * \see properParams, flagTree
     */

    virtual bool isInheritedFlag(const QString &flag) const;        /**<
     * Returns true if the \c flag flag is define in one of
     * the ancestors of the object.
     *
     * \see isRedefiniedFlag, isInheritedParam
     */
    virtual bool isRedefiniedFlag(const QString &flag) const;       /**<
     * Returns true if the \c flag flag is an inherited flag
     * with a object proper value.
     *
     * \see isInheritedFlag, isRedefiniedParam
     */
    virtual bool getFlag(const QString &flag) const;                /**<
     * Returns the value of the \c flag flag, loocking
     * for it in the different ancestors if not found.
     *
     * \see hasFlag, getParam, GameObject::getFlag
     */
    virtual bool hasFlag(const QString &flag) const;                /**<
     * Returns true if the \c flag flag is defined in the
     * object or one of its ancestors.
     *
     * \see getFlag, hasParam, GameObject::hasFlag
     */
    virtual QList<QString> flags() const;                           /**<
     * Returns the list of the flags of the object,
     * both proper and inherited.
     *
     * \see properFlags, flagTree, params
     */
    virtual QList<QString> properFlags() const;                     /**<
     * Returns the list of the flags that are only defined
     * in the object (the uninherited flags)
     *
     * \see paramTree, properFlags
     */
    HierarchicalAttr flagTree() const;                              /**<
     * Returns the hierarchy of flags, that is the list of
     * ancestors and wich flags they define.
     *
     * \see properFlags, paramTree
     */

    virtual bool isInheritedEvent(const QString &event) const;
    virtual bool hasEvent(const QString &event) const;
    virtual Event& getEvent(const QString &event);
    virtual QList<QString> getEvents() const;
    virtual QList<QString> properEvents() const;
    HierarchicalAttr eventTree() const;

    virtual bool isInheritedOrder(const QString &order) const;
    virtual bool hasOrder(const QString &order) const;
    virtual Order& getOrder(const QString &order);
    virtual QList<QString> getOrders() const;
    virtual QList<QString> properOrders() const;
    HierarchicalAttr orderTree() const;

protected:

    InheritableObject *aAncestor;

private:
    static void removeLastRedondances(HierarchicalAttr &attr);
};



/*!
 * \brief The GameObjectType class is the base class for GameObject
 * representing a type.
 *
 * It introduces a system of descendant registration, to make the tree
 * presentation easier (see ParamTreeItemModel, FlagTreeItemModel).
 *
 * The GameObjectType class has been introduced to enable the
 * polymorphism with types.
 *
 * \see Type
 */
class DefaultTypes;
class GameObjectType : public InheritableObject
{
protected:
    GameObjectType(DefaultTypes &parent);
    GameObjectType(GameObjectType &ancestor);
    virtual ~GameObjectType();
public:
    const QList<GameObjectType *> &descendants() const; /**<
     * Returns the list of types that inherite from the current instance.
     */
private:
    void addDescendant(GameObjectType &type);
    void removeDescendant(GameObjectType &type);
    QList<GameObjectType*> descendantTypes;
    GameObjectType* ancestorType;
};





#endif // GAMEOBJECT_H
