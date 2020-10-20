#include "viewsetup.h"

ViewSetup::ViewSetup(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Укажите параметры настройки");
    hostname=new QLineEdit(ini.getString("db/hostname"));
    port=new QLineEdit(QString::number(ini.getInt("db/port")));
    dbname=new QLineEdit(ini.getString("db/dbname"));
    username=new QLineEdit(ini.getString("db/username"));
    password=new QLineEdit(ini.getString("db/password"));

    EditProjectPath=new QPushButton(ini.getString("projects/path"));
    connect(EditProjectPath,SIGNAL(clicked()),this,SLOT(DirProjectPath()));
    EditCsvPath=new QPushButton(ini.getString("csv/path"));
    connect(EditCsvPath,SIGNAL(clicked()),this,SLOT(DirCsvPath()));
    EditImportPath=new QPushButton(ini.getString("import/path"));
    connect(EditImportPath,SIGNAL(clicked()),this,SLOT(DirImportPath()));
    EditExportPath=new QPushButton(ini.getString("export/path"));
    connect(EditExportPath,SIGNAL(clicked()),this,SLOT(DirExportPath()));
    QFormLayout *hbox=new QFormLayout();
    hbox->addRow(new QLabel("Раздел БД"));
    hbox->addRow("IP Сервер",hostname);
    hbox->addRow("Порт",port);
    hbox->addRow("Имя базы данных",dbname);
    hbox->addRow("Пользователь",username);
    hbox->addRow("Пароль",password);
    hbox->addRow(new QLabel("Каталоги"));
    hbox->addRow("Хранение проектов",EditProjectPath);
    hbox->addRow("Импорт данных по-канально csv",EditCsvPath);
    hbox->addRow("Импорт всх данных перекрестка txt",EditImportPath);
    hbox->addRow("Экспорт перекрестка в текстовом виде",EditExportPath);
    QPushButton *okBtn=new QPushButton("Изменить");
    connect(okBtn,SIGNAL(clicked()),this,SLOT(moveDataToSetup()));
    hbox->addWidget(okBtn);
    setLayout(hbox);
    setMinimumSize(500,300);

}

void ViewSetup::moveDataToSetup()
{
    ini.setString("db/hostname",hostname->text());
    ini.setInt("db/port",port->text().toInt());
    ini.setString("db/dbname",dbname->text());
    ini.setString("db/username",username->text());
    ini.setString("db/password",password->text());

    emit accept();
}

void ViewSetup::DirProjectPath()
{
    QString dir=QFileDialog::getExistingDirectory(this,"Каталог хранения проектов",ini.getString("projects/path"));
    if(dir.size()==0) return;
    ini.setString("projects/path",dir);
    EditProjectPath->setText(dir);
    update();
}

void ViewSetup::DirCsvPath()
{
    QString dir=QFileDialog::getExistingDirectory(this,"Каталог CSV",ini.getString("csv/path"));
    if(dir.size()==0) return;
    ini.setString("csv/path",dir);
    EditCsvPath->setText(dir);
    update();
}

void ViewSetup::DirImportPath()
{
    QString dir=QFileDialog::getExistingDirectory(this,"Каталог для импорта",ini.getString("import/path"));
    if(dir.size()==0) return;
    ini.setString("import/path",dir);
    EditImportPath->setText(dir);
    update();
}

void ViewSetup::DirExportPath()
{
    QString dir=QFileDialog::getExistingDirectory(this,"Каталог для экспорта",ini.getString("export/path"));
    if(dir.size()==0) return;
    ini.setString("export/path",dir);
    EditExportPath->setText(dir);
    update();
}
