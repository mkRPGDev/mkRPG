#ifndef OPTIONS_H
#define OPTIONS_H

#include <QSettings>
#include <QDir>
#include <QSize>
#include <QPoint>
#include <QColor>

/*!
 * \file options.h
 * \brief Definition of the Options class, and the constants
 * that are used in this class.
 *
 * The headers of types which are used in the application must be include here.
 * See Options for details.
 *
 * \author Baptiste Pauget
 */

// TODO vérifier que les noms existent !

const QString ADAPT = "Adjustable";
const QString VAL = "Value";

#define WIN "Window" /**<
 * Group of window related options.
 */
#define DIR "Directories" /**<
    * Group of paths options.
    */
#define MAP "MapsEditor" /**<
    * Group of MapsEditor related options.
    */



#define DefaultF(group, opt, val) defaultValues[group][opt] = QPair<QVariant, bool>(val, false) /**<
 * This macro defines a new unadaptati option identified by its group and name.
 */
#define Default(group, opt, val) defaultValues[group][opt] = QPair<QVariant, bool>(val, true) /**<
 * This macro defines a new adaptati option identified by its group and name.
 */

/*!
 * \brief The Options class provides session-independant options and preferences.
 *
 *
 * ## Features
 *
 * The Options class aims at storing global options, that are available at any place
 * in the entire application. The preferences are permanantly stored and remain bewteen
 * the separate sessions and windows.
 *
 * Two sorts of options exist :
 * - The adjustable ones : the value of the option change when \ref save is called.
 * - The non-adjustable ones : the value of the option doesn't change is \ref save is called,
 * the option must be modified with \setDefault.
 *
 * The sort of option can be set with the \ref setAdjustable function.
 *
 *
 *
 * ## Design
 *
 * The Options class is designed following the __Singleton design pattern__.
 * The constructor is thus private, and the only Options instance is created at the
 * first call of \ref options.
 *
 * QSetting is used internaly, see Qt's documentation for details about the storing mechanisms.
 *
 *
 * ## Reading and writting existing options
 *
 * To read or write options, the Options instance must be retreived, using the \ref options function, then
 * the \ref load and \ref save functions can be called.
 *
 *
 *
 *
 * ## Adding options
 *
 * To add a new option, it is only needed to add a default hard coded value,
 * uisng the #Default and #DefaultF macros in the Options constructor.
 *
 * It is strongly advice to use macro to define new options group (see #WIN, for an example).
 *
 * \note
 * To use Options with custom types (other than ```C++``` standard),
 * the defining header of the type must be included at the top of the
 * \ref options.h file, in order to be used in the default value declaration.
 *
 * \warning
 * Pointer objects are not supported, and the result of the use of the Options
 * class with such values is undefined.
 *
 * \see options.h
 */
struct Options{
    template<class T>
    T load(QString group, QString opt){
        a.beginGroup(group);
        T val = a.value(opt+VAL,defaultValues[group][opt].first).template value<T>();
        a.endGroup();
        return val;
    } /**<
     * Reads an option defined by its group and name.
     *
     * \note The template argument must be precised
     * since it can't be deduced from arguments' types.
     *
     * \warning If the option type and the reading type mismatch, an default null value is returned.
     *
     * \see \ref save
     */
    template<class T>
    void save(QString group, QString opt, T val){
        a.beginGroup(group);
        if(a.value(opt+ADAPT, defaultValues[group][opt].second).toBool())
            a.setValue(opt+VAL, val);
        a.endGroup();
    } /**<
     * Writes the new value of the options defined by its group and name, if the option is adjustable.
     * See Options for details about options types.
     *
     * \note The template argument can be omitted since it would be deduced from the value argument.
     *
     * \see \ref setDefault, \ref load
     */
    template<class T>
    void setDefault(QString group, QString opt, T val){
        a.beginGroup(group);
        a.setValue(opt+VAL,val);
        a.endGroup();
    } /**<
     * Writes the new value of the options defined by its group and name, whatever the option type is.
     * See Options for details about options types.
     *
     * \note The template argument can be omitted since it would be deduced from the value argument.
     *
     * \see \ref save, \ref load
     */
    bool isAdjustable(QString group, QString opt, bool adjust = true); /**<
     * Returns ```true``` if the option defined by its group and name is adjustable, ```false``` elsewhere.
     *
     * \see \ref setAdjustable
     */

    void setAdjustable(QString group, QString opt, bool adjust); /**<
     * Sets if the option defined by its group and name is adjustable.
     *
     * \see \ref isAdjustable
     */


    void reinitialise(QString group = ""); /**<
     * Clear all options from the group. If ```group == ""```, all entries are deleted.
     */
    static Options &options(); /**<
     * Returns the unique Options instance.
     */

private:
    QSettings a;
    QMap<QString, QMap<QString, QPair<QVariant, bool>>> defaultValues;
    Options();
    ~Options();
    static Options *instance;
};





#endif // OPTIONS_H
