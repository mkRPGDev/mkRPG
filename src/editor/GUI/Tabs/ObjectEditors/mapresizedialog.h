#ifndef MAPRESIZEDIALOG_H
#define MAPRESIZEDIALOG_H

#include "ui_mapresizedialog.h"
#include <QPushButton>



/*!
 * \file mapresizedialog.h
 * \brief Definition of the MapResizeDialog class
 *
 */


/*!
 * \brief The MapResizeDialog class provides a dialog window
 * used to define how the map should be resized.
 */
class MapResizeDialog : public QDialog, private Ui::MapResizeDialog
{
    Q_OBJECT

public:
    explicit MapResizeDialog(int w, int h, QWidget *parent = 0);
    QRect newRectangle() const;

private slots:
    void on_nWidth_valueChanged(int i);
    void on_nHeight_valueChanged(int i);

private:
    void updateSize();
    int wi,he;
    int nWi,nHe;
};

#endif // MAPRESIZEDIALOG_H
