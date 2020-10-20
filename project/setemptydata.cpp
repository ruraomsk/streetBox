#include "setemptydata.h"

SetEmptyData::SetEmptyData(Cross *cross, QWidget *parent): QDialog(parent)
{
    this->cross=cross;
    setWindowTitle("Укажите необходимые параметры");
    lrnd=new QCheckBox();
    QDate now(QDate::currentDate());
    ldate=new QDateTimeEdit(now);
    ldate->setCalendarPopup(true);
    ldate->setDisplayFormat("dd/MM/yyyy");
    QHBoxLayout *hbox=new QHBoxLayout();
    hbox->addWidget(ldate);
    hbox->addWidget(new QLabel("Случайные данные"));
    hbox->addWidget(lrnd);
    QPushButton *okBtn=new QPushButton("OK");
    connect(okBtn,SIGNAL(clicked()),this,SLOT(moveData()));
    hbox->addWidget(okBtn);
    QPushButton *cancelBtn=new QPushButton("Cancel");
    connect(cancelBtn,SIGNAL(clicked()),this,SLOT(reject()));
    hbox->addWidget(cancelBtn);
    setLayout(hbox);
}

void SetEmptyData::moveData()
{
    ArchStat arch(cross->Region,cross->Area,ldate->date());
    cross->Attach(arch);
    cross->SetData(ldate->date(),lrnd->isChecked());

    emit accept();
}
