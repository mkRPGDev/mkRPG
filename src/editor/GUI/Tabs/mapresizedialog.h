#ifndef MAPRESIZEDIALOG_H
#define MAPRESIZEDIALOG_H

#include "ui_mapresizedialog.h"
#include <QPushButton>

class MapResizeDialog : public QDialog, private Ui::MapResizeDialog
{
    Q_OBJECT

public:
    explicit MapResizeDialog(int w, int h, QWidget *parent = 0);


private slots:
    void on_nWidth_valueChanged(int i);
    void on_nHeight_valueChanged(int i);

private:
    void updateSize();
    int wi,he;
    int nWi,nHe;
};

#endif // MAPRESIZEDIALOG_H
