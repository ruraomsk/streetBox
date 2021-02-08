#include "formgetemptycross.h"

FormGetEmptyCross::FormGetEmptyCross( Project *proj,QWidget *parent): QDialog(parent)
{
    project=proj;
    setWindowTitle("Укажите необходимые параметры");
    lname=new QLineEdit("",this);
    ldesc=new QLineEdit("",this);
    lregion=new QLineEdit(QString::number(project->Region),this);
    larea=new QLineEdit(QString::number(project->Area),this);
    lsubarea=new QLineEdit(QString::number(project->SubArea),this);
    lstep=new QLineEdit(QString::number(project->StepDevice),this);
    lchanel=new QLineEdit(QString::number(project->Chanels),this);
    lrnd=new QCheckBox();

    QFormLayout *hbox=new QFormLayout();
    hbox->addRow("Номер перекрестка",lname);
    hbox->addRow("Описание перекрестка",ldesc);
    hbox->addRow("Регион",lregion);
    hbox->addRow("Район",larea);
    hbox->addRow("Подрайон",lsubarea);
    hbox->addRow("Интервал заполнения",lstep);
    hbox->addRow("Ко-во каналов",lchanel);
    hbox->addRow("Случайные данные",lrnd);
    QPushButton *okBtn=new QPushButton("OK");
    connect(okBtn,SIGNAL(clicked()),this,SLOT(moveData()));
    QHBoxLayout *btn=new QHBoxLayout;
    btn->addWidget(okBtn);
    QPushButton *cancelBtn=new QPushButton("Cancel");
    connect(cancelBtn,SIGNAL(clicked()),this,SLOT(reject()));
    btn->addWidget(cancelBtn);
    hbox->addRow(btn);
    setLayout(hbox);


}

void FormGetEmptyCross::moveData()
{
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
    cross->Rnd=lrnd->isChecked();
    project->crosses.append(cross);
    emit accept();
}
