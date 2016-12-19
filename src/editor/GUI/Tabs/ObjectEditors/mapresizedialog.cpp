#include "mapresizedialog.h"

MapResizeDialog::MapResizeDialog(int w, int h, QWidget *parent) :
    QDialog(parent), wi(w), he(h)
{
    setupUi(this);
    buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Resize"));
    oWidth->setValue(wi);
    oHeight->setValue(he);
    nWidth->setValue(wi);
    nHeight->setValue(he);
}


void MapResizeDialog::on_nHeight_valueChanged(int i){
    nHe = i;
    updateSize();
}

void MapResizeDialog::on_nWidth_valueChanged(int i){
    nWi = i;
    updateSize();
}

void MapResizeDialog::updateSize(){
    nbCells->setText(QString::number(nHe*nWi)+tr(" cell(s)", "", nHe*nWi));

}
