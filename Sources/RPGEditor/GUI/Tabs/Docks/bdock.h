#ifndef BDOCKS_H
#define BDOCKS_H

#include <QtWidgets>
#include "intertie.h"

class BDock : public QFrame
{
    Q_OBJECT
    Q_PROPERTY(bool unfold READ unfold WRITE setUnfold)
    Q_PROPERTY(int currentSize READ currentSize WRITE setCurrentSize)
public:
    explicit BDock(QString title, QWidget *dock, QWidget *parent = 0);
    bool unfold() const;
    void setUnfold(bool v);
    int currentSize() const;
    void setCurrentSize(int t);

signals:

public slots:

private slots:
    void mouseDoubleClickEvent(QMouseEvent *me);
    void paintEvent(QPaintEvent *pe);
    void resizeEvent(QResizeEvent *re);

private:
    int wHeight;
    Intertie zoneSize;
    QWidget *dock;
    QLabel *name;
    QVBoxLayout *lay;
    QHBoxLayout *button;
    QScrollArea *container;
    QToolButton *bUnfold, *bLinked;
    BinaryStateMachine dLinked, dUnfold;
};

#endif // BDOCKS_H
