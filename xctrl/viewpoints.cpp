#include "viewpoints.h"

ViewPoints::ViewPoints(Project *project,Xctrl *xctrl,QWidget *parent) : QWidget(parent)
{
    this->project=project;
    this->xctrl=xctrl;
    QVBoxLayout* mainLayout = new QVBoxLayout;
    setLayout( mainLayout );

    m_view = new QTableView;
    //    m_view->horizontalHeader().setResetResizeMode( QHeaderView::Stretch );
    m_view->setModel( ptable = new PointsTable(this->project,this->xctrl,this) );
    connect(ptable,SIGNAL(updated()),this,SLOT(updateTable()));
    m_view->resizeColumnsToContents();
    mainLayout->addWidget( m_view );
    QHBoxLayout *panelLayout = new QHBoxLayout;
    mainLayout->addLayout( panelLayout );
    QPushButton *bnRemove = new QPushButton( "Удалить"  );
    connect( bnRemove, SIGNAL( clicked() ), this, SLOT( removeSelected() ) );
    panelLayout->addWidget( bnRemove);
    QPushButton *bnAdd = new QPushButton( "Добавить"  );
    connect( bnAdd, SIGNAL( clicked() ), ptable, SLOT( addRecord() ) );
    panelLayout->addWidget( bnAdd);


    show();
}

void ViewPoints::removeSelected()
{
    QModelIndex index=m_view->selectionModel()->currentIndex();
    ptable->removeSelected(index);
    project->isChanged=true;
}

void ViewPoints::updateTable()
{
    emit updated();
}

PointsTable::PointsTable(Project *project,Xctrl *xctrl, ViewPoints *parent)
{
    this->project=project;
    this->xctrl=xctrl;
    this->parent=parent;
    foreach (auto calc, xctrl->Calculates) {
        bool found=false;
        foreach (auto val, project->crosses) {
            if(val->Number==calc.ID) {
                found=true;
                QString result="";
                foreach (auto var, calc.ChanR) {
                    if (var<1 || var>val->Chanels){
                        result+=QString::number(var)+" нет ";
                    }
                }
                foreach (auto var, calc.ChanL) {
                    if (var<1 || var>val->Chanels){
                        result+=QString::number(var)+" нет ";
                    }
                }
                errors.append(result);
                break;
            }
        }
        if(!found){
            errors.append("Нет такого перекрестка");
        }
    }
}

int PointsTable::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return xctrl->Calculates.size();
}

int PointsTable::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 6;
}

QVariant PointsTable::data(const QModelIndex &index, int role) const
{
    if (
            !index.isValid() ||
            xctrl->Calculates.size() <= index.row() ||
            ( role != Qt::DisplayRole && role != Qt::EditRole )
            )
        return QVariant();
    Calc c=xctrl->Calculates[index.row()];
    switch( index.column() ) {
    case 0:
        return  c.Region;
    case 1:
        return c.Area;
    case 2:
        return c.ID;
    case 3:{
        QString list;
        foreach (auto val, c.ChanL) {
            list+=" "+QString::number(val);
        }
        return list;
    }
    case 4:{
        QString list;
        foreach (auto val, c.ChanR) {
            list+=" "+QString::number(val);
        }
        return list;
    }
    case 5:{
        return errors[index.row()];
    }
    }

    return QVariant();
}

bool PointsTable::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if( !index.isValid() || role != Qt::EditRole || xctrl->Calculates.size() <= index.row() ) {
        return false;
    }
    project->isChanged=true;
    switch (index.column()) {
    case 0:
        xctrl->Calculates[index.row()].Region=value.toInt();
        emit updated();
        break;
    case 1:
        xctrl->Calculates[index.row()].Area=value.toInt();
        emit updated();
        break;
    case 2:
        xctrl->Calculates[index.row()].ID=value.toInt();
        emit updated();
        break;
    case 3:{
        QStringList listVariant=value.toString().split(" ");
        QList<int> list;
        foreach (auto val, listVariant) {
            list.append(val.toInt());
        }
        xctrl->Calculates[index.row()].ChanL=list;
        emit updated();
        break;
    }
    case 4:{
        QStringList listVariant=value.toString().split(" ");
        QList<int> list;
        foreach (auto val, listVariant) {
            list.append(val.toInt());
        }
        xctrl->Calculates[index.row()].ChanR=list;
        emit updated();
        break;
    }
    default:
        return false;

    }
    bool found=false;
    foreach (auto val, project->crosses) {
        if(val->Number==xctrl->Calculates[index.row()].ID) {
            found=true;
            QString result="";
            foreach (auto var, xctrl->Calculates[index.row()].ChanR) {
                if (var<1 || var>val->Chanels){
                    result+=QString::number(var)+" нет ";
                }
            }
            foreach (auto var, xctrl->Calculates[index.row()].ChanL) {
                if (var<1 || var>val->Chanels){
                    result+=QString::number(var)+" нет ";
                }
            }
            errors[index.row()]=result;
            break;
        }
    }
    if(!found){
        errors[index.row()]="Нет такого перекрестка";
    }
    return true;
}

QVariant PointsTable::headerData(int section, Qt::Orientation orientation, int role) const
{
    if( role != Qt::DisplayRole ) {
        return QVariant();
    }

    if( orientation == Qt::Vertical ) {
        return section;
    }

    switch( section ) {
    case 0:
        return  "Регион";
    case 1:
        return "Район";
    case 2:
        return "Перекресток";
    case 3:
        return "Номера каналов прямого";
    case 4:
        return "Номера каналов обратного";
    case 5:
        return "Примечания к данному перекрестку";
    }

    return QVariant();
}

Qt::ItemFlags PointsTable::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractTableModel::flags( index );
    if( index.isValid() && index.row()<4) {
        flags |= Qt::ItemIsEditable;
    }
    return flags;
}

void PointsTable::removeSelected(const QModelIndex& index)
{
    if(index.isValid()){
        beginRemoveRows(QModelIndex(),index.row(),index.row());
        xctrl->Calculates.removeAt(index.row());
        errors.removeAt(index.row());
        endRemoveRows();
        project->isChanged=true;
        emit updated();
    }
}

void PointsTable::addRecord()
{
    Calc c(xctrl->Region,xctrl->Area,0);
    int row=xctrl->Calculates.size();
    beginInsertRows(QModelIndex(),row,row);
    xctrl->Calculates.append(c);
    errors.append("");
    endInsertRows();
    project->isChanged=true;
    emit updated();
}

