#include "paramtreeitemmodel.h"

ParamTreeItemModel::ParamTreeItemModel(QObject *parent) :
    QAbstractItemModel(parent),
    obj(nullptr)
{

}



int ParamTreeItemModel::rowCount(const QModelIndex &parent) const{
    if(obj){
        if(type){
            if(parent.isValid()){
                if(parent.parent().isValid()){
                    if(parent.parent().parent().isValid())
                        return 0;
                    return 2;
                }
                return type->properParams().length();
            }
            return type->paramTree().length();
        }
        else{
            if(parent.isValid()){
                if(parent.parent().isValid())
                    return 0;
                return 2;
            }
            return obj->params().length();
        }
    }
    return 0;
}

int ParamTreeItemModel::columnCount(const QModelIndex &parent) const{
    return 2;
}



QVariant ParamTreeItemModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return QVariant(section == 0 ? tr("Parameter") : tr("Value"));
    return QVariant();
}

QVariant ParamTreeItemModel::data(const QModelIndex &index, int role) const{
    return QVariant("bjk");
}

QModelIndex ParamTreeItemModel::index(int row, int column, const QModelIndex &parent) const{
    if(parent.isValid())
        return createIndex(row, column, parent.internalId()+1);
    return createIndex(row, column, 10*(type ? ancestor(type, row)->ident() : obj->ident()));
}

QModelIndex ParamTreeItemModel::parent(const QModelIndex &child) const{
    if(!child.isValid() || !(child.internalId()%10)) return QModelIndex();
    if(type){
        if(child.internalId()%10 == 1){
            int i(0);
            QString tn(game->object(child.internalId()/10)->typeName());
            HierarchicalAttr a(type->paramTree());
            for(auto it(a.begin()); it != a.end(); ++i)
                if((it++)->first == tn) it = a.end();
            return i < a.length() ? createIndex(i, 0, child.internalId() -1) : QModelIndex();
        }
        //else static_cast<GameObjectType*>(game->object(child.internalId()/10)->properParams().indexOf()
    }
    else{

    }
    return QModelIndex();
}


void ParamTreeItemModel::setObject(GameObject *o){
    beginResetModel();
    obj = o;
    type =  dynamic_cast<GameObjectType*>(o);
    endResetModel();
}


GameObjectType *ParamTreeItemModel::ancestor(GameObjectType *obj, int &gen) const{
    int req = gen;
    GameObjectType* anc = obj->ancestor();
    if(anc){
        anc = ancestor(anc, gen);
        ++gen;
        if(anc) return anc;
        else return req==gen ? obj : nullptr;
    }
    else{
        gen = 0;
        return req ? nullptr : obj;
    }

}
