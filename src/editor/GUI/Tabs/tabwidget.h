#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QWidget>

class TabWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TabWidget(QWidget *parent = 0);
    int index() const;
    void setIndex(int i);
    virtual void updateGame(){}

signals:

public slots:

private:
    int ind;

};

#endif // TABWIDGET_H
