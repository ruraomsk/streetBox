#include "readcsvfile.h"

ReadCSVFile::ReadCSVFile(Project *project,QWidget *parent) : QDialog(parent)
{

    this->project=project;
    QFileDialog dialog(this,"Укажите имя файла для чтения",ini.getString("import/path"));
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setNameFilters(ini.getString("import/ext").split(" "));
    if(dialog.exec()!=QDialog::Accepted){
        return;
    }
    QStringList nameFiles=dialog.selectedFiles();
    if (nameFiles.size()!=1) {
        QMessageBox msg;
        msg.setText("Указано для чтения более одного файла!");
        msg.exec();
        return;
    }
    QFile file(nameFiles[0]);
    if (!file.open(QIODevice::ReadOnly|QIODevice::Text)){
        QMessageBox msg;
        msg.setText("Ошибка чтения файла"+file.fileName());
        msg.exec();
        return;
    }
    QTextStream stream(&file);
    while(!stream.atEnd()){
        lines.append(stream.readLine());
    }
    file.close();
    fileLoaded=true;
    loadData();

}

void ReadCSVFile::loadData()
{

    lname=new QLineEdit("",this);
    ldesc=new QLineEdit("",this);
    lregion=new QLineEdit(QString::number(project->Region),this);
    larea=new QLineEdit(QString::number(project->Area),this);
    lsubarea=new QLineEdit(QString::number(project->SubArea),this);
    lstep=new QLineEdit(QString::number(project->Step),this);
    lchanel=new QLineEdit(QString::number(project->Chanels),this);
    rowStart=ini.getInt("import/rowstart");
    colDate=ini.getInt("import/coldate");
    colTime=ini.getInt("import/coltime");
    colData=ini.getInt("import/coldata");
    columns=countColumns(rowStart);
    QStringList formsDate={"dd-MM-yyyy","dd MM yyyy","dd/MM/yyyy","yyyy-MM-dd","yyyy/MM/dd","yyyy MM dd"};
    QStringList formsTime={"h:m","h m","h/m","hh:mm","hh mm","hh/mm"};

    fdate=new QComboBox();
    fdate->addItems(formsDate);
    connect(fdate,SIGNAL(currentIndexChanged(const QString &)),this,SLOT(redrawFormatDate(const QString &)));

    ftime=new QComboBox();
    ftime->addItems(formsTime);
    connect(ftime,SIGNAL(currentIndexChanged(const QString &)),this,SLOT(redrawFormatTime(const QString &)));

    lsimbol=new QLineEdit(simbol);
    connect(lsimbol,SIGNAL(editingFinished()),this,SLOT(getChanges()));

    lrow=new QSpinBox();
    lrow->setValue(rowStart);
    lrow->setRange(0,lines.size());
    connect(lrow,SIGNAL(valueChanged(int )),this,SLOT(getSpinBoxChanges(int )));

    lrows=new QSpinBox();
    lrows->setRange(10,lines.size());
    lrows->setSingleStep(10);
    lrows->setValue(rows);
    connect(lrows,SIGNAL(valueChanged(int )),this,SLOT(getSpinBoxChanges(int )));

    ldate=new QSpinBox;
    ldate->setValue(colDate);
    ldate->setRange(0,columns);
    connect(ldate,SIGNAL(valueChanged(int )),this,SLOT(getSpinBoxChanges(int )));

    ldata=new QSpinBox;
    ldata->setValue(colData);
    ldata->setRange(0,columns);
    connect(ldata,SIGNAL(valueChanged(int )),this,SLOT(getSpinBoxChanges(int )));

    ltime=new QSpinBox;
    ltime->setRange(0,columns);
    ltime->setValue(colTime);
    connect(ltime,SIGNAL(valueChanged(int )),this,SLOT(getSpinBoxChanges(int )));

    lcolumns=new QLabel((QString::number(columns)));

    ltext=new QTextEdit;
    makeText();
    QGridLayout *grid=new QGridLayout(this);
    grid->setAlignment(Qt::AlignTop);
    QWidget *top=new QWidget;
    QWidget *cross=new QWidget;

    QFormLayout *hbox=new QFormLayout();
    hbox->addRow("Номер перекрестка",lname);
    hbox->addRow("Описание перекрестка",ldesc);
    hbox->addRow("Регион",lregion);
    hbox->addRow("Район",larea);
    hbox->addRow("Подрайон",lsubarea);
    hbox->addRow("Интервал заполнения",lstep);
    hbox->addRow("Ко-во каналов",lchanel);
    cross->setLayout(hbox);
    QFormLayout *form=new QFormLayout;
    form->addRow("Символ разделитель колонок",lsimbol);
    form->addRow("Получилось колонок",lcolumns);
    form->addRow("С какой строки начинаются данные",lrow);
    form->addRow("Номер колонки с датой",ldate);
    form->addRow("Формат даты",fdate);
    form->addRow("Номер колонки со временем",ltime);
    form->addRow("Формат времени",ftime);
    form->addRow("Номер колонки с которой начинаются данные",ldata);
    //    form->addRow("Cколько строк выводить в образец",lrows);
    top->setLayout(form);
    //    QWidget *showFile=new QWidget;
    //    QVBoxLayout *vbox=new QVBoxLayout;
    //    vbox->addWidget(ltext);
    wtable=new QTableWidget;
    makeTable();
    grid->addWidget(cross,0,1);
    grid->addWidget(top,1,1);
    grid->addWidget(ltext,2,1);
    grid->addWidget(wtable,0,2,4,1);
    QWidget *button=new QWidget;
    QHBoxLayout *bbox=new QHBoxLayout();
    QPushButton *okBtn=new QPushButton("Создать");
    connect(okBtn,SIGNAL(clicked()),this,SLOT(createCross()));
    bbox->addWidget(okBtn);
    QPushButton *cancelBtn=new QPushButton("Отменить");
    connect(cancelBtn,SIGNAL(clicked()),this,SLOT(rejectCross()));
    bbox->addWidget(cancelBtn);
    button->setLayout(bbox);
    grid->addWidget(button,3,1);

    setLayout(grid);
}

void ReadCSVFile::redraw()
{
    columns=countColumns(rowStart);
    lcolumns->setText(QString::number(columns));
    lsimbol->setText(simbol);
    lrow->setValue(rowStart);
    lrows->setValue(rows);
    ldate->setValue(colDate);
    ldata->setValue(colData);
    ltime->setValue(colTime);
    lcolumns->setText(QString::number(columns));
    ini.setInt("import/rowstart",rowStart);
    ini.setInt("import/coldate",colDate);
    ini.setInt("import/coltime",colTime);
    ini.setInt("import/coldata",colData);
    makeText();
    makeTable();
}

void ReadCSVFile::redrawFormatDate(const QString &name)
{
    formatDate=name;
    redraw();
}

void ReadCSVFile::redrawFormatTime(const QString &name)
{
    formatTime=name;
    redraw();
}

void ReadCSVFile::getChanges()
{
//    qDebug()<<"getChanged";
    simbol=lsimbol->text();
    redraw();
}

void ReadCSVFile::getSpinBoxChanges(int value)
{
//    qDebug()<<"getSpinBoxChanges";
    rowStart=lrow->value();
    rows=lrows->value();
    colDate=ldate->value();
    colData=ldata->value();
    colTime=ltime->value();
    redraw();
}

void ReadCSVFile::createCross()
{
//    qDebug()<<"createCross";
    if (lname->text().size()==0){
        Support::ErrorMessage("Укажите имя");
        return;
    }
    foreach (auto var, project->crosses) {
        if (var->Number==lname->text().toInt()) {
            Support::ErrorMessage("Такое имя уже используется");
            return;
        }
    }
    if (ldesc->text().size()==0){
        Support::ErrorMessage("Укажите описание перекрестка");
        return;
    }
    if(!Support::isVerStep(lstep->text().toInt())){
        Support::ErrorMessage("Интервал заполнения не верный");
        return;
    }
    if(lchanel->text().toInt()>16 ||lchanel->text().toInt()<1 ){
        Support::ErrorMessage("Не правильное число каналов");
        return;
    }
    Cross *cross=new Cross(lname->text().toInt(),ldesc->text());
    cross->Region=lregion->text().toInt();
    cross->Area=larea->text().toInt();
    cross->SubArea=lsubarea->text().toInt();
    cross->Step=lstep->text().toInt();
    cross->Chanels=lchanel->text().toInt();
    //Собираем все даты
    QMap<QDate,QDate> tdate;
    foreach (auto var, tableMatrix) {
        tdate[QDate::fromString(var[0],formatDate)]=QDate::fromString(var[0],formatDate);
    }
    foreach (auto date, tdate) {
        QList<QVector<int>> oneday;
        //Создаем матрицу значений на этот день
        int time=0;
        while(time<24*60){
            QVector<int> data(cross->Chanels+1);
            data[0]=time;
            time+=cross->Step;
            oneday.append(data);
        }
        //Заполняем ее значениями из матрицы
        for (int i = 0; i < oneday.size(); ++i) {
            QVector<int> t=oneday[i];
            int hour=t[0]/60;
            hour=hour==24?0:hour;
            int min=t[0]%60;
            QTime ttime=QTime(hour,min);
            foreach (auto line, tableMatrix) {
                if (date!=QDate::fromString(line[0],formatDate)) continue;
                if (ttime!=QTime::fromString(line[1],formatTime)) continue;
                for (int j = 0; j < (line.size()-2)&&j<cross->Chanels; ++j) {
                    t[j+1]=line[j+2].toInt();
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
        cross->setDataFromMatrix(date,oneday);
    }
    project->appendCross(cross);
    emit accept();
}

void ReadCSVFile::rejectCross()
{
    emit reject();
}

int ReadCSVFile::countColumns(int row)
{
    int count=0;
    for (int i=row; i<lines.size();++i){
        count=lines[i].split(simbol).size()>count?lines[i].split(simbol).size():count;
    }
    return count;
}

void ReadCSVFile::makeText()
{
    QString text;
    for (int i = rowStart; i < lines.size(); ++i) {
        text.append(lines[i]+"\n");
    }
    ltext->setText(text);
}

void ReadCSVFile::makeTable()
{
    tableMatrix=makeMatrix();
    wtable->setColumnCount(2+(columns-colData));
    wtable->setRowCount(0);
    QStringList tableHeaders;
    tableHeaders.append("Дата");
    tableHeaders.append("Время");
    for (int var = 0; var < (columns-colData); ++var) {
        tableHeaders.append(QString::number(var+1));
    }
    wtable->setHorizontalHeaderLabels(tableHeaders);
    wtable->setSelectionMode(QAbstractItemView::SingleSelection);
    wtable->setSelectionBehavior(QAbstractItemView::SelectRows);
    int row=0;
    foreach (auto var, tableMatrix) {
        wtable->insertRow(row);
        QDate tdate=QDate::fromString(var[0],formatDate);
        QTime ttime=QTime::fromString(var[1],formatTime);

        wtable->setItem(row,0,new QTableWidgetItem(tdate.toString(formatDate)));
        wtable->setItem(row,1,new QTableWidgetItem(ttime.toString(formatTime)));
        for (int j = 0; j < (columns-colData); ++j) {
            wtable->setItem(row,j+2,new QTableWidgetItem(var[2+j]));
        }
        row++;
    }
    wtable->resizeColumnsToContents();
    wtable->setMinimumSize(ini.getSize("table/size"));

}

QList<QVector<QString> > ReadCSVFile::makeMatrix()
{
    QList<QVector<QString> > result;
    for (int i = rowStart; i < lines.size(); ++i) {
        auto vals=lines[i].split(simbol);
        if (vals.size()!=columns) continue;
        QVector<QString> vrow;
        vrow.append(vals[colDate]);
        vrow.append(vals[colTime]);
        for (int col = colData; col < columns; ++col) {
            vrow.append(vals[col]);
        }
        result.append(vrow);
    }
    return result;
}
