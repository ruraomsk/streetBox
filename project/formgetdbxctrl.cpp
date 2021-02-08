#include "formgetdbxctrl.h"


FormGetDBXctrl::FormGetDBXctrl(Project *proj, QWidget *parent):QDialog(parent)
{

    project=proj;
    auto list=Support::listXT();
    combo=new QComboBox;
    combo->addItems(list);
    combo->setEditable(false);
    connect(combo,&QComboBox::textActivated,this,&FormGetDBXctrl::ChangedXT);

    QFormLayout *hbox=new QFormLayout();
    hbox->addWidget(combo);
    auto lis=list[0].split(":");
    if (lis.size()!=3){
        Support::ErrorMessage("Нет доступа к БД");
        return;
    }
    getParams(list[0]);
    QPushButton *okBtn=new QPushButton("OK");
    connect(okBtn,&QPushButton::clicked,this,&FormGetDBXctrl::moveData);
    QHBoxLayout *btn=new QHBoxLayout;
    btn->addWidget(okBtn);
    QPushButton *cancelBtn=new QPushButton("Cancel");
    connect(cancelBtn,SIGNAL(clicked()),this,SLOT(reject()));
    btn->addWidget(cancelBtn);
    hbox->addRow(btn);
    setLayout(hbox);

}

void FormGetDBXctrl::moveData()
{
//    foreach (auto var, project->xctrls) {
//        if (var->Region==lregion && var->Area==larea && var->SubArea==lsubarea ) {
//            Support::ErrorMessage("Такой ХТ уже есть");
//            return;
//        }
//    }
    auto json = Support::getXT(lregion,larea,lsubarea);
    QJsonParseError jError;
    QJsonDocument jdoc=QJsonDocument::fromJson(json.toUtf8(),&jError);
    if (jError.error!=QJsonParseError::NoError){
        Support::ErrorMessage(jError.errorString());
        emit reject();
        return;
    }
    State *state=new State(jdoc.toVariant().toMap());
    project->updateFromState(state);
    emit accept();

}

void FormGetDBXctrl::ChangedXT(const QString &name)
{
    getParams(name);
    update();
}

void FormGetDBXctrl::getParams(const QString &name)
{
    auto list=name.split(":");
    lregion=getInt(list[0]);
    larea=getInt(list[1]);
    lsubarea=getInt(list[2]);
}

int FormGetDBXctrl::getInt(const QString name)
{
    auto list=name.split("=");
    return list[1].toInt();
}
