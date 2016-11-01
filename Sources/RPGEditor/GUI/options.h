#ifndef OPTIONS_H
#define OPTIONS_H

#include <QSettings>
#include <QDir>
#include <QSize>
#include <QPoint>
#include <QColor>




const QString ADAPT("Adaptatif");
const QString VAL("Valeur");

#define WIN "Window"
#define DIR "Directories"
#define MAP "MapsTab"

/*!
 * \brief Classe singleton encapsulant la gestion des options permanentes.
 *
 * Ajoute à QSettings quelques fonctionnalités :
 *
 * La possiblité de définir si les options s'adaptent lors de la modificatoin
 * ou non (par exemple, la taille de la fenêtre), voir \ref estAdaptatif, \ref defAdaptatif
 *
 * La gestion des gradients chargés, disponibles à tout moment et en tout lieu dans l'application.
 */
struct Options{
    template<class T>
    T load(QString group, QString opt){
        a.beginGroup(group);
        T val = a.value(opt+VAL,defaultValues[group][opt].first).template value<T>();
        a.endGroup();
        return val;
    }
    template<class T>
    void save(QString group, QString opt, T val){
        a.beginGroup(group);
        if(a.value(opt+ADAPT, defaultValues[group][opt].second).toBool())
            a.setValue(opt+VAL, val);
        a.endGroup();
    }/* // NOTE inutile ?
    template<class T>
    void defDefaut(QString group, QString opt, T val){
        a.beginGroup(group);
        a.setValue(opt+VAL,val);
        a.endGroup();
    }*/
    bool isAdaptaive(QString group, QString opt, bool adapt = true);
    void setAdaptaive(QString group, QString opt, bool adapt);


    void reinitialise();
    static Options &options();

private:
    QSettings a;
    QMap<QString, QMap<QString, QPair<QVariant, bool>>> defaultValues;
    Options();
    ~Options();
    static Options *instance;
};





#endif // OPTIONS_H
