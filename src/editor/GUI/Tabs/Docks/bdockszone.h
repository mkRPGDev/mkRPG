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
    explicit BLayout(QWidget *parent = 0);
    void setOrientation(Qt::Orientation o);
    void insert(BDock *d, int ind = -1);
    void setSpacing(int e);
    void setLength(int t);
    int spacing() const;

signals:
    void sizeChanged(int);
    void showPoint(int, int);

private slots:
    void adjust();
    void dockClick(int i, const QPoint &p);
    void dockMove(int k, const QPoint &p);
    void dockRelease(int i, const QPoint &p);
    void dockStopped(int i);

private:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *re);
    void mouseDoubleClickEvent(QMouseEvent *me);

    int len, size;
    int space;

    int clPos;
    int initPos;
    bool repainting;
    //QBoxLayout *lay;
    QPoint prev;
    Qt::Orientation orient;
    BDock *movingDock;
    QVector<BDock*> docks;
    QVector<QPair<int,int>> placement;
};







class BDocksZone : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int length READ length WRITE setLength)
    Q_PROPERTY(int currentLength READ currentLength WRITE setCurrentLenght)

    /*!
     * \brief The ScrollBarMode enum describe the way
     * the BDocksZone reacts when a scroll bar is needed
     */
    enum ScrollBarMode{
        AlwaysVisible,  /**< Always show the scroll bar, even if it is usless*/
        Adjustable,     /**< Show the scroll bar when needed, adaptating the docks length*/
        Fixed           /**< Show the scroll bar when needed, keeping the docks length fixed*/
    };

public:
    explicit BDocksZone(QWidget *parent = 0);
    void setUnfold(bool u, bool anim = true);
    const BinaryStateMachine* states() const;
    int length() const;
    void setLength(int t);
    inline ScrollBarMode scrollBarMode() const{return scrm;}
    void setScrollBarMode(ScrollBarMode m);

    int currentLength() const;

    void addDock(QString title, BDockWidget *dock); // TODO temporaire

signals:

public slots:
    void swap(bool anim = true);

protected slots:
    void setCurrentLenght(int t);

private slots:
    void showPoint(int x, int y);
    void foldingChanged(bool f);
    void docksSizeChanged();
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *me);
    void mouseMoveEvent(QMouseEvent *me);
    void resizeEvent(QResizeEvent *event);
    void updateDockLength();
    void updateMousePos();

private:
    void mouseDoubleClickEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *me);
    void setDockLength(int l, bool inert = true);

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
    ScrollBarMode scrm;


    static const int BUTTON = 12;
};

#endif // BDOCKSZONE_H
