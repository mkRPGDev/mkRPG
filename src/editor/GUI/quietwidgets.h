#ifndef QUIETWIDGETS_H
#define QUIETWIDGETS_H

#include <QComboBox>



/*!
 * \file quietwidgets.h
 * \brief Definition of the editors that does not emit
 * signal when their value is changed by setting directly it.
 *
 * These classes are usefull to avoid event loops.
 */


/*!
 * \brief The QuietComboBox class is a QComboBox with a
 * \ref userChangedCurrentIndex signal.
 */
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
