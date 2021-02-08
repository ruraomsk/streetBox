#include "viewareal.h"

ViewAreal::ViewAreal(Project *project, Xctrl *xctrl, QWidget *parent): QWidget(parent)
{
    this->project=project;
    this->xctrl=xctrl;
    grid=new QGridLayout;
    QVBoxLayout* mainLayout = new QVBoxLayout;
//    setLayout( mainLayout );

    m_view = new QTableView;
    //    m_view->horizontalHeader().setResetResizeMode( QHeaderView::Stretch );
    m_view->setModel( stable = new ArealTable(this->project,this->xctrl,this) );
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
    grid->addLayout(mainLayout,1,1);
    vor=new Voronoi(qMax(xctrl->Left,xctrl->Right),qMax(xctrl->Left,xctrl->Right));
    foreach(auto ar,xctrl->Areals){
        vor->addAreal(ar);
    }
    vor->makeAreals();
    voroni=new ViewVoronoi(vor);
    grid->addWidget(voroni->text,2,2);
    grid->addWidget(voroni,0,2,2,2);

    setLayout( grid );
//    show();
}

void ViewAreal::removeSelected()
{
    QModelIndex index=m_view->selectionModel()->currentIndex();
    stable->removeSelected(index);
    project->isChanged=true;
    emit updated();
}

void ViewAreal::updateTable()
{
    redraw();
    return;
}

void ViewAreal::redraw()
{
    auto temp=vor->sprayes;
    auto tempn=vor->nameSprayes;
    delete vor;
    delete voroni->text;
    delete voroni;
    vor=new Voronoi(qMax(xctrl->Left,xctrl->Right),qMax(xctrl->Left,xctrl->Right));
    foreach(auto ar,xctrl->Areals){
        vor->addAreal(ar);
    }
    vor->makeAreals();
    vor->sprayes=temp;
    vor->nameSprayes=tempn;
    voroni=new ViewVoronoi(vor);

    grid->addWidget(voroni->text,2,2);
    grid->addWidget(voroni,0,2,2,2);

}

ArealTable::ArealTable(Project *project, Xctrl *xctrl, ViewAreal *parent)
{
    this->project=project;
    this->xctrl=xctrl;
    this->parent=parent;

}

int ArealTable::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return xctrl->Areals.size();
}

int ArealTable::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 4;
}

QVariant ArealTable::data(const QModelIndex &index, int role) const
{
    if (
            !index.isValid() ||
            xctrl->Areals.size() <= index.row() ||
            ( role != Qt::DisplayRole && role != Qt::EditRole )
            )
        return QVariant();
    Areal s=xctrl->Areals[index.row()];
    switch( index.column() ) {
    case 0: return  s.L;
    case 1: return s.R;
    case 2: return s.Plan;
    case 3: return s.Description;
    }
    return QVariant();
}

bool ArealTable::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if( !index.isValid() || role != Qt::EditRole || xctrl->Areals.size() <= index.row() ) {
        return false;
    }
    project->isChanged=true;
    switch (index.column()) {
    case 0:
        xctrl->Areals[index.row()].L=value.toInt();
        emit updated();
        break;
    case 1:
        xctrl->Areals[index.row()].R=value.toInt();
        emit updated();
        break;
    case 2:
        xctrl->Areals[index.row()].Plan=value.toInt();
        emit updated();
        break;
    case 3:
        xctrl->Areals[index.row()].Description=value.toString();
        emit updated();
        break;
    default:
        return false;

    }
    return false;
}

QVariant ArealTable::headerData(int section, Qt::Orientation orientation, int role) const
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
        return "План ";
    case 3:
        return "Описание";
    }
    return QVariant();
}

Qt::ItemFlags ArealTable::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractTableModel::flags( index );
    if( index.isValid() ) {
        flags |= Qt::ItemIsEditable;
    }
    return flags;
}

void ArealTable::removeSelected(const QModelIndex &index)
{
    if(index.isValid()){
        beginRemoveRows(QModelIndex(),index.row(),index.row());
        xctrl->Areals.removeAt(index.row());
        endRemoveRows();
        project->isChanged=true;
        emit updated();
    }
}

void ArealTable::addRecord()
{
    Areal s(0,0,0,"");
    int row=xctrl->Areals.size();
    beginInsertRows(QModelIndex(),row,row);
    xctrl->Areals.append(s);
    endInsertRows();
    project->isChanged=true;
    emit updated();
}


