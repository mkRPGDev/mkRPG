#include "celldock.h"






CellDock::CellDock(QWidget *parent) :
    BDockWidget(parent), map(nullptr), typesModel(nullptr)
{
    setupUi(this);

    paramModel = new ParamTreeItemModel(this);
    flagsModel = new FlagTreeItemModel(this);
    params->setModel(paramModel);
    flags->setModel(flagsModel);
}

void CellDock::updateGame(){
    if(game==nullptr){
        map = nullptr;
        return;
    }
    Map *newMap = game->currentMap();
    if(newMap != map){ // TODO vérifier si il y a eu des modifications
        //cellTypes->setModel(new ObjectsTreeModel(&game->world().types().cellType(), this));
        if(typesModel) delete typesModel;
        typesModel = new CellTypeListModel(game->world().types().cellType(), this);
        cellTypes->setModel(typesModel);
        map = newMap;
        selectionChanged();
    }
}

void CellDock::selectionChanged(){
    int sel = 0;
    Cell *lastSelCell = nullptr;
    for(int i(0); i<map->width(); ++i)
        for(int j(0); j<map->height(); ++j)
            if(map->cell(i,j).isSelected()){
                sel += 1;
                lastSelCell = &map->cell(i,j);
            }
    paramModel->setObject(lastSelCell);
    flagsModel->setObject(lastSelCell);
    params->expandView();
    flags->expandView();
    emit changeDockName(tr("Cell (")+QString::number(sel) + tr(" selected)", "the number or selected cells", sel));
}

void CellDock::on_cellTypes_userChangedCurrentIndex(int k){
    if(map == nullptr) return;
    CellType &ct(typesModel->cellTypeAt(k));
    for(int i(0); i<map->width(); ++i)
        for(int j(0); j<map->height(); ++j)
            if(map->cell(i,j).isSelected())
                map->cell(i,j).setCellType(ct);
    map->touch(); // temporaire
    emit gameModified();
}
