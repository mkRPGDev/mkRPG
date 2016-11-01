#include "intertie.h"

#include <QDebug>

Intertie::Intertie(QObject *parent) : QObject(parent)
{
    ti = new QTimer(this);
    ti->setInterval(20);
    connect(ti, SIGNAL(timeout()), this, SLOT(updateValue()));
    objective = 0;
    currentValue = 0;
    speed = 0;
    acceleration = 4;
    maximumSpeed = 40;
}

int Intertie::value() const{
    return currentValue;
}

void Intertie::link(QObject *obj, const char *prop){
    linkedProperties.append(QPair<QObject*,const char*>(obj, prop));
}

void Intertie::updateValue(){
    speed = MinMax(-maximumSpeed, speed + (objective < currentValue ? -acceleration : acceleration), maximumSpeed);
    int vi = Max(speed,-speed);
    currentValue += MinMax(-vi,objective-currentValue,vi);

    emit valueChanged(currentValue);
    updateLinkedProperties();
    if(currentValue == objective){
        speed = 0;
        ti->stop();
        emit modificationFinished(currentValue);
    }
}

void Intertie::setAcceleration(int a){
    acceleration = a;
}

void Intertie::setUpdateInterval(int d){
    ti->setInterval(d);
}

void Intertie::updateLinkedProperties() const{
    for(auto i : linkedProperties)
        i.first->setProperty(i.second, currentValue);
}

void Intertie::setValue(int v, bool inert){
    objective = v;
    if(inert){
        if(!ti->isActive())
            ti->start();
    }
    else{
        currentValue = v;
        emit valueChanged(currentValue);
        emit modificationFinished(currentValue);
        updateLinkedProperties();
    }
}

void Intertie::setMaximumSpeed(int vM){
    maximumSpeed = vM;
}





BinaryStateMachine::BinaryStateMachine(QObject *parent) :
    QStateMachine(parent)
{
    yes = new QState(this);
    no = new QState(this);
    yes->addTransition(this, SIGNAL(__swap()), no);
    no->addTransition(this, SIGNAL(__swap()), yes);

    setInitialState(yes);
    start();
}


void BinaryStateMachine::swap(){
    emit swapped(!configuration().contains(yes));
    emit __swap();
}

void BinaryStateMachine::setPositive(bool p){
    if(configuration().contains(yes) ^ p)
        emit __swap();
}

void BinaryStateMachine::setNegative(bool n){
    setPositive(!n);
}

void BinaryStateMachine::defineProperty(QObject* obj, const char* prop){
    yes->assignProperty(obj, prop, true);
    no->assignProperty(obj, prop, false);
}
void BinaryStateMachine::defineProperty(QObject* obj, const char* prop, QVariant yesValue, QVariant noValue){
    yes->assignProperty(obj, prop, yesValue);
    no->assignProperty(obj, prop, noValue);
}

bool BinaryStateMachine::isPositive() const{
    return configuration().contains(yes);
}

bool BinaryStateMachine::isNegative() const{
    return !isPositive();
}
