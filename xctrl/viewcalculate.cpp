#include "viewcalculate.h"

#include <QFormLayout>

ViewCalculate::ViewCalculate(    Project *project,Xctrl *xctrl,QWidget *parent) : QWidget(parent)
{
    this->project=project;
    this->xctrl=xctrl;
    group=new QGroupBox("Способ выбора значений");
    gdate=new QRadioButton("По дате");
    gcomment=new QRadioButton("По заметкам");
    gdefault=new QRadioButton("По умолчанию");
    gdate->setChecked(true);
    QVBoxLayout *vbox=new QVBoxLayout;
    vbox->addWidget(gdate);
    vbox->addWidget(gcomment);
    vbox->addWidget(gdefault);
    vbox->addStretch(1);
    group->setLayout(vbox);

    gstyle=new QGroupBox("Способ расчета");
    garea=new QRadioButton("Центры областей");
    gbox=new QRadioButton("Лучи");
    if (xctrl->UseStrategy){
        gbox->setChecked(true);
    } else {
        garea->setChecked(true);
    }

    QVBoxLayout *vb=new QVBoxLayout;
    vb->addWidget(garea);
    vb->addWidget(gbox);
    vb->addStretch(1);
    gstyle->setLayout(vb);

    grid=new QGridLayout;
    grid->addWidget(group,0,0);
    grid->addWidget(gstyle,1,0);

    cdates=new QComboBox;
    cdates->addItems(getAllDates());
    ccomments=new QComboBox;
    ccomments->addItems(getAllComments());
    QFormLayout *fbox=new QFormLayout;
    fbox->addRow("Выберите дату",cdates);
    fbox->addRow("Выберите заметку",ccomments);
    grid->addLayout(fbox,2,0);
    QHBoxLayout *hbox=new QHBoxLayout;
    btnLoad=new QPushButton("Загрузить данные");
    connect(btnLoad,SIGNAL(clicked()),this,SLOT(loadData()));
    hbox->addWidget(btnLoad);
    btnCalculate=new QPushButton("Расчитать");
    connect(btnCalculate,SIGNAL(clicked()),this,SLOT(calculateData()));
    hbox->addWidget(btnCalculate);
    btnPush=new QPushButton("Брызнуть");
    connect(btnPush,SIGNAL(clicked()),this,SLOT(pushSpray()));
    hbox->addWidget(btnPush);
    grid->addLayout(hbox,3,0);
    ltext=new QTextEdit("Протокол");
    ltext->setMinimumSize(ini.getSize("protocol/size"));
    grid->addWidget(ltext,4,0);
    emptyTable();
    table();
    setLayout(grid);
    show();
}

QVector<Point> ViewCalculate::getSprays()
{
    QVector<Point> result;
    foreach (auto var, calcData->fin) {
        Point p(var[2].toInt(),var[1].toInt());
        result.append(p);
    }
    return result;
}

QVector<QString> ViewCalculate::getNames()
{
    QVector<QString> result;
    foreach (auto var, calcData->fin) {
        result.append(var[0]+" "+addString);
    }
    return result;

}

DataGraph ViewCalculate::getData()
{
    DataGraph r("Расчет "+QDateTime::currentDateTime().toString());
    r.datas.clear();
    foreach (auto var, calcData->fin) {
        QVector<int> line;
        auto ls=var[0].split(":");
        line.append(ls[0].toInt()*60+ls[1].toInt());
        line.append(var[1].toInt());
        line.append(var[2].toInt());
        r.datas.append(line);
    }
    return r;

}

void ViewCalculate::loadData()
{
    if (gdefault->isChecked()){
        addString="";
        calcData=new XTCalculate(project,xctrl);
    }
    if (gdate->isChecked()){
        addString=cdates->currentText();
        calcData=new XTCalculate(project,xctrl,QDate::fromString(cdates->currentText(),"dd.MM.yyyy"));
    }
    if (gcomment->isChecked()){
        addString=ccomments->currentText();
        calcData=new XTCalculate(project,xctrl,ccomments->currentText());
    }
    QString text;
    foreach (auto line, calcData->protocol) {
        text.append(line+"\n");
    }
    ltext->setText(text);
    update();
}

void ViewCalculate::calculateData()
{
    if (gbox->isChecked()){
        xctrl->UseStrategy=true;
    } else {
        xctrl->UseStrategy=false;
    }

    if (gbox->isChecked())    calcData->calculate();
    if (garea->isChecked())    calcData->calcAreal();

    QString text;
    foreach (auto line, calcData->protocol) {
        text.append(line+"\n");
    }
    ltext->setText(text);
    delete wtable;
    wtable=new QTableWidget;
    QStringList list={"Время","Прямое","Обратное","ПК назначенный","Отношение"};
    wtable->setColumnCount(list.size());
    for (int i = 0; i < list.size(); ++i) {
        QTableWidgetItem *t=new QTableWidgetItem(list[i]);
        wtable->setHorizontalHeaderItem(i,t);
    }
    wtable->setMinimumSize(ini.getSize("table/size"));
    int row=0;
    foreach (auto var, calcData->fin) {
        wtable->insertRow(row);
        for (int j = 0; j < list.size() && j<var.size(); ++j) {
            wtable->setItem(row,j,new QTableWidgetItem((var[j])));
        }
        row++;
    }
    wtable->resizeColumnsToContents();
    grid->addWidget(wtable,0,1,5,5);
    update();
}

void ViewCalculate::pushSpray()
{
    emit newSpray();
}

QStringList ViewCalculate::getAllDates()
{
    QMap<QDate,bool> result;
    result.clear();
    QStringList ret;
    foreach (auto cl, xctrl->Calculates) {
        foreach (auto cr, project->crosses) {
            if(cr->Number!=cl.ID) continue;
            foreach (auto date, cr->archs.keys()) {
                result[date]=true;
            }
        }
    }
    foreach (auto date, result.keys()) {
        ret.append(date.toString("dd.MM.yyyy"));
    }
    return ret;
}

QStringList ViewCalculate::getAllComments()
{
    QMap<QString,bool> result;
    result.clear();
    QStringList ret;
    foreach (auto cl, xctrl->Calculates) {

        foreach (auto cr, project->crosses) {
            if(cr->Number!=cl.ID) continue;
            foreach (auto a, cr->archs) {
                if(a.Comment.size()!=0) result[a.Comment]=true;
            }
        }
    }
    foreach (auto str, result.keys()) {
        ret.append(str);
    }
    return ret;

}

void ViewCalculate::emptyTable()
{
    matrix.clear();
    int time=xctrl->Step;
    while (time<24*60){
        QVector<QString> v(10);
        v[0]=QString::asprintf("%02d:%02d",time/60,time%60);
        matrix.append(v);
        time+=xctrl->Step;
    }
}

void ViewCalculate::table()
{
    //    if(wtable!=nullptr) delete wtable;
    wtable=new QTableWidget;
    QStringList list={"Время","Колонка 1","Колонка 2","Колонка 3","Колонка 4","Колонка 5","Колонка 6"};
    wtable->setColumnCount(list.size());
    for (int i = 0; i < list.size(); ++i) {
        QTableWidgetItem *t=new QTableWidgetItem(list[i]);
        wtable->setHorizontalHeaderItem(i,t);
    }
    wtable->setMinimumSize(ini.getSize("table/size"));
    int row=0;
    foreach (auto var, matrix) {
        wtable->insertRow(row);
        for (int j = 0; j < list.size() && j<var.size(); ++j) {
            wtable->setItem(row,j,new QTableWidgetItem((var[j])));
        }
        row++;
    }
    wtable->resizeColumnsToContents();
    grid->addWidget(wtable,0,1,5,5);
}
