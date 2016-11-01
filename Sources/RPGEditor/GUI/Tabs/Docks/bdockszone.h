#ifndef BDOCKSZONE_H
#define BDOCKSZONE_H

#include <QtWidgets>
#include "intertie.h"
#include "bdock.h"

#include "GUI/options.h"


class BLayout : public QWidget
{
    Q_OBJECT

public:
    explicit BLayout(Qt::Orientation o, QWidget *parent = 0);
    void insert(BDock *d, int ind = -1);
    void setSpacing(int e);
    void setLength(int t);
    int spacing() const;

private slots:
    void adjust();

private:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *re);
    void mouseDoubleClickEvent(QMouseEvent *);

    int len;
    int space;
    //QBoxLayout *lay;
    Qt::Orientation orient;
    QList<BDock*> docks;
};







class BDocksZone : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int length READ length WRITE setLength)
    Q_PROPERTY(int currentLength READ currentLength WRITE setCurrentLenght)

public:
    explicit BDocksZone(QWidget *parent = 0);
    void setUnfold(bool u, bool anim = true);
    const BinaryStateMachine* states() const;
    int length() const;
    void setLength(int t);

    virtual int currentLength() const = 0;

    void addDock(QString title, QWidget *dock); // TODO temporaire

signals:

public slots:
    void swap(bool anim = true);

protected slots:
    virtual void setCurrentLenght(int t) = 0;

private slots:
    void foldingChanged(bool f);

protected:
    void mouseDoubleClickEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *me);
    void baseLength(int pos);
    void newLength(int pos);

    bool resizing;
    int lMin, lMax;
    int base, lBase;
    int lId;
    Intertie *inLength;
    QGridLayout *lay;
    BinaryStateMachine unfoldStates;
    QToolButton *bUnfold;
    QScrollArea *dockArea;
    BLayout *docks;


    static const int BUTTON = 12;
};

#endif // BDOCKSZONE_H
