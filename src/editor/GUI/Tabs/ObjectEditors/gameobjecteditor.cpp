#include "gameobjecteditor.h"
#include "mapeditor.h"
#include "celltypeeditor.h"

GameObjectEditor::GameObjectEditor(QWidget *parent) : QWidget(parent)
{

}



#define SpecificEditor(Type) \
    Type *obj##Type = dynamic_cast<Type*>(&obj); if(obj##Type) return new Type##Editor(*obj##Type, parent)

GameObjectEditor *GameObjectEditor::editor(GameObject &obj, QWidget *parent){
    SpecificEditor(Map);
    SpecificEditor(CellType);

    return new GameObjectEditor(parent);
}
