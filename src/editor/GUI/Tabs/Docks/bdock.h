#ifndef BDOCKS_H
#define BDOCKS_H

#include <QtWidgets>
#include "intertie.h"
#include "bdockwidget.h"


class BDock : public QFrame
{
    Q_OBJECT
    Q_PROPERTY(bool unfold READ unfold WRITE setUnfold)
    Q_PROPERTY(int currentSize READ currentSize WRITE setCurrentSize)
public:
    explicit BDock(QString title, BDockWidget *dock, QWidget *parent = 0);
    bool unfold() const;
    int currentSize() const;
    void setCurrentSize(int t);
    void setIndex(int i);
    inline int index() const{return ind;}
    void moveTo(int i, bool inert = true);
    void setLength(int l);

signals:
    void mouseClick(int i, const QPoint &p);
    void mouseMove(int i, const QPoint &p);
    void mouseRelease(int i, const QPoint &p);
    void movementFinished(int i);

public slots:
    void setTitle(QString s);
    void setUnfold(bool v);

private slots:
    void mouseDoubleClickEvent(QMouseEvent *me);
    void mousePressEvent(QMouseEvent *me);
    void mouseMoveEvent(QMouseEvent *me);
    void mouseReleaseEvent(QMouseEvent *me);
    void paintEvent(QPaintEvent *pe);
    void resizeEvent(QResizeEvent *re);
    void setPosition(int i);
    void endOfMovement();

private:
    int ind;
    int wHeight;
    Intertie zoneSize;
    Intertie position;
    BDockWidget *dock;
    QLabel *name;
    QVBoxLayout *lay;
    QHBoxLayout *button;
    QScrollArea *container;
    QToolButton *bUnfold, *bLinked;
    BinaryStateMachine dLinked, dUnfold;
};

#endif // BDOCKS_H
