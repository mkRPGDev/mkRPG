#ifndef OBJECT_H
#define OBJECT_H

#include <QtCore>
#include <QtGui>
#include <assert.h>

/*!
 * \file object.h
 *
 * \brief Definition of the base class GameObject, and some inherited classes.
 *
 * ## The objects structure
 *
 * ### %Objects destructors
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




#define ObjectsMapC(name,names,Type,Types,pref,arg) \
private: \
    QMap<int, Type*> pref##Types; \
public: \
    void add##Type(Type* arg){pref##Types[arg->ident()] = arg; touch();} \
    void remove##Type(Type* arg){if(pref##Types.contains(arg->ident()))pref##Types.remove(arg->ident()); touch();} \
    inline Type* name(int id) const{return pref##Types.value(id, nullptr);} \
    inline QList<Type*> names() const{return pref##Types.values();} /**< \deprecated*/
#define ObjectsMap(pref,ini,Ini,body,sg,pl) \
    ObjectsMapC(ini##body##sg, ini##body##pl, Ini##body##sg, Ini##body##pl, pref,ini) /**< \deprecated*/


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
#define SetFlag(flag, value) aFlags[#flag] = value /*!<
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
#define FlagGetter(flag, Flag) inline bool is##Flag() const{return aFlags[#flag];} /*!<
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
#define SetParam(param, value) aParams[#param] = value /*!<
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
#define ParamGetter(param) inline int param() const{return aParams[#param];} /*!<
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
#define AttrFree(Attr) if(a##Attr) a##Attr->removeReference();
#define AttrLink(Attr) if(a##Attr) a##Attr->addReference(); //TODO mise à jour doc.
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


    GameObject(Game *g = nullptr, GameObject *parent = nullptr); /**<
     * Constructs a new GameObject with parent \c parent and the reference to the game \c g.
     *
     * \note
     * If these objects cannot be given to the constructor (case of an array of objects), the
     * \ref init method must be called after the creation to make the GameObject valid.
     */
    virtual ~GameObject();

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

    inline int getParam(const QString &p) const {return aParams.value(p, 0);} /**<
     * Returns the value of the \c p parameter.
     *
     * \note
     * If the requested parameter does not exists, a null value is returned, and the parameters
     * map stay unchanged
     *
     * \see params, hasParam, setParam, getFlag
     */
    inline void setParam(const QString &p, int v) {aParams[p] = v; touch();} /**<
     * Set the value of the \c p parameter.
     *
     * \note
     * If the requested parameter does not exists, it is created.
     *
     * \see params, hasParam, getParam, setFlag
     */
    inline bool hasParam(const QString &p) const{return  aParams.contains(p);} /**<
     * Returns true if the parameter \p is register in the object's parameters.
     *
     * \see getParam, setParam, hasFlag
     */
    inline QList<QString> params() const{return aParams.keys();} /**<
     * Returns the list of the registered paramters
     *
     * \see getParam, setParam, flags
     */
    inline bool getFlag(const QString &f) const{return aFlags.value(f,false);} /**<
     * Returns the value of the \c f flag.
     *
     * \note
     * If the requested parameter does not exists, a \c false value is returned, and the flags
     * map stay unchanged
     *
     * \see flags, hasFlag, setFlag, getParam
     */
    inline void setFlag(const QString &f, bool v) {aFlags[f] = v; touch();} /**<
     * Set the value of the \c f flag.
     *
     * \note
     * If the requested flag does not exists, it is created.
     *
     * \see flags, hasFlag, getFlag, setParam
     */
    inline bool hasFlag(const QString &f) const{return aFlags.contains(f);} /**<
     * Returns true if the falg \c f is register in the object's flags.
     *
     * \see getFlag, setFlag, hasParam
     */
    inline QList<QString> flags() const{return aFlags.keys();} /**<
     * Returns the list of the registered flags
     *
     * \see getFlag, setFlag, params
     */

    void touch(); /**<
     * Notify the object and its parent that it has been modified.
     *
     * \see lastInternalEdition, lastChildrenEdition, lastEdition.
     */
    void addReference();
    void removeReference();

    void setParent(GameObject *p);


protected:
    void addChild(GameObject *c);
    void removeChild(GameObject *c);
    void childrenTouched(const QDateTime &d);

    GameObject *parent;
    QMap<int, GameObject*> children;
    Game *game;
    int id;
    int nbRef;
    QMap<QString, int> aParams;
    QMap<QString, bool> aFlags;
    QString fileName;
    QDateTime lastEdit, lastChildEdit;
};







/*!
 * \brief The Image class stores an external file in a QImage, and
 * gives each image ressources a unique identifier.
 */
class Image : public GameObject
{
public:
    Image(Game*g, GameObject *parent, const QString &fileName);
    inline bool isValid() const{return GameObject::isValid() && !im.isNull();}
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
    Object(Game *g, GameObject *parent);

    C0(Flag, v,V,visible)
    C0(Flag, m,M,ovable)
    C0(Flag, i,I,nteractive)
    C0(AttrT,i,I,mage)


};




#endif // OBJECT_H
