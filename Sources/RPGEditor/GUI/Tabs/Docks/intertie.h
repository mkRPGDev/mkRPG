#ifndef INTERTIE_H
#define INTERTIE_H

#include <QObject>
#include <QTimer>
#include <QStateMachine>


#define Min(a,b) ((a)<(b) ? (a) : (b))
#define Max(a,b) ((a)>(b) ? (a) : (b))
#define MinMax(a,x,b) Min(Max(a,x),b)


/*!
 * \brief The Intertie class provide int that move
 * smoothly from their value to an objective.
 */
class Intertie : public QObject
{
    Q_OBJECT
public:
    explicit Intertie(QObject *parent = 0);
    int value() const;
    void link(QObject *obj, const char *prop);


signals:
    void modificationFinished(int);
    void valueChanged(int);

public slots:
    void setValue(int v, bool inert = true);
    void setMaximumSpeed(int vM);
    void setAcceleration(int a);
    void setUpdateInterval(int d);

private slots:
    void updateValue();

private:
    inline void updateLinkedProperties() const;
    int objective;
    int currentValue;
    int speed;
    int acceleration;
    int maximumSpeed;
    QTimer *ti;
    QList<QPair<QObject*, const char*>> linkedProperties;
    bool running;
};


/*!
 * \brief The BinaryStateMachine class is a simple QStateMachine
 * with two states.
 */
class BinaryStateMachine : public QStateMachine
{
    Q_OBJECT

public:
    explicit BinaryStateMachine(QObject *parent = 0);
    void defineProperty(QObject* obj, const char* prop);
    void defineProperty(QObject* obj, const char* prop, QVariant yesValue, QVariant noValue);
    bool isPositive() const;
    bool isNegative() const;

signals:
    void swapped(bool);
    void __swap();

public slots:
    void swap(); // Pas de SIGNAL(echange(bool))
    void define(bool v); // SIGNAL(echange(bool))

private:
    QState *yes, *no;

};


#endif // INTERTIE_H
