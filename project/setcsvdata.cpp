#include "setcsvdata.h"


SetCSVData::SetCSVData(Cross *cross, QWidget *parent):QDialog(parent)
{
    this->cross=cross;
    QFileDialog dialog(this,"Укажите имя файла для чтения",ini.getString("csv/path"));
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setNameFilters(ini.getString("csv/ext").split(" "));
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

void SetCSVData::loadData()
{
    rowStart=ini.getInt("csv/rowstart");
    colDate=ini.getInt("csv/coldate");
    colTime=ini.getInt("csv/coltime");
    colData=ini.getInt("csv/coldata");
    columns=countColumns(rowStart);
    QStringList formsDate={"yyyy-MM-dd","dd.MM.yyyy","dd MM yyyy","dd/MM/yyyy","dd-MM-yyyy","yyyy/MM/dd","yyyy MM dd"};

    fdate=new QComboBox();
    fdate->addItems(formsDate);
    connect(fdate,SIGNAL(currentIndexChanged(const QString &)),this,SLOT(redrawFormatDate(const QString &)));


    lsimbol=new QLineEdit(simbol);
    connect(lsimbol,SIGNAL(editingFinished()),this,SLOT(getChanges()));

    lchanel=new QSpinBox();
    lchanel->setValue(1);
    lchanel->setRange(1,cross->Chanels);
    connect(lchanel,SIGNAL(valueChanged(int )),this,SLOT(getSpinBoxChanges(int )));

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
    ldate->setRange(0,columns-1);
    connect(ldate,SIGNAL(valueChanged(int )),this,SLOT(getSpinBoxChanges(int )));

    ldata=new QSpinBox;
    ldata->setValue(colData);
    ldata->setRange(0,columns-1);
    connect(ldata,SIGNAL(valueChanged(int )),this,SLOT(getSpinBoxChanges(int )));

    ltime=new QSpinBox;
    ltime->setRange(0,columns-2);
    ltime->setValue(colTime);
    connect(ltime,SIGNAL(valueChanged(int )),this,SLOT(getSpinBoxChanges(int )));


    ltext=new QTextEdit;
    makeText();
    QGridLayout *grid=new QGridLayout(this);
    grid->setAlignment(Qt::AlignTop);
    QWidget *top=new QWidget;
    QWidget *cross=new QWidget;

    QFormLayout *hbox=new QFormLayout();
    hbox->addRow("Номер канала в проекте",lchanel);
    cross->setLayout(hbox);
    QFormLayout *form=new QFormLayout;
    form->addRow("Символ разделитель колонок",lsimbol);
    form->addRow("С какой строки начинаются данные",lrow);
    form->addRow("Номер колонки с датой",ldate);
    form->addRow("Формат даты",fdate);
    form->addRow("Номер колонки со временем",ltime);
    form->addRow("Номер колонки с которой начинаются данные",ldata);

    top->setLayout(form);
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

void SetCSVData::redraw()
{
    lsimbol->setText(simbol);
    lrow->setValue(rowStart);
    lrows->setValue(rows);
    lchanel->setValue(chanel);
    int tc=countColumns(rowStart);
    if (columns!=tc){
        //Меняем и проверяем все границы
        colData=colData>tc?tc:colData;
        colDate=colDate>tc?tc:colDate;
        colTime=colTime>tc?tc:colTime;
        ldate->setRange(0,tc-1);
        ldata->setRange(0,tc-1);
        ltime->setRange(0,tc-2);
    }
    columns=tc;
    ldate->setValue(colDate);
    ldata->setValue(colData);
    ltime->setValue(colTime);
    ini.setInt("csv/rowstart",rowStart);
    ini.setInt("csv/coldate",colDate);
    ini.setInt("csv/coltime",colTime);
    ini.setInt("csv/coldata",colData);
    makeText();
    makeTable();
}

void SetCSVData::redrawFormatDate(const QString &name)
{
    formatDate=name;
    redraw();
}

void SetCSVData::getChanges()
{
//    qDebug()<<"getChanged";
    simbol=lsimbol->text();
    redraw();
}

void SetCSVData::getSpinBoxChanges(int value)
{
    rowStart=lrow->value();
    rows=lrows->value();
    colDate=ldate->value();
    colData=ldata->value();
    colTime=ltime->value();
    chanel=lchanel->value();
    redraw();
}

void SetCSVData::createCross()
{
    this->setCursor(Qt::WaitCursor);
//    qDebug()<<"createCross";
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
            QVector<int> data(2);
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
                if (ttime!=QTime::fromString(line[1],"h:m")) continue;
                t[1]=line[2].toInt();
                oneday[i]=t;
                break;
            }
        }
        //Пытаемся подавить нули
//        QVector<int> last(2);
//        for (int i = 0; i < oneday.size(); ++i) {
//            QVector<int> t=oneday[i];
//                t[1]=t[1]==0?last[1]:t[1];
//                last[1]=t[1];
//            oneday[i]=t;
//        }
        //Если нет такой даты в cross то создадим пустую матрицу с полным набором каналов
        //И дополним ее своими данными
        QList<QVector<int>> matrix;
        if (!cross->archs.contains(date)){
            //Создаем матрицу значений на этот день
            int time=0;
            while(time<24*60){
                QVector<int> data(cross->Chanels+1);
                data[0]=time;
                time+=cross->Step;
                matrix.append(data);
            }
            cross->setDataFromMatrix(date,matrix);
        } else {
            matrix=cross->archs[date].ToMatrix();
            //Дополним колонками если надо
            for (int i = 0; i < matrix.size(); ++i) {
                    while(matrix[i].size()<cross->Chanels+1){
                        matrix[i].append(0);
                    }
            }
        }
        for (int i = 0; i < matrix.size(); ++i) {
            for (int j = 0; j < oneday.size(); ++j) {
                if (matrix[i][0]!=oneday[j][0]) continue;
//                qDebug()<<matrix[i][0]<<matrix[i].size();
                matrix[i][chanel]=oneday[j][1];
                break;
            }
        }
        cross->archs[date].FromMatrix(matrix);
    }
    this->setCursor(Qt::ArrowCursor);
    emit accept();
}

void SetCSVData::rejectCross()
{
    emit reject();

}

int SetCSVData::countColumns(int row)
{
    int count=0;
    for (int i=row; i<lines.size();++i){
        count=lines[i].split(simbol).size()>count?lines[i].split(simbol).size():count;
    }
    return count;
}

void SetCSVData::makeText()
{
    QString text;
    for (int i = rowStart; i < lines.size(); ++i) {
        text.append(lines[i]+"\n");
    }
    ltext->setText(text);
}

void SetCSVData::makeTable()
{
    tableMatrix=makeMatrix();
    wtable->setColumnCount(3);
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
        QTime ttime=QTime::fromString(var[1],"h:m");

        wtable->setItem(row,0,new QTableWidgetItem(tdate.toString(formatDate)));
        wtable->setItem(row,1,new QTableWidgetItem(ttime.toString("h:m")));
            wtable->setItem(row,2,new QTableWidgetItem(var[2]));
        row++;
    }
    wtable->resizeColumnsToContents();
    wtable->setMinimumSize(ini.getSize("table/size"));
}

QList<QVector<QString> > SetCSVData::makeMatrix()
{
    QList<QVector<QString> > result;
    for (int i = rowStart; i < lines.size(); ++i) {
        auto vals=lines[i].split(simbol);
        if (vals.size()!=columns) continue;
        QVector<QString> vrow;
        vrow.append(vals[colDate]);
        vrow.append(vals[colTime]+":"+vals[colTime+1]);
        vrow.append(vals[colData]);
        result.append(vrow);
    }
    return result;
}
