#include "dialogaddemptyxt.h"

DialogAddEmptyXT::DialogAddEmptyXT(Project *project, QWidget *parent) :QDialog(parent)
{
    this->project=project;
    setWindowTitle("Укажите необходимые параметры");
    lname=new QLineEdit("",this);
    lregion=new QLineEdit(QString::number(project->Region),this);
    larea=new QLineEdit(QString::number(project->Area),this);
    lsubarea=new QLineEdit(QString::number(project->SubArea),this);
    lstep=new QLineEdit(QString::number(project->Step),this);

    QFormLayout *hbox=new QFormLayout();
    hbox->addRow("Имя ХТ",lname);
    hbox->addRow("Регион",lregion);
    hbox->addRow("Район",larea);
    hbox->addRow("Подрайон",lsubarea);
    hbox->addRow("Интервал заполнения",lstep);
    QPushButton *okBtn=new QPushButton("OK");
    connect(okBtn,SIGNAL(clicked()),this,SLOT(setData()));
    QHBoxLayout *btn=new QHBoxLayout;
    btn->addWidget(okBtn);
    QPushButton *cancelBtn=new QPushButton("Cancel");
    connect(cancelBtn,SIGNAL(clicked()),this,SLOT(reject()));
    btn->addWidget(cancelBtn);
    hbox->addRow(btn);
    setLayout(hbox);
}

void DialogAddEmptyXT::setData()
{
    if (lname->text().size()==0){
        Support::ErrorMessage("Укажите имя");
        return;
    }
    foreach (auto var, project->xctrls) {
        if (var->name==lname->text()) {
            Support::ErrorMessage("Такое имя уже используется");
            return;
        }
    }
    if(!Support::isVerStep(lstep->text().toInt())){
        Support::ErrorMessage("Интервал расчета не верный");
        return;
    }
    Xctrl *xctrl=new Xctrl(lregion->text().toInt(),larea->text().toInt(),lsubarea->text().toInt());
    xctrl->Step=lstep->text().toInt();
    xctrl->name=lname->text();
    project->xctrls.append(xctrl);
    emit accept();

}
