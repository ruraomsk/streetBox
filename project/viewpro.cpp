#include "viewpro.h"

ViewPro::ViewPro(QWidget *parent,Project *project) : QWidget(parent)
{
    this->project=project;
    tab=new QTabWidget;
    top();
    tab->addTab(wtop,"Основные");
    appendTabCross();

    QGridLayout *grid=new QGridLayout(this);
    grid->addWidget(tab,0,1);
    setLayout(grid);

    this->show();
}

//Вызов формы добавления перекрестка
void ViewPro::addCross(int flag)
{
    //какими данными заполнить перекресток
    // flag ==0 - Создаем пустой перекресток
    //      ==1 - загружаем данные из Базы данных
    //      ==2 - загружаем из CSV
    switch (flag) {
    case 0:{
        FormGetEmptyCross frm(project,this);
        if (frm.exec()==QDialog::Accepted) {
            project->isChanged=true;
            appendTabCross();
        };
        break;
    }
    case 1:{
        FormGetDbCross fdb(project,this);
        if (fdb.exec()==QDialog::Accepted) {
            project->isChanged=true;
            appendTabCross();
        }
        break;
    }
    case 2:{
        ReadCSVFile fCSV(project,this);
        if (fCSV.exec()==QDialog::Accepted) {
            project->isChanged=true;
            appendTabCross();
        }
        break;
    }
    default:
        Support::ErrorMessage("Нет такой команды");
        return;
    }
}
//Вызов формы выбора для удаления перекрестка
void ViewPro::delCross()
{
    if(!isCross()) return;
    if (QMessageBox::Yes == QMessageBox(QMessageBox::Information, "Удаление", "Вы уверены?", QMessageBox::Yes|QMessageBox::No).exec())
    {
        for (int i = 0; i < project->crosses.size(); ++i) {
            if(project->crosses[i]->Number==tab->tabText(tab->currentIndex()).toInt()){
                project->crosses.removeAt(i);
                tab->removeTab(tab->currentIndex());
                project->isChanged=true;
                break;
            }
        }
        appendTabCross();
    }
    //    qDebug()<<"delete";

}

void ViewPro::addData(int flag)
{
    QString name=tab->tabText(tab->currentIndex());
    if(!isCross()) return;
    //какими данными заполнить перекресток
    // flag ==0 - Создаем пустой перекресток
    //      ==1 - загружаем данные из Базы данных
    //      ==2 - загружаем из CSV
    //      ==3 - загружаем почасовые данные
    switch (flag) {
    case 0:{
        for (int i = 0; i < project->crosses.size(); ++i) {
            if (project->crosses[i]->Number==name.toInt()){
                auto cross=project->crosses[i];
                SetEmptyData frm(cross,this);
                if (frm.exec()==QDialog::Accepted) {
                    project->isChanged=true;
                    project->crosses[i]=cross;
                    break;
                }
            }
        }
    }
        break;
    case 1:{
        for (int i = 0; i < project->crosses.size(); ++i) {
            if (project->crosses[i]->Number==name.toInt()){
                auto cross=project->crosses[i];
                SetDBData frd(cross,this);
                if (frd.exec()==QDialog::Accepted) {
                    project->isChanged=true;
                    project->crosses[i]=cross;
                    break;

                }

            }
        }
    }

        break;
    case 2:{
        for (int i = 0; i < project->crosses.size(); ++i) {
            if (project->crosses[i]->Number==name.toInt()){
                auto cross=project->crosses[i];
                SetCSVData csvd(cross,this);
                if (csvd.exec()==QDialog::Accepted) {
                    project->isChanged=true;
                    project->crosses[i]=cross;
                    break;

                }

            }
        }
    }
        break;

    case 3:{
        for (int i = 0; i < project->crosses.size(); ++i) {
            if (project->crosses[i]->Number==name.toInt()){
                auto cross=project->crosses[i];
                SetHourData cshd(cross,this);
                if (cshd.exec()==QDialog::Accepted) {
                    project->isChanged=true;
                    project->crosses[i]=cross;
                    break;

                }

            }
        }
    }

        break;
    default:
        Support::ErrorMessage("такая команда не поддерживается");
        return;
    }
    static_cast<ViewCross*>(tab->widget(tab->currentIndex()))->Update();

}

void ViewPro::exportCrossDataToCSV()
{
    if(!isCross()) return;
    QFileDialog dialog(this,"Укажите имя файла для экспорта",ini.getString("export/path"));
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setNameFilters(ini.getString("export/ext").split(" "));
    if(dialog.exec()!=QDialog::Accepted){
        return;
    }
    QStringList nameFiles=dialog.selectedFiles();
    if (nameFiles.size()==1) {
        ViewCross *c=static_cast<ViewCross*>(tab->widget(tab->currentIndex()));

        c->cross->SaveCrossToCSV(Support::CorrectNameFile(nameFiles[0],ini.getString("export/path")));
        //        project->isChanged=false;
    } else{
        QMessageBox msg;
        msg.setText("Указано для сохранения более одного файла!");
        msg.exec();
    }
    return;
}

void ViewPro::repairDataCross()
{
    if(!isCross()) return;
    ViewCross *c=static_cast<ViewCross*>(tab->widget(tab->currentIndex()));
    c->repairData();
    project->isChanged=true;
}

void ViewPro::AddXtEmpty()
{
    DialogAddEmptyXT frm(project,this);
    if (frm.exec()==QDialog::Accepted) {
        project->isChanged=true;
        appendTabCross();
    };
    qDebug()<<"AddXtEmpty";
}

void ViewPro::AddXtFromJSON()
{
    ReadJSONFile rd(project,this);
    if (rd.exec()==QDialog::Accepted) {
        project->isChanged=true;
        appendTabCross();
    };
}

void ViewPro::DeleteXt()
{
    if(!isXT()) return;
    if (QMessageBox::Yes == QMessageBox(QMessageBox::Information, "Удаление", "Вы уверены?", QMessageBox::Yes|QMessageBox::No).exec())
    {
        for (int i = 0; i < project->xctrls.size(); ++i) {
            if(("XT:"+project->xctrls[i]->name)==tab->tabText(tab->currentIndex())){
                project->xctrls.removeAt(i);
                tab->removeTab(tab->currentIndex());
                project->isChanged=true;
                break;
            }
        }
        appendTabCross();
    }

    qDebug()<<"DeleteXT";
}

void ViewPro::SaveToJson()
{
    if(!isXT()) return;
    QFileDialog dialog(this,"Укажите имя файла для JSON",ini.getString("export/path"));
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    QString list={"*.json *.*"};
    dialog.setNameFilters(list.split(" "));
    if(dialog.exec()!=QDialog::Accepted){
        return;
    }
    QStringList nameFiles=dialog.selectedFiles();
    if (nameFiles.size()==1) {
        ViewXctrl *x=static_cast<ViewXctrl*>(tab->widget(tab->currentIndex()));

        x->SaveToJSON(Support::CorrectNameFile(nameFiles[0],list));
        //        project->isChanged=false;
    } else{
        QMessageBox msg;
        msg.setText("Указано для сохранения более одного файла!");
        msg.exec();
    }
    return;
    qDebug()<<"SaveToJson";

}




void ViewPro::appendTabCross()
{
    //Вначале проверим имена табов
    for (int i = 0; i < tab->count(); ++i) {
        if (tab->tabText(i).contains("XT:")){
            ViewXctrl *xt=static_cast<ViewXctrl *>(tab->widget(i));
            if(xt->getName()!=tab->tabText(i)){
                tab->setTabText(i,xt->getName());
            }
        }
    }

    foreach (auto c, project->crosses) {
        bool found=false;
        for (int i = 0; i < tab->count(); ++i) {
            if (QString::number(c->Number)==tab->tabText(i)) {
                found=true;
                break;
            }
        }
        if(!found){
            tab->addTab(new ViewCross(project,c,this),QString::number(c->Number));
        }
    }
    foreach (auto xt, project->xctrls) {
        bool found=false;
        for (int i = 0; i < tab->count(); ++i) {
            if (!tab->tabText(i).contains("XT:")) continue;
            if (("XT:"+xt->name)==tab->tabText(i)) {
                found=true;
                break;
            }
            if (xt->name.size()!=0) continue;
        }
        if(!found){
            tab->addTab(new ViewXctrl(project,xt,this),"XT:"+xt->name);
        }
    }

}
//Перемещаем данные из виджетов редактирования в проект
void ViewPro::moveData()
{
    project->description=ldesc->text();
    project->Region=lregion->text().toInt();
    project->Area=larea->text().toInt();
    project->SubArea=lsubarea->text().toInt();
    //    qDebug()<<lstep->text().toInt();
    if(!Support::isVerStep(lstep->text().toInt())){
        Support::ErrorMessage("Интервал заполнения не верный");
        return;
    }
    project->Step=lstep->text().toInt();
    if(lchanels->text().toInt()<1||lchanels->text().toInt()>16) {
        Support::ErrorMessage("Ошибочное кол-во каналов");
        return;
    }
    project->Chanels=lchanels->text().toInt();
    project->isChanged=true;
}
//Заполняем виджет редактирования основных параметров проекта
void ViewPro::top()
{
#define maxSize 500,100
    wtop=new QWidget;
    lregion=new QLineEdit(QString::number(project->Region),this);
    lregion->setMaximumSize(maxSize);
    larea=new QLineEdit(QString::number(project->Area),this);
    larea->setMaximumSize(maxSize);
    lsubarea=new QLineEdit(QString::number(project->SubArea),this);
    lsubarea->setMaximumSize(maxSize);
    ldesc=new QLineEdit(project->description,this);
    ldesc->setMaximumSize(maxSize);
    lstep=new QLineEdit(QString::number(project->Step),this);
    lstep->setMaximumSize(maxSize);
    lchanels=new QLineEdit(QString::number(project->Chanels),this);
    lchanels->setMaximumSize(maxSize);

    QFormLayout *hbox=new QFormLayout();
    hbox->addRow("Описание проекта",ldesc);
    hbox->addRow("Регион",lregion);
    hbox->addRow("Район",larea);
    hbox->addRow("Подрайон",lsubarea);
    hbox->addRow("Интервал времени",lstep);
    hbox->addRow("Кол-во каналов",lchanels);
    QPushButton *okBtn=new QPushButton("Сохранить");
    okBtn->setMaximumSize(100,100);
    connect(okBtn,SIGNAL(clicked()),this,SLOT(moveData()));
    QHBoxLayout *btn=new QHBoxLayout;
    btn->addWidget(okBtn);

    hbox->addRow(btn);

    wtop->setLayout(hbox);
}

bool ViewPro::isCross()
{
    QString name=tab->tabText(tab->currentIndex());
    if (name=="Основные"||name.contains("XT:")) {
        Support::ErrorMessage("Не выбран перекресток");
        return false;
    }
    return true;
}

bool ViewPro::isXT()
{
    QString name=tab->tabText(tab->currentIndex());
    if (!name.contains("XT:")) {
        Support::ErrorMessage("Не выбран XT");
        return false;
    }
    return true;

}


