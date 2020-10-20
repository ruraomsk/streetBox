#include "readjsonfile.h"

ReadJSONFile::ReadJSONFile(Project *project,QWidget *parent) : QDialog(parent)
{
    this->project=project;
    QFileDialog dialog(this,"Укажите имя файла для чтения",ini.getString("import/path"));
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    QString list={"*.json *.*"};
    dialog.setNameFilters(list.split(" "));
    if(dialog.exec()!=QDialog::Accepted){
        return;
    }
    QStringList nameFiles=dialog.selectedFiles();
    if (nameFiles.size()!=1) {
        Support::ErrorMessage("Указано для чтения более одного файла!");
        emit reject();
        return;
    }
    QFile file(nameFiles[0]);
    if (!file.open(QIODevice::ReadOnly|QIODevice::Text)){
        Support::ErrorMessage("Ошибка чтения файла"+file.fileName());
        emit reject();
        return;
    }
    QString json=file.readAll();
    file.close();
    QJsonParseError jError;
    QJsonDocument jdoc=QJsonDocument::fromJson(json.toUtf8(),&jError);
    if (jError.error!=QJsonParseError::NoError){
        Support::ErrorMessage(jError.errorString());
        emit reject();
        return;
    }
    xt=new Xctrl(jdoc.toVariant().toMap());
    if (isNameCorrect(xt->name)){
        project->xctrls.append(xt);
        emit accept();
        return;
    }
    lname=new QLineEdit(xt->name);
    QFormLayout *hbox=new QFormLayout();
    hbox->addRow("Имя ХТ",lname);
    QPushButton *okBtn=new QPushButton("OK");
    connect(okBtn,&QPushButton::clicked,this,&ReadJSONFile::setData);
    QHBoxLayout *btn=new QHBoxLayout;
    btn->addWidget(okBtn);
    QPushButton *cancelBtn=new QPushButton("Cancel");
    connect(cancelBtn,SIGNAL(clicked()),this,SLOT(reject()));
    btn->addWidget(cancelBtn);
    hbox->addRow(btn);
    setLayout(hbox);


}

void ReadJSONFile::setData()
{
    if (isNameCorrect(lname->text())){
        xt->name=lname->text();
        project->xctrls.append(xt);
        emit accept();
        return;
    }
}

bool ReadJSONFile::isNameCorrect(QString name)
{
    if(xt->name.size()==0){
        Support::ErrorMessage("Заполните имя XT");
        return false;
    }
    foreach (auto val, project->xctrls) {
        if(val->name==name) {
            Support::ErrorMessage("Такое имя XT уже есть");
            return false;
        }
    }
    return true;
}
