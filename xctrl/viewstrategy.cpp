#include "viewstrategy.h"

ViewStrategy::ViewStrategy(Project *project,Xctrl *xctrl,QWidget *parent) : QWidget(parent)
{
    this->project=project;
    this->xctrl=xctrl;
    QVBoxLayout* mainLayout = new QVBoxLayout;
    setLayout( mainLayout );

    m_view = new QTableView;
    //    m_view->horizontalHeader().setResetResizeMode( QHeaderView::Stretch );
    m_view->setModel( stable = new StrategyTable(this->project,this->xctrl,this) );
    connect(stable,SIGNAL(updated()),this,SLOT(updateTable()));
    m_view->resizeColumnsToContents();
    mainLayout->addWidget( m_view );
    QHBoxLayout *panelLayout = new QHBoxLayout;
    mainLayout->addLayout( panelLayout );
    QPushButton *bnRemove = new QPushButton( "Удалить"  );
    connect( bnRemove, SIGNAL( clicked() ), this, SLOT( removeSelected() ) );
    panelLayout->addWidget( bnRemove);
    QPushButton *bnAdd = new QPushButton( "Добавить"  );
    connect( bnAdd, SIGNAL( clicked() ), stable, SLOT( addRecord() ) );
    panelLayout->addWidget( bnAdd);
//    show();
}

void ViewStrategy::removeSelected()
{
    QModelIndex index=m_view->selectionModel()->currentIndex();
    stable->removeSelected(index);
    project->isChanged=true;
    emit updated();
}

void ViewStrategy::updateTable()
{
    emit updated();
    return;
}

StrategyTable::StrategyTable(Project *project, Xctrl *xctrl, ViewStrategy *parent)
{
    this->project=project;
    this->xctrl=xctrl;
    this->parent=parent;

}

int StrategyTable::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return xctrl->Strategys.size();
}

int StrategyTable::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 8;
}

QVariant StrategyTable::data(const QModelIndex &index, int role) const
{
    if (
            !index.isValid() ||
            xctrl->Strategys.size() <= index.row() ||
            ( role != Qt::DisplayRole && role != Qt::EditRole )
            )
        return QVariant();
    Strategy s=xctrl->Strategys[index.row()];
    switch( index.column() ) {
    case 0: return  s.L;
    case 1: return s.R;
    case 2: return s.PlanL;
    case 3: return s.PlanM;
    case 4: return s.PlanR;
    case 5: return s.Fl;
    case 6: return s.Fr;
    case 7: return s.Description;
    }
    return QVariant();
}

bool StrategyTable::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if( !index.isValid() || role != Qt::EditRole || xctrl->Strategys.size() <= index.row() ) {
        return false;
    }
    project->isChanged=true;
    switch (index.column()) {
    case 0:
        xctrl->Strategys[index.row()].L=value.toInt();
        emit updated();
        break;
    case 1:
        xctrl->Strategys[index.row()].R=value.toInt();
        emit updated();
        break;
    case 2:
        xctrl->Strategys[index.row()].PlanL=value.toInt();
        emit updated();
        break;
    case 3:
        xctrl->Strategys[index.row()].PlanM=value.toInt();
        emit updated();
        break;
    case 4:
        xctrl->Strategys[index.row()].PlanR=value.toInt();
        emit updated();
        break;
    case 5:
        xctrl->Strategys[index.row()].Fl=value.toFloat();
        emit updated();
        break;
    case 6:
        xctrl->Strategys[index.row()].Fr=value.toFloat();
        emit updated();
        break;
    case 7:
        xctrl->Strategys[index.row()].Description=value.toString();
        emit updated();
        break;
    default:
        return false;

    }
    return false;
}

QVariant StrategyTable::headerData(int section, Qt::Orientation orientation, int role) const
{
    if( role != Qt::DisplayRole ) {
        return QVariant();
    }
    if( orientation == Qt::Vertical ) {
        return section;
    }
    switch( section ) {
    case 0:
        return  "Прямой";
    case 1:
        return "Обратный";
    case 2:
        return "КС П";
    case 3:
        return "КС С";
    case 4:
        return "КС О";
    case 5:
        return "Луч П";
    case 6:
        return "Луч О";
    case 7:
        return "Описание";
    }
    return QVariant();
}

Qt::ItemFlags StrategyTable::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractTableModel::flags( index );
    if( index.isValid() ) {
        flags |= Qt::ItemIsEditable;
    }
    return flags;
}

void StrategyTable::removeSelected(const QModelIndex &index)
{
    if(index.isValid()){
        beginRemoveRows(QModelIndex(),index.row(),index.row());
        xctrl->Strategys.removeAt(index.row());
        endRemoveRows();
        project->isChanged=true;
        emit updated();
    }
}

void StrategyTable::addRecord()
{
    Strategy s(0,0,0,0,0,1.0,1.0,"");
    int row=xctrl->Strategys.size();
    beginInsertRows(QModelIndex(),row,row);
    xctrl->Strategys.append(s);
    endInsertRows();
    project->isChanged=true;
    emit updated();
}

