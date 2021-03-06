#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H


#include <QtGui>
#include <assert.h>
#include <algorithm>

/*!
 * \file gameobject.h
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
 * Attribute type               | Complete declaration  | Getter      | Setter
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



bool cleverComp(const QString &na, const QString &nb);  /**<
 * Compares to string classing \c name_10 after \c name_2.
 */
bool isValidName(const QString &n);                     /**<
 * Returns true if the string \c n contains only letters, digits or "_".
 */

#define UNUSED(p)                                                                                                                           /*!<
 * Usefull macro to avoid "Unused parameter" warning.
 */

#define TypeName(Type) virtual QString typeName() const{return QObject::tr(#Type);}                                                         /*!<
 * This macro declares the type name that will be visible to the user.
 *
 * It must be use in a GameObject inherited class public scope.
 */

#define C(Macro, init,Init,body, ...) Macro(init##body, Init##body, ##__VA_ARGS__)                                                          /*!<
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
#define C0(Macro, init,Init,body) Macro(init##body, Init##body)                                                                             /*!<
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
#define C1(Macro, init,Init,body, arg) Macro(init##body, Init##body, arg)                                                                   /*!<
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
#define ProtectFlag(flag) reserved.insert(QString(#flag));                                                                                  /*!<
 * The ProtectFlag macro registers the flag named \c flag as protected
 * \i ie it cannot be modified directly in a flag editor (it will not appear).
 *
 * \see flag, ProtectParam
 */
#define SetFlag(flag, value) setFlag(#flag,value)                                                                                           /*!<
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
#define FlagGetter(flag, Flag) inline bool is##Flag() const{return getFlag(#flag);}                                                         /*!<
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
#define FlagSetter(flag, Flag) inline void set##Flag(bool flag){SetFlag(flag,flag); touch();}                                               /*!<
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
#define Flag(flag, Flag) FlagGetter(flag, Flag) FlagSetter(flag, Flag)                                                                      /*!<
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
#define ProtectParam(param) reserved.insert(QString(#param));                                                                               /*!<
 * The ProtectParam macro registers the flag named \c param as protected
 * \i ie it cannot be modified directly in a param editor (it will not appear).
 *
 * \see param, ProtectFlag
 */
#define SetParam(param, value) setParam(#param,value)                                                                                       /*!<
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
#define ParamGetter(param) inline int param() const{return getParam(#param);}                                                               /*!<
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
#define ParamSetter(param, Param) inline void set##Param(int param##Value){SetParam(param,param##Value); touch();}                          /*!<
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

#define Param(param, Param) ParamGetter(param) ParamSetter(param, Param) ParamDom(param,Param)                                              /*!<
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
#define AttrGetter(attr, Attr, Type) inline Type* attr() const{return a##Attr;}                                                             /*!<
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
#define AttrFree(Attr) if(a##Attr) a##Attr->removeReference();                                                                              /*!<
 * The AttrFree macro decrease the number of references of the GameObject attribut attr.
 * It should be used before the deletion/modifcation of the pointer.
 *
 * \see AttrLink
 */
#define AttrLink(Attr) if(a##Attr) a##Attr->addReference();                                                                                 /*!<
 * The AttrLink macro increase the number of references of the GameObject attribut attr.
 * It should be used before the saving of a pointer as an attribute.
 *
 * \see AttrFree
 */
#define AttrSetter(attr, Attr, Type) inline void set##Attr(Type* new##Attr){AttrFree(Attr); a##Attr = new##Attr; AttrLink(Attr); touch();}  /*!<
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
#define AttrDef(Attr, Type) private: Type* a##Attr = nullptr; public:                                                                       /*!<
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
#define Attr(attr, Attr, Type) AttrDef(Attr, Type) AttrGetter(attr,Attr,Type) AttrSetter(attr, Attr, Type)                                  /*!<
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
#define AttrT(type,Type) Attr(type, Type, Type)                                                                                             /*!<
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
    Parameter(const Parameter &p) : Parameter(p.min, p.max, p.pValue){}
    void setValue(int v){pValue = std::min(std::max(min,v),max);}
    inline int value() const {return pValue;}
    void setMinimum(int m) {min = m; max = std::max(max,min); setValue(pValue);}
    inline int minimum() const {return min;}
    void setMaximum(int m) {max = m; min = std::min(max,min); setValue(pValue);}
    inline int maximum() const {return max;}
    void setDomain(int minVal, int maxVal) {min = minVal; max = std::max(maxVal,min); setValue(pValue);}

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
    Event(const Event *e) : aType(e->aType){}

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
        ChangeFlag,
        EmitEvent,
        ConditionalEvent,
        WatchDogEvent,
        SetTimer,
        SetObject
    };

    Order(){}
    Order(const Order *o) : aType(o->aType){}

    QString typeName(){return "order";}
private:
    OrderType aType;
};


class GameObject;
/*!
 * \brief The Action class describes a game Action, which if the
 * basal part to enable animated games.
 */
class Action
{
public:
    Action();
    ~Action();
    GameObject *emitter() const;
    void setEmitter(GameObject *emitter);
    const QString &event() const;
    void setEvent(const QString &event);
    const QList<QPair<GameObject*, QString>> &receivers() const;
    void addReceiver(GameObject *receiver, const QString &order);
    void removeReceiver(GameObject *receiver);
    void removeReceiver(GameObject *receiver, const QString &order);

    bool isValid() const;
private:
    GameObject *aEmitter;
    QString aEvent;
    QList<QPair<GameObject*, QString>> aReceivers;
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
    GameObject(GameObject &parent);                                                             /**<
     * Constructs a new GameObject with parent \c parent.
     */
    GameObject(Game *g = nullptr, GameObject *parent = nullptr);                                /**<
     * Constructs a new GameObject with parent \c parent and the reference to the game \c g.
     *
     * \note
     * If these objects cannot be given to the constructor (case of an array of objects), the
     * \ref init method must be called after the creation to make the GameObject valid.
     */

public:
    virtual ~GameObject();                                                                      /**<
     * The default destructor destroy every children of the instance
     */

    TypeName(GameObject)

    inline int ident() const{return id;}                                                        /**<
     * Returns the name wide unique identifier of the object.
     *
     * \see init, \ref GameObject::GameObject "GameObject"
     */
    virtual bool isEditable() const{return true;}                                               /**<
     * Returns \c true if the GameObject is editable by the user.
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


    virtual bool hasEvent(const QString &event) const;                                          /**<
     * Return \c true if the GameObject has a event named \c event.
     *
     * \see getEvent, getEvents, hasOrder
     */
    virtual Event& getEvent(const QString &event) const;                                        /**<
     * Returns the event named \c event.
     *
     * \note
     * If the requested event does not exist, an empty
     * Event is inserted.
     *
     * \see getEvents, getOrder
     */
    virtual Event& addEvent(const QString &event);                                              /**<
     * Create a new event named \c event.
     *
     * \note
     * If the a event name \c event already exists, it will be discarded.
     *
     * \see removeEvent, addOrder
     */
    virtual void removeEvent(const QString &event);                                             /**<
     * Delete the event named \c event, if exists.
     *
     * \see addEvent, removeOrder
     */
    virtual QList<QString> events() const;                                                      /**<
     * Returns the list of event's names of the object.
     *
     * \see getEvent, getOrders
     */
    virtual void renameEvent(const QString &order, const QString &newOrder);                    /**<
     * Changes the name of the \c event event to \c newEvent.
     *
     * \note
     * If the requested event does not exist, a new event is created.
     *
     * \see addEvent, removeEvent
     */

    virtual bool hasOrder(const QString &order) const;                                          /**<
     * Return \c true if the GameObject has a order named \c order.
     *
     * \see getOrder, getOrders, hasEvent
     */
    virtual Order& getOrder(const QString &order) const;                                        /**<
     * Returns the order named \c order.
     *
     * \note
     * If the requested order does not exist, an empty
     * Order object is inserted.
     *
     * \see getOrders, getEvent
     */
    virtual Order& addOrder(const QString &order);                                              /**<
     * Create a new order named \c order.
     *
     * \note
     * If the a order name \c order already exists, it will be discarded.
     *
     * \see removeOrder, addEvent
     */
    virtual void removeOrder(const QString &order);                                             /**<
     * Delete the order named \c order, if exists.
     *
     * \see addOrder, removeEvent
     */
    virtual QList<QString> orders() const;                                                      /**<
     * Returns the list of order's names of the object.
     *
     * \see getOrder, getEvents
     */
    virtual void renameOrder(const QString &order, const QString &newOrder);                    /**<
     * Changes the name of the \c order order to \c newOrder.
     *
     * \note
     * If the requested order does not exist, a new order is created.
     *
     * \see addOrder, removeOrder
     */

    void addEmittedAction(Action *action);                                                      /**<
     * Register the GameObject as the emitter of the action \c action.
     *
     * This is usefull to avoid dangling references.
     *
     * \see removeEmittedAction, addReceivedAction
     */
    void removeEmittedAction(Action *action);                                                   /**<
     * Free the GameObject from the action \c action as its emitter.
     *
     * This is usefull to avoid dangling references.
     *
     * \see addEmittedAction, removeReceivedAction
     */
    void addReceivedAction(Action *action);                                                     /**<
     * Register the GameObject as one of the receivers of the action \c action.
     *
     * This is usefull to avoid dangling references.
     *
     * \see removeReceivedAction, addEmittedAction
     */
    void removeReceivedAction(Action *action);                                                  /**<
     * Free the GameObject from the action \c action as one of its receivers.
     *
     * This is usefull to avoid dangling references.
     *
     * \see addReceivedAction, removeEmittedAction
     */

    Game *getGame(){return game;}                                                               /**<
     * Returns the Game from which the GameObject is part.
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

    void copy(GameObject &obj);                                                                 /**<
     * Loads the content of \c obj in the current instance.
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
    void childrenTouched(const QDateTime &d);                                                   /**<
     * Registers a change in one of the child of the object.
     */
    QList<QString> filter(QList<QString> l) const;                                              /**<
     * Removes the protected parameters/flags for the user visible lists.
     */

    GameObject *aParent;
    QMap<int, GameObject*> aChildren;
    Game* game;
    int nbRef;
    QMap<QString, Parameter> aParams;
    QMap<QString, bool> aFlags;
    QMap<QString, Event*> aEvents;
    QMap<QString, Order*> aOrders;
    QString aName;
    QString fileName;
    QDateTime lastEdit, lastChildEdit;
    QSet<QString> reserved;
    QList<Action*> aEmittedActions;
    QList<Action*> aReceivedActions;
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
    InheritableObject *ancestor();                                  /**<
     * Returns the object from with the current instance inherits
     *
     * \see hasAncestor
     */

    virtual bool isInheritedParam(const QString &param) const;      /**<
     * Returns true if the \c param parameter is define in one of
     * the ancestors of the object.
     *
     * \see isRedefiniedParam, isInheritedFlag isInheritedEvent, isInheritedOrder
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
     * \see hasEvent, GameObject::getEvent, getFlag, getEvent, getOrder
     */
    virtual bool hasParam(const QString &param) const;              /**<
     * Returns true if the \c param parameter is defined in the
     * object or one of its ancestors.
     *
     * \see getParam, GameObject::hasParam, hasFlag, hasEvent, hasOrder
     */
    virtual QList<QString> params() const;                          /**<
     * Returns the list of the parameters of the object,
     * both proper and inherited.
     *
     * \see properParams, paramTree, flags, events, orders
     */
    virtual QList<QString> properParams() const;                    /**<
     * Returns the list of the parameters that are only defined
     * in the object (the uninherited parameters)
     *
     * \see params, paramTree, properFlags, properEvents, properOrders
     */
    HierarchicalAttr paramTree() const;                             /**<
     * Returns the hierarchy of parameters, that is the list of
     * ancestors and which parameters they define.
     *
     * \see params, properParams, flagTree, eventTree, orderTree
     */

    virtual bool isInheritedFlag(const QString &flag) const;        /**<
     * Returns true if the \c flag flag is define in one of
     * the ancestors of the object.
     *
     * \see isRedefiniedFlag, isInheritedParam isInheritedEvent, isInheritedOrder
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
     * \see hasEvent, GameObject::getEvent, getParam, getEvent, getOrder
     */
    virtual bool hasFlag(const QString &flag) const;                /**<
     * Returns true if the \c flag flag is defined in the
     * object or one of its ancestors.
     *
     * \see getFlag, GameObject::hasFlag, hasParam, hasEvent, hasOrder
     */
    virtual QList<QString> flags() const;                           /**<
     * Returns the list of the flags of the object,
     * both proper and inherited.
     *
     * \see properFlags, flagTree, params, events, orders
     */
    virtual QList<QString> properFlags() const;                     /**<
     * Returns the list of the flags that are only defined
     * in the object (the uninherited flags)
     *
     * \see flags, flagTree, properParams, properEvents, properOrders
     */
    HierarchicalAttr flagTree() const;                              /**<
     * Returns the hierarchy of flags, that is the list of
     * ancestors and which flags they define.
     *
     * \see flags, properFlags, paramTree, eventTree, orderTree
     */

    virtual bool isInheritedEvent(const QString &event) const;      /**<
     * Returns true if the \c event event is define in one of
     * the ancestors of the object.
     *
     * \see isInheritedParam, isInheritedFlag, isInheritedOrder
     */
    virtual bool hasEvent(const QString &event) const;              /**<
     * Returns true if the \c event event is defined in the
     * object or one of its ancestors.
     *
     * \see getEvent, GameObject::hasEvent, hasParam, hasFlag, hasOrder
     */
    virtual Event& getEvent(const QString &event) const;            /**<
     * Returns the \c event event, loocking
     * for it in the different ancestors if not found.
     *
     * \see hasEvent, GameObject::getEvent, getParam, getFlag, getOrder
     */
    virtual QList<QString> events() const;                          /**<
     * Returns the list of the events of the object,
     * both proper and inherited.
     *
     * \see properEvents, eventTree, params, flags, orders
     */
    virtual QList<QString> properEvents() const;                    /**<
     * Returns the list of the events that are only defined
     * in the object (the uninherited events)
     *
     * \see events, eventTree, properParams, properFlags, properOrders
     */
    HierarchicalAttr eventTree() const;                             /**<
     * Returns the hierarchy of event, that is the list of
     * ancestors and which event they define.
     *
     * \see events, properEvents, paramTree, flagTree, orderTree
     */

    virtual bool isInheritedOrder(const QString &order) const;      /**<
     * Returns true if the \c order order is define in one of
     * the ancestors of the object.
     *
     * \see isInheritedParam, isInheritedFlag isInheritedEvent
     */
    virtual bool hasOrder(const QString &order) const;              /**<
     * Returns true if the \c order order is defined in the
     * object or one of its ancestors.
     *
     * \see getOrder, GameObject::hasOrder, hasParam, hasFlag, hasEvent
     */
    virtual Order& getOrder(const QString &order) const;            /**<
     * Returns the \c order order, loocking
     * for it in the different ancestors if not found.
     *
     * \see hasOrder, GameObject::getEvent, getParam, getFlag, getEvent
     */
    virtual QList<QString> orders() const;                          /**<
     * Returns the list of the orders of the object,
     * both proper and inherited.
     *
     * \see properOrders, orderTree, params, flags, events
     */
    virtual QList<QString> properOrders() const;                    /**<
     * Returns the list of the orders that are only defined
     * in the object (the uninherited orders)
     *
     * \see orders, orderTree, properParams, properFlags, properEvents
     */
    HierarchicalAttr orderTree() const;                             /**<
     * Returns the hierarchy of orders, that is the list of
     * ancestors and which orders they define.
     *
     * \see orders, properOrders, paramTree, flagTree, eventTree
     */

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
/*!
 * \brief The GameObjectType class represents types of gameobjects that can
 * be inherited and will be used in TypedObject.
 */
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
