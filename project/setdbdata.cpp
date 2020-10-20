#include "setdbdata.h"


SetDBData::SetDBData(Cross *cross, QWidget *parent): QDialog(parent)
{
    this->cross=cross;
    result=Support::listStatistics(cross->Region,cross->Area);
    if (result.size()==0){
        Support::ErrorMessage("Нет информации");
        return;
    }
    QVBoxLayout *vbox=new QVBoxLayout;
    combo=new QComboBox;
    foreach (auto id, result.keys()) {
        combo->addItem(QString::number(id));
    }
    connect(combo,SIGNAL(textActivated(const QString&)),this,SLOT(Changed(const QString&)));
    vbox->addWidget(combo);

    wtable=new QTableWidget;
    table();
    vbox->addWidget(wtable);

    QWidget *button=new QWidget;
    QHBoxLayout *bbox=new QHBoxLayout();
    QPushButton *okBtn=new QPushButton("Создать");
    connect(okBtn,SIGNAL(clicked()),this,SLOT(loadFromDb()));
    bbox->addWidget(okBtn);
    QPushButton *cancelBtn=new QPushButton("Отменить");
    connect(cancelBtn,SIGNAL(clicked()),this,SLOT(reject()));
    bbox->addWidget(cancelBtn);
    button->setLayout(bbox);
    vbox->addWidget(button);
    setLayout(vbox);
}

void SetDBData::Changed(const QString &sdate)
{
    table();
    wtable->update();
}

void SetDBData::loadFromDb()
{
    QList<QDate> res;
    int id=combo->currentText().toInt();
    foreach (auto check, checks) {
        if (check->isChecked()){
            res.append(QDate::fromString(check->text(),"dd.MM.yyyy"));
        }
    }
    foreach (auto date, res) {
        auto json=Support::getArchStat(cross->Region,cross->Area,id,date);
        ArchStat arch(json);
        arch.Area=cross->Area;
        arch.ID=id;
        arch.Region=cross->Region;
        arch.Date=date;
        cross->Attach(arch);
    }
    emit accept();
}

void SetDBData::table()
{
    wtable->setColumnCount(1);
    wtable->setRowCount(result[combo->currentText().toInt()].size());
    wtable->setHorizontalHeaderLabels({"Время"});
    checks.clear();
    int row=0;
    foreach (auto date, result[combo->currentText().toInt()]) {
//        qDebug()<<date;
        auto *ch=new QCheckBox(date.toString("dd.MM.yyyy"));
        wtable->setCellWidget(row++,0,ch);
        checks.append(ch);
    }
    wtable->resizeColumnsToContents();
}
