#ifndef QUIETWIDGETS_H
#define QUIETWIDGETS_H

#include <QComboBox>


class QuietComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit QuietComboBox(QWidget *parent = nullptr);
signals:
    void userChangedCurrentIndex(int);
public slots:
    void setModel(QAbstractItemModel *model);
    void setCurrentIndex(int index);
private slots:
    void signalFilter(int index);
private:
    bool waiting;
};


#endif // QUIETWIDGETS_H
