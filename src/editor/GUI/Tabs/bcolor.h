#ifndef BCOLOR_H
#define BCOLOR_H

#include <QtWidgets>

class BColor : public QFrame
{
    Q_OBJECT
public:
    explicit BColor(QWidget *parent = 0);
    const QString &name() const;
    const QColor &color() const;

signals:
    void colorChanged(const QColor&);

public slots:
    void setColor(const QColor &c);
    void setName(const QString &s);

private:
    void mousePressEvent(QMouseEvent *me);
    QColor coul;
    QString n;
};

#endif // BCOLOR_H
