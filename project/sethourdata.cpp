#include "sethourdata.h"

#include <QDateTimeEdit>


SetHourData::SetHourData(Cross *cross, QWidget *parent):QDialog(parent)
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

void SetHourData::loadData()
{
    typedef void (QSpinBox::*QSpinBoxIntSignal)(int);
    rowStart=ini.getInt("csv/rowstart");
    colTime=ini.getInt("csv/coltime");
    colData=ini.getInt("csv/coldata");
    columns=countColumns(rowStart);
    QStringList formsDate={"yyyy-MM-dd","dd.MM.yyyy","dd MM yyyy","dd/MM/yyyy","dd-MM-yyyy","yyyy/MM/dd","yyyy MM dd"};



    lsimbol=new QLineEdit(simbol);
    connect(lsimbol,&QLineEdit::editingFinished,this,&SetHourData::getChanges);

    lchanel=new QSpinBox();
    lchanel->setValue(1);
    lchanel->setRange(1,cross->Chanels);
    connect(lchanel, static_cast<QSpinBoxIntSignal>(&QSpinBox::valueChanged),this, &SetHourData::getSpinBoxChanges);

    lrow=new QSpinBox();
    lrow->setValue(rowStart);
    lrow->setRange(0,lines.size());
    connect(lrow, static_cast<QSpinBoxIntSignal>(&QSpinBox::valueChanged),this, &SetHourData::getSpinBoxChanges);

    lrows=new QSpinBox();
    lrows->setRange(10,lines.size());
    lrows->setSingleStep(10);
    lrows->setValue(rows);
    connect(lrows, static_cast<QSpinBoxIntSignal>(&QSpinBox::valueChanged),this, &SetHourData::getSpinBoxChanges);
    QDate now(QDate::currentDate());

    ldate=new QDateTimeEdit(now);
    ldate->setCalendarPopup(true);
    ldate->setDisplayFormat("dd/MM/yyyy");

    ldata=new QSpinBox;
    ldata->setValue(colData);
    ldata->setRange(0,columns-1);
    connect(ldata, static_cast<QSpinBoxIntSignal>(&QSpinBox::valueChanged),this, &SetHourData::getSpinBoxChanges);

    ltime=new QSpinBox;
    ltime->setRange(0,columns-1);
    ltime->setValue(colTime);
    connect(ltime, static_cast<QSpinBoxIntSignal>(&QSpinBox::valueChanged),this, &SetHourData::getSpinBoxChanges);


    ltext=new QTextEdit;
    makeText();
    QGridLayout *grid=new QGridLayout(this);
    grid->setAlignment(Qt::AlignTop);
    QWidget *top=new QWidget;
    QWidget *cross=new QWidget;

    QFormLayout *hbox=new QFormLayout();
    hbox->addRow("Номер канала в проекте",lchanel);
    hbox->addRow("На какую дату вводим",ldate);
    cross->setLayout(hbox);
    QFormLayout *form=new QFormLayout;
    form->addRow("Символ разделитель колонок",lsimbol);
    form->addRow("С какой строки начинаются данные",lrow);
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
    connect(okBtn,&QPushButton::clicked,this,&SetHourData::createCross);
    bbox->addWidget(okBtn);
    QPushButton *cancelBtn=new QPushButton("Отменить");
    connect(cancelBtn,&QPushButton::clicked,this,&SetHourData::createCross);
    bbox->addWidget(cancelBtn);
    button->setLayout(bbox);
    grid->addWidget(button,3,1);

    setLayout(grid);

}

void SetHourData::redraw()
{
    lsimbol->setText(simbol);
    lrow->setValue(rowStart);
    lrows->setValue(rows);
    lchanel->setValue(chanel);
    int tc=countColumns(rowStart);
    if (columns!=tc){
        //Меняем и проверяем все границы
        colData=colData>tc?tc:colData;
        colTime=colTime>tc?tc:colTime;
        ldata->setRange(0,tc-1);
        ltime->setRange(0,tc-2);
    }
    columns=tc;
    ldata->setValue(colData);
    ltime->setValue(colTime);
    ini.setInt("csv/rowstart",rowStart);
    ini.setInt("csv/coltime",colTime);
    ini.setInt("csv/coldata",colData);
    makeText();
    makeTable();

}

void SetHourData::getChanges()
{
    simbol=lsimbol->text();
    redraw();
}

void SetHourData::getSpinBoxChanges(int value)
{
    rowStart=lrow->value();
    rows=lrows->value();
    colData=ldata->value();
    colTime=ltime->value();
    chanel=lchanel->value();
    redraw();
}

void SetHourData::createCross()
{
    //Собираем все даты
    QMap<QDate,QDate> tdate;
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
            if (ttime!=QTime::fromString(line[0],"h:m")) continue;
            t[1]=line[1].toInt();
            oneday[i]=t;
            break;
        }
    }
    //Пытаемся подавить нули
    QVector<int> last(2);
    for (int i = 0; i < oneday.size(); ++i) {
        QVector<int> t=oneday[i];
        t[1]=t[1]==0?last[1]:t[1];
        last[1]=t[1];
        oneday[i]=t;
    }
    //Если нет такой даты в cross то создадим пустую матрицу с полным набором каналов
    //И дополним ее своими данными
    QList<QVector<int>> matrix;
    if (!cross->archs.contains(ldate->date())){
        //Создаем матрицу значений на этот день
        int time=0;
        while(time<24*60){
            QVector<int> data(cross->Chanels+1);
            data[0]=time;
            time+=cross->Step;
            matrix.append(data);
        }
        cross->setDataFromMatrix(ldate->date(),matrix);
    } else {
        matrix=cross->archs[ldate->date()].ToMatrix();
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
    cross->archs[ldate->date()].FromMatrix(matrix);
    emit accept();

}

void SetHourData::rejectCross()
{
    emit reject();
}

int SetHourData::countColumns(int row)
{
    int count=0;
    for (int i=row; i<lines.size();++i){
        count=lines[i].split(simbol).size()>count?lines[i].split(simbol).size():count;
    }
    return count;

}

void SetHourData::makeText()
{
    QString text;
    for (int i = rowStart; i < lines.size(); ++i) {
        text.append(lines[i]+"\n");
    }
    ltext->setText(text);

}

void SetHourData::makeTable()
{
    tableMatrix=makeMatrix();
    wtable->setColumnCount(2);
    wtable->setRowCount(0);
    QStringList tableHeaders;
    tableHeaders.append("Время");
    tableHeaders.append("1");
    wtable->setHorizontalHeaderLabels(tableHeaders);
    wtable->setSelectionMode(QAbstractItemView::SingleSelection);
    wtable->setSelectionBehavior(QAbstractItemView::SelectRows);
    int row=0;
    foreach (auto var, tableMatrix) {
        wtable->insertRow(row);
        QTime ttime=QTime::fromString(var[0],"h:m");
        wtable->setItem(row,0,new QTableWidgetItem(ttime.toString("h:m")));
        wtable->setItem(row,1,new QTableWidgetItem(var[1]));
        row++;
    }
    wtable->resizeColumnsToContents();
    wtable->setMinimumSize(ini.getSize("table/size"));

}

QList<QVector<QString> > SetHourData::makeMatrix()
{
    QList<QVector<QString> > result;
    for (int i = rowStart; i < lines.size(); ++i) {
        auto vals=lines[i].split(simbol);
        if (vals.size()!=columns) continue;
        QVector<QString> vrow;
        if(colTime<vals.size()) vrow.append(vals[colTime]+":00"); else continue;
        if(colData<vals.size())vrow.append(vals[colData]); else continue;
        result.append(vrow);
    }
    return result;
}
