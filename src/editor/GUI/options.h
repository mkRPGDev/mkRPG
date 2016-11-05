#ifndef OPTIONS_H
#define OPTIONS_H

#include <QSettings>
#include <QDir>
#include <QSize>
#include <QPoint>
#include <QColor>

/*!
 * \file options.h
 * \brief This file defines the Options class, and the constants
 * that are used in this class.
 *
 * The headers of types which are used in the appoication must be include here.
 * See Options for details.
 *
 * \author Baptiste Pauget
 */

// TODO vérifier que les noms existent !

const QString ADAPT("Adaptatif");
const QString VAL("Value");

#define WIN "Window" /**<
 * Group of window related options.
 */
#define DIR "Directories" /**<
    * Group of paths options.
    */
#define MAP "MapsEditor" /**<
    * Group of MapsEditor related options.
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
 * - The adaptative ones : the value of the option change when \ref save is called.
 * - The unadaptatives ones : the value of the option doesn't change is \ref save is called,
 * the option must be modified with \setDefault.
 *
 * The sort of option can be set with the \ref setAdaptative function.
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
 * To read or write options, the Options instance must be retreived, thanks to \ref options, then
 * the \ref load and \ref save functions can be called.
 *
 *
 * ## Adding options
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
    }
    /**<
     * Reads an option defined by its group and name.
     *
     * \note The template argument must be precised
     * since it can't be deduced from arguments' types.
     *
     * \warning If the option type and the reading type mismatch, an default null value is returned.
     *
     * \see save
     */
    template<class T>
    void save(QString group, QString opt, T val){
        a.beginGroup(group);
        if(a.value(opt+ADAPT, defaultValues[group][opt].second).toBool())
            a.setValue(opt+VAL, val);
        a.endGroup();
    }
    /**<
     * Writes the new value of the options defined by its group and name, if the option is adaptative.
     * See Options for details about options types.
     *
     * \note The template argument can be omitted since it would be deduced from the value argument.
     *
     * \see setDefault, load
     */
    template<class T>
    void setDefault(QString group, QString opt, T val){
        a.beginGroup(group);
        a.setValue(opt+VAL,val);
        a.endGroup();
    }
    /**<
     * Writes the new value of the options defined by its group and name, whatever the option type is.
     * See Options for details about options types.
     *
     * \note The template argument can be omitted since it would be deduced from the value argument.
     *
     * \see save, load
     */
    bool isAdaptaive(QString group, QString opt, bool adapt = true);

    void setAdaptaive(QString group, QString opt, bool adapt);



    void reinitialise(QString group = "");
    /**<
     *
     */
    static Options &options();
    /**<
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
