#include "newgame.h"

NewGame::NewGame(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
}


void NewGame::on_browse_clicked(){
    QString f(QFileDialog::getExistingDirectory(this, "Create in", QDir::homePath()));
    if(f != "") folderP->setText(f);
}


QString NewGame::name() const{
    return gName->text();
}
QString NewGame::folder() const{
    return folderP->text();
}
bool NewGame::createFolder() const{
    return newFolder->isChecked();
}


/* TODO
 *
 * -> complétion du chemin
 * -> vérification du chemin/nom
 *
 */
