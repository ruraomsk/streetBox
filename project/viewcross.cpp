#include "viewcross.h"

ViewCross::ViewCross(Project *project,Cross *cross,QWidget *parent) : QWidget(parent)
{
    this->project=project;
    this->cross=cross;
    wtable=new QTableWidget();
    whead=new QWidget();
    head();
    grid=new QGridLayout(this);
    grid->setAlignment(Qt::AlignTop);
    grid->addWidget(whead,0,1);
    setLayout(grid);
    table();
    wgraph=new QScrollArea;
    graph();
    this->show();
}



void ViewCross::Changed(const QString &name)
{
    if(name.size()==0){
        lcomment->setText("");
        cross->now=QDate::fromJulianDay(0);
    } else {
        lcomment->setText(cross->archs[QDate::fromString(name,"dd.MM.yyyy")].Comment);
        cross->now=QDate::fromString(name,"dd.MM.yyyy");
    };
    project->isChanged=true;
    table();
    graph();
    update();
}

void ViewCross::changeComment()
{
    cross->archs[QDate::fromString(combo->currentText(),"dd.MM.yyyy")].Comment=lcomment->text();
    if(!Support::isVerStep(lstep->text().toInt())){
        Support::ErrorMessage("Интервал заполнения не верный");
        return;
    }
    cross->Step=lstep->text().toInt();
    if(lchanels->text().toInt()<1||lchanels->text().toInt()>16){
        Support::ErrorMessage("Неверное кол-во каналов");
        return;
    }
    cross->Chanels=lchanels->text().toInt();
    project->isChanged=true;
}

void ViewCross::saveTable(int row, int column)
{
    if(cross->summ) return;
    tableMatrix[row][column]=wtable->item(row,column)->text().toInt();
    cross->archs[QDate::fromString(combo->currentText(),"dd.MM.yyyy")].FromMatrix(tableMatrix);
    project->isChanged=true;
}

void ViewCross::itemCkliked()
{
    int len=checked.size();
    checked.clear();
    for (int i = 0; i < len; ++i) {
        checked.append(false);
    }
    foreach (auto it, wtable->selectedItems()) {
        checked[it->column()]=true;
    }
    //    qDebug()<<checked;
    project->isChanged=true;
    graph();
    update();
}

void ViewCross::summChanged()
{
    cross->summ=lsumm->isChecked();
    table();
    graph();
    project->isChanged=true;
    update();
}

void ViewCross::deleteDate()
{
    if (combo->count()<2){
        Support::ErrorMessage("Тогда удалите весь перекресток!");
        return;
    }
    cross->archs.remove(QDate::fromString(combo->currentText(),"dd.MM.yyyy"));
    project->isChanged=true;
    Update();
}

void ViewCross::changeDate()
{

}

void ViewCross::ChangedInterval(const QString &name)
{
    if(name.size()==0){
        cross->SummInreval=60;
    } else {
        cross->SummInreval=name.toInt();
    };
    table();
    graph();
    project->isChanged=true;
    update();
}

void ViewCross::repairData()
{
    auto savearch=cross->archs;
    cross->archs.clear();
    foreach (auto arch, savearch) {
        tableMatrix=arch.ToMatrix();
        QList<QVector<int>> oneday;
        //Создаем матрицу значений на этот день
        int time=0;
        while(time<24*60){
            QVector<int> data(cross->Chanels+1);
            data[0]=time;
            time+=cross->Step;
            oneday.append(data);
        }
        //Переносим данные из матрицы
        for (int i = 0; i < oneday.size(); ++i) {
            QVector<int> t=oneday[i];
            foreach (auto line, tableMatrix) {
                if (line[0]!=t[0]) continue;
                for (int j = 0; j < (line.size()-1)&&j<cross->Chanels; ++j) {
                    t[j+1]=line[j+1];
                }
                oneday[i]=t;
                break;
            }
        }
        //Пытаемся подавить нули
        QVector<int> last(cross->Chanels+1);
        for (int i = 0; i < oneday.size(); ++i) {
            QVector<int> t=oneday[i];
            for (int i = 1; i < last.size(); ++i) {
                t[i]=t[i]==0?last[i]:t[i];
                last[i]=t[i];
            }
            oneday[i]=t;
        }
        cross->setDataFromMatrix(arch.Date,oneday);
    }
    table();
    graph();
    project->isChanged=true;
    update();
    //    Support::Message("Расчет закончен");
}

QDate ViewCross::getCurrentDate()
{
    return QDate::fromString(combo->currentText());
}

void ViewCross::Update()
{
    combo->clear();
    foreach (auto var, cross->archs) {
        if (var.Date.toString().size()!=0) combo->addItem(var.Date.toString("dd.MM.yyyy"));
    }
    if(combo->currentText().size()==0){
        lcomment->setText("");
    } else {
        lcomment->setText(cross->archs[QDate::fromString(combo->currentText())].Comment);
        cross->now=QDate::fromString(combo->currentText(),"dd.MM.yyyy");
    };
    table();
    graph();
    project->isChanged=true;
    update();
}


void ViewCross::head()
{
    QFormLayout *fbox=new QFormLayout;
//    ldecrib=new QLineEdit(cross->Description);
//    connect(ldecrib,SIGNAL(editingFinished()),this,SLOT(changeComment()));
    bnDelete=new QPushButton("Удалить");
    connect(bnDelete,SIGNAL(clicked()),this,SLOT(deleteDate()));
    fbox->addWidget(bnDelete);
    lstep=new QLineEdit(QString::number(cross->Step));
    connect(lstep,SIGNAL(editingFinished()),this,SLOT(changeComment()));
    lchanels=new QLineEdit(QString::number(cross->Chanels));
    connect(lchanels,SIGNAL(editingFinished()),this,SLOT(changeComment()));
//    fbox->addRow("описание",ldecrib);
    fbox->addRow("Интервал измерений",lstep);
    fbox->addRow("Кол-во каналов",lchanels);

    combo=new QComboBox();
//    combo->setEditable(false);
    connect(combo,SIGNAL(textActivated(const QString&)),this,SLOT(Changed(const QString&)));
    foreach (auto comt, cross->archs) {
        combo->addItem(comt.Date.toString("dd.MM.yyyy"));
    }
    if(combo->currentText().size()==0){
        lcomment=new QLineEdit("");
        cross->now=QDate::fromJulianDay(0);
    } else {
        lcomment=new QLineEdit(cross->archs[QDate::fromString(combo->currentText(),"dd.MM.yyyy")].Comment);
        cross->now=QDate::fromString(combo->currentText(),"dd.MM.yyyy");
    };
    connect(lcomment,SIGNAL(editingFinished()),this,SLOT(changeComment()));
    fbox->addRow("Метка данных",lcomment);
    fbox->addRow("Дата измерений",combo);
    lsumm=new QCheckBox("Суммировать");
    lsumm->setChecked(cross->summ);
    connect(lsumm,SIGNAL(stateChanged(int)),this,SLOT(summChanged(int)));
    linterval=new QComboBox;
    linterval->addItems(Support::getElite(cross->Step));
    if(linterval->count()==0){
        lsumm->setDisabled(true);
    } else {
        lsumm->setDisabled(false);
    }
    connect(linterval,SIGNAL(textActivated(const QString&)),this,SLOT(ChangedInterval(const QString&)));
    cross->SummInreval=linterval->currentText().toInt();
    fbox->addRow(lsumm,linterval);
    whead->setLayout(fbox);
    whead->setMaximumSize(500,300);
}

void ViewCross::table()
{
    delete wtable;
    wtable=new QTableWidget;
    QDate date=QDate::fromString(combo->currentText(),"dd.MM.yyyy");
    tableMatrix=cross->archs[date].ToMatrix();
    if (cross->summ){
        //Установлено суммирование начинаем приводить матрицу
        tableMatrix=Support::Summator(tableMatrix,cross->Step,cross->SummInreval);
    }

    int len=0;
    foreach (auto var, tableMatrix) {
        len=var.size()>len?var.size():len;
    }
    if (len==0){
        //        Support::ErrorMessage("Отсуcтвуют данные по перекрестку");
        return;
    }
    wtable->setColumnCount(len);
    if(len!=checked.size()){
        checked.clear();
        for (int i = 0; i < len; ++i) {
            checked.append(false);
        }
    }
    QTableWidgetItem *t=new QTableWidgetItem("Время");
    t->setCheckState(Qt::Unchecked);
    wtable->setHorizontalHeaderItem(0,t);
    wtable->setSelectionBehavior(QAbstractItemView::SelectColumns);
    wtable->setSelectionMode(QAbstractItemView::MultiSelection);
    //    qDebug()<<checked;
    for (int var = 1; var < len; ++var) {
        QTableWidgetItem *t=new QTableWidgetItem(QString::number(var));
        //        t->setSelected(checked[var]);
        wtable->setHorizontalHeaderItem(var,t);
    }
    wtable->setMaximumSize(ini.getSize("table/size"));
    for (int row = 0; row < wtable->rowCount(); ++row) {
        wtable->removeRow(row);
    }
    int row=0;
    foreach (auto var, tableMatrix) {
        wtable->insertRow(row);
        QString time=QString::asprintf("%02d:%02d",var[0]/60,var[0]%60);
        wtable->setItem(row,0,new QTableWidgetItem(time));
        for (int j = 1; j < len && j<var.size(); ++j) {
            wtable->setItem(row,j,new QTableWidgetItem(QString::number(var[j])));
        }
        row++;
    }
    wtable->resizeColumnsToContents();
    connect(wtable,SIGNAL(cellChanged(int ,int )),this,SLOT(saveTable(int ,int )));
    connect(wtable,SIGNAL(itemSelectionChanged()),this,SLOT(itemCkliked()));
    grid->addWidget(wtable,1,1);

}

void ViewCross::graph()
{
//    qDebug()<<"graph"<<checked<<checked.size();

    delete wgraph;
    QWidget *local=new QWidget;
    wgraph=new QScrollArea;
    QVBoxLayout *vbox=new QVBoxLayout;
    for (int index = 0; index < combo->count(); ++index) {
        if(combo->itemText(index).size()==0) continue;
        //        qDebug()<<combo->itemText(index);
        QDate date=QDate::fromString(combo->itemText(index),"dd.MM.yyyy");
        QList<QVector<int> > tabMatrix=cross->archs[date].ToMatrix();
        if (cross->summ){
            //Установлено суммирование начинаем приводить матрицу
            tabMatrix=Support::Summator(tabMatrix,cross->Step,cross->SummInreval);
        }
        QChart *chart=new QChart();
        QDateTimeAxis *axisX=new QDateTimeAxis;
        axisX->setTickCount(25);
        axisX->setFormat("hh:mm");
        axisX->setTitleText("Время");
        QValueAxis *axisY=new QValueAxis;
        axisY->setLabelFormat("%i");
        axisY->setTitleText("Значения");
        chart->addAxis(axisX,Qt::AlignBottom);
        chart->addAxis(axisY,Qt::AlignLeft);
        int max=0;
        for (int column = 1; column < checked.size(); ++column) {
            if (!checked[column]) continue;
            QLineSeries *series=new QLineSeries;
            foreach(auto m,tabMatrix){
                int hour=m[0]/60;
                hour=hour==24?0:hour;
                int min=m[0]%60;
                QTime time(hour,min);
                QDateTime dt(date,time);
                series->append(dt.toMSecsSinceEpoch(),m[column]);

                max=max<m[column]?m[column]:max;
            }
            QTime time(0,0);
            QDateTime dt(date.addDays(1),time);
            axisX->setMax(dt);
            QDateTime dtm(date,time);
            axisX->setMin(dtm);

            series->setName(QString::number(column));
            series->setPointLabelsVisible(true);
            series->setPointLabelsFormat("@yPoint");
            chart->addSeries(series);
            axisY->setMax(qreal(max+(0.1*max)));
            series->attachAxis(axisX);
            series->attachAxis(axisY);
        }
        chart->legend()->hide();
        chart->setTitle(combo->itemText(index)+":"+cross->archs[date].Comment);
//        chart->createDefaultAxes();
        QChartView *viewchart=new QChartView(chart);
        viewchart->setRenderHint(QPainter::Antialiasing);
        viewchart->setMinimumSize(ini.getSize("chart/sizesmall"));
        vbox->addWidget(viewchart);
    }
    local->setLayout(vbox);
    wgraph->setWidget(local);
    wgraph->setWindowTitle("Графики");
    wgraph->setMinimumSize(ini.getSize("chart/sizehuge"));
    grid->addWidget(wgraph,0,2,3,3);
}
