#include "formgetdbcross.h"

FormGetDbCross::FormGetDbCross(Project *proj, QWidget *parent):QDialog(parent)
{
    project=proj;
    auto list=Support::listCrosses(project->Region,project->Area);
    combo=new QComboBox;
    combo->addItems(list);
    combo->setEditable(false);
    connect(combo,&QComboBox::textActivated,this,&FormGetDbCross::ChangedDK);

    QFormLayout *hbox=new QFormLayout();
    hbox->addWidget(combo);
    auto lis=list[0].split(":");
    if (lis.size()!=3){
        Support::ErrorMessage("Нет доступа к БД");
        return;
    }
    lname=new QLineEdit(lis[0],this);
    ldesc=new QLineEdit(lis[1],this);
    lregion=new QLineEdit(QString::number(project->Region),this);
    larea=new QLineEdit(QString::number(project->Area),this);
    lsubarea=new QLineEdit(lis[2],this);
    lstep=new QLineEdit(QString::number(project->StepDevice),this);
    lchanel=new QLineEdit(QString::number(project->Chanels),this);
    hbox->addRow("Номер перекрестка",lname);
    hbox->addRow("Описание перекрестка",ldesc);
    hbox->addRow("Регион",lregion);
    hbox->addRow("Район",larea);
    hbox->addRow("Подрайон",lsubarea);
    hbox->addRow("Интервал заполнения",lstep);
    hbox->addRow("Ко-во каналов",lchanel);

    QPushButton *okBtn=new QPushButton("OK");
    connect(okBtn,&QPushButton::clicked,this,&FormGetDbCross::moveData);
    QHBoxLayout *btn=new QHBoxLayout;
    btn->addWidget(okBtn);
    QPushButton *cancelBtn=new QPushButton("Cancel");
    connect(cancelBtn,SIGNAL(clicked()),this,SLOT(reject()));
    btn->addWidget(cancelBtn);
    hbox->addRow(btn);
    setLayout(hbox);


}

void FormGetDbCross::moveData()
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
    if (!Support::isVerStep(lstep->text().toInt())){
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
    project->crosses.append(cross);
    emit accept();

}

void FormGetDbCross::ChangedDK(const QString &name)
{
//    qDebug()<<name;
    auto list=name.split(":");
    lname->setText(list[0]);
    ldesc->setText(list[1]);
    lsubarea->setText(list[2]);
    update();

}
