#ifndef OBJECT_H
#define OBJECT_H

#include <QtCore>
#include <QtGui>
#include <assert.h>

/*!
 * \file object.h
 *
 * \brief This header defines the base class Object and Image.
 *
 * ## The objects structure
 *
 *
 *
 *
 * ## The Macro System
 *
 * To add conveniently attributes and flags to BaseObject subclassed objects,
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
 * BaseObject based Attributes  | #Attr                 |#AttrGetter  |#AttrSetter
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



/*! Internal used only
  */
#define ObjectsMapC(name,names,Type,Types,pref,arg) \
private: \
    QMap<int, Type*> pref##Types; \
public: \
    void add##Type(Type* arg){pref##Types[arg->ident()] = arg; touch();} \
    void remove##Type(Type* arg){if(pref##Types.contains(arg->ident()))pref##Types.remove(arg->ident()); touch();} \
    inline Type* name(int id) const{return pref##Types.contains(id) ? pref##Types[id] : nullptr;} \
    inline QList<Type*> names() const{return pref##Types.values();}
/*! The macro ObjectsMap define the methods necessary to
 * manipulate a set of objects
 */
#define ObjectsMap(pref,ini,Ini,body,sg,pl) \
    ObjectsMapC(ini##body##sg, ini##body##pl, Ini##body##sg, Ini##body##pl, pref,ini)


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
 * C(Attr, p,P,arent, BaseObject)
 *     --> Attr(parent, Parent, BaseObject)
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
 * C(Attr, p,P,arent, BaseObject)
 *     --> Attr(parent, Parent, BaseObject)
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
 * The \ref BaseObject::touch "touch" function isn't called. After this macro use,
 * the \ref BaseObject "object" is no longer synchronised.
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
 * The \ref BaseObject::touch "touch" function isn't called. After this macro use,
 * the \ref BaseObject "object" is no longer synchronised.
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
#define AttrGetter(attr, Attr, Type) inline Type *attr() const{return a##Attr;} /*!<
 * The AttrGetter macro defines a generic getter method for the attribute named \c attr of type \c Type.
 *
 * With respect to the \ref object.h "name convention", this macro needs the attribute's name with lower and upper
 * initial letter case.
 *
 * \b Example
 * \code{.cpp}
 * AttrGetter(parent,Parent,BaseObject)
 *     --> inline BaseObject* parent() const{return aParent;}
 * \endcode
 * \see Attr, AttrSetter, C
 */
#define AttrSetter(attr, Attr, Type) inline void set##Attr(Type *attr##Object){a##Attr = attr##Object; touch();} /*!<
 * The AttrSetter macro defines a generic setter method for the attribute named \c attr of type \c Type.
 *
 * With respect to the \ref object.h "name convention", this macro needs the attribute's name with lower and upper
 * initial letter case.
 *
 * \b Example
 * \code{.cpp}
 * AttrSetter(parent,parent,BaseObject)
 *     --> inline void setParent(BaseObject* parentObject){aParent = parentObject; touch();}
 * \endcode
 * \see Attr, AttrGetter, C
 */
#define AttrDef(Attr, Type) private: Type *a##Attr; public: /*!<
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
 * Attr(Parent,BaseObject)
 *     -->
 *     private:
 *         BaseObject *aParent;
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
 * Attr(parent,Parent, BaseObject)
 *     -->
 *     private:
 *         BaseObject *aParent;
 *     public:
 *         inline BaseObject* parent() const{return aParent;}
 *         inline void setParent(BaseObject* parentObject){aParent = parentObject; touch();}
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
 * \brief The BaseObject class is the base class for every part
 * of games.
 *
 * Each instance is identified by a game-wide unique
 * identifier.
 *
 * On each modification, the lastEdit attribute has to be updated, in order that
 * other \ref Object "objects" can see that modifications occured.
 */
class BaseObject
{
public:
    BaseObject(Game *g = nullptr, BaseObject *parent = nullptr);
    void init(Game *g, BaseObject *p); /**< initialise the object in case it had been construct with a NULL pointer (array of objects)*/
    int ident() const{return id;}
    virtual bool isValid() const{return id;} /**< return true if the object has been initialised*/
    inline const QDateTime& lastModification() const{return lastEdit;} /**< return the last time of modification*/

    inline int getParam(const QString &p) const {assert(hasParam(p)); return aParams[p];} /**<
     * Returns the value of the \c p parameter.
     *
     * \warning
     * This method assumes that the requested parameter exists.
     *
     * \see params, hasParam, getFlag
     */
    inline bool hasParam(const QString &p) const{return  aParams.contains(p);} /**<
     * Returns true if the parameter \p is register in the object's parameters.
     *
     * \see getParam, hasFlag
     */
    inline QList<QString> &params() const{return aParams.keys();}/**<
     * Returns the list of the registered paramters
     *
     * \see getParam, flags
     */
    inline bool getFlag(const QString &f) const{return aFlags[f];} /**<
     * Returns the value of the \c f flag.
     *
     * \warning
     * This method assumes that the requested flag exists.
     *
     * \see flags, hasFlag, getParam
     */
    inline bool hasFlag(const QString &f) const{return aFlags.contains(f);} /**<
     * Returns true if the falg \c f is register in the object's flags.
     *
     * \see getFlag, hasParam
     */
    inline QList<QString> &flags() const{return aFlags.keys();} /**<
     * Returns the list of the registered flags
     *
     * \see getFlag, params
     */

    void touch();/**< As expected */



protected:
    BaseObject *parent;
    Game *game;
    int id;
    QMap<QString, int> aParams;
    QMap<QString, bool> aFlags;
    QString fileName;
    QDateTime lastEdit, lastChildEdit;
};








class Object : public BaseObject
{
public:
    Object(Game *g, BaseObject *parent);
    C0(Flag, v,V,visible)

};


/*!
 * \brief The Image class stores an external file in a QImage, and
 * gives each image ressources a unique identifier.
 */
class Image : public BaseObject
{
public:
    Image(Game*g, BaseObject *parent, const QString &fileName);
    inline bool isValid() const{return BaseObject::isValid() && !im.isNull();}
    inline const QImage& image() const{return im;}
    inline const QSize size() const{return im.size();}
    void update(){} // TODO
private:
    QImage im;
};




#endif // OBJECT_H
