#include "viewpro.h"
extern Common common;
ViewPro::ViewPro(QWidget *parent,Project *project) : QWidget(parent)
{
    this->project=project;
    tab=new QTabWidget;
    top();
    tab->addTab(wtop,"Основные");
    appendTabCross();

    QGridLayout *grid=new QGridLayout();
    grid->addWidget(tab,0,1);
    setLayout(grid);

    //    this->show();
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
}

void ViewPro::AddXtFromJSON()
{
    ReadJSONFile rd(project,this);
    if (rd.exec()==QDialog::Accepted) {
        project->isChanged=true;
        appendTabCross();
    };
}

void ViewPro::AddXtFromBD()
{
    FormGetDBXctrl rd(project,this);
    if (rd.exec()==QDialog::Accepted) {
        project->isChanged=true;
        appendTabCross();
    };
}

void ViewPro::SaveXTToBD()
{
    if(project->xctrls.size()==0){
        Support::ErrorMessage("ХТ не существует!");
        return;
    }
    State *state=project->makeState();
    Support::saveXT(state->Region,state->Area,state->SubArea,state->ToJSON().toUtf8());
    Support::Message("XT сохранен в БД");
    return;
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
    if(!Support::isVerStep(lstepdev->text().toInt())){
        Support::ErrorMessage("Интервал заполнения не верный");
        return;
    }
    project->StepDevice=lstepdev->text().toInt();
    if(!Support::isVerStep(lstepxt->text().toInt())){
        Support::ErrorMessage("Интервал заполнения не верный");
        return;
    }
    project->StepXT=lstepxt->text().toInt();
    if(lchanels->text().toInt()<1||lchanels->text().toInt()>16) {
        Support::ErrorMessage("Ошибочное кол-во каналов");
        return;
    }
    project->Chanels=lchanels->text().toInt();
    if (lswitch->isChecked()) project->Switch=true; else project->Switch=false;
    if (lrelease->isChecked()) project->Release=true; else project->Release=false;
    common.stepXT=project->StepXT;
    if (gbox->isChecked()) project->UseStrategy=true;
    else project->UseStrategy=false;
    common.use=project->UseStrategy;
    for (int i = 0; i < project->prioryty.rows; ++i) {
        for (int j = 0; j < project->prioryty.columns; ++j) {
            project->prioryty.prior[i][j]=ptable->item(i,j)->text().toInt();
        }
    }
    for (int i = 0; i < 12; ++i) {
        for (int j = 0; j < 2; ++j) {
            project->external[i][j]=etable->item(i,j)->text().toInt();
        }
    }

    project->isChanged=true;
}

void ViewPro::loadTable()
{
    resultTable();

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
    lstepdev=new QLineEdit(QString::number(project->StepDevice),this);
    lstepdev->setMaximumSize(maxSize);
    lstepxt=new QLineEdit(QString::number(project->StepXT),this);
    lstepxt->setMaximumSize(maxSize);
    lchanels=new QLineEdit(QString::number(project->Chanels),this);
    lchanels->setMaximumSize(maxSize);
    lswitch=new QCheckBox("");
    lswitch->setChecked(project->Switch);
    lrelease=new QCheckBox("");
    lrelease->setChecked(project->Release);
    grid=new QGridLayout(this);
    grid->setAlignment(Qt::AlignTop);

    QFormLayout *hbox=new QFormLayout();
    hbox->addRow("Описание проекта",ldesc);
    hbox->addRow("Регион",lregion);
    hbox->addRow("Район",larea);
    hbox->addRow("Подрайон",lsubarea);
    hbox->addRow("Опрос устройств",lstepdev);
    hbox->addRow("Интервал времени расчет XT",lstepxt);
    hbox->addRow("Кол-во каналов",lchanels);
    hbox->addRow("Разрешить расчет ",lswitch);
    hbox->addRow("Разрешить выполнение ",lrelease);
    grid->addLayout(hbox,0,0);

    gstyle=new QGroupBox("Способ расчета");
    garea=new QRadioButton("Центры областей");
    gbox=new QRadioButton("Лучи");
    if (project->UseStrategy){
        gbox->setChecked(true);
    } else {
        garea->setChecked(true);
    }
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(gstyle);
    vbox->addWidget(garea);
    vbox->addWidget(gbox);
    vbox->addStretch(1);
    grid->addLayout(vbox,1,0);

    QPushButton *okBtn=new QPushButton("Сохранить");
    okBtn->setMaximumSize(150,100);
    connect(okBtn,SIGNAL(clicked()),this,SLOT(moveData()));
    QPushButton *loadBtn=new QPushButton("Загрузить расчет");
    loadBtn->setMaximumSize(150,100);
    connect(loadBtn,SIGNAL(clicked()),this,SLOT(loadTable()));
    QHBoxLayout *btn=new QHBoxLayout;
    btn->addWidget(okBtn);
    btn->addWidget(loadBtn);
    grid->addLayout(btn,3,0);

    table();
    resultTable();
    extTable();
    wtop->setLayout(grid);

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

void ViewPro::table()
{
    ptable=new QTableWidget;
    ptable->setColumnCount(project->prioryty.columns);
    for (int var = 0; var < project->prioryty.columns; ++var) {
        QTableWidgetItem *t=new QTableWidgetItem(QString::number(var));
        t->setCheckState(Qt::Unchecked);
        ptable->setHorizontalHeaderItem(var,t);
    }

    ptable->setMaximumSize(ini.getSize("table/small"));
    for (int i = 0; i < project->prioryty.rows; ++i) {
        ptable->insertRow(i);
        for (int j = 0; j < project->prioryty.columns; ++j) {
            ptable->setItem(i,j,new QTableWidgetItem(QString::number(project->prioryty.prior[i][j])));

        }
    }
    ptable->resizeColumnsToContents();
    //    connect(wtable,SIGNAL(itemSelectionChanged()),this,SLOT(itemCkliked()));
    grid->addWidget(ptable,0,1);

}

void ViewPro::resultTable()
{
    QTableWidget *wtable=new QTableWidget;
    QList<ViewXctrl *> xts;
    for (int i = 0; i < tab->count(); ++i) {
        if (tab->tabText(i).contains("XT:")){
            ViewXctrl *xt=static_cast<ViewXctrl *>(tab->widget(i));
            xts.append(xt);
        }
    }
    if (xts.size()==0) {
        delete wtable;
        return;
    }
    wtable->setColumnCount(xts.size()+2);
    QTableWidgetItem *t=new QTableWidgetItem("Время");
    t->setCheckState(Qt::Unchecked);
    wtable->setHorizontalHeaderItem(0,t);
    for (int var = 0; var < xts.size(); ++var) {
        t=new QTableWidgetItem(xts[var]->getXctrl()->name);
        t->setCheckState(Qt::Unchecked);
        wtable->setHorizontalHeaderItem(var+1,t);
    }
    t=new QTableWidgetItem("Результат");
    t->setCheckState(Qt::Unchecked);
    wtable->setHorizontalHeaderItem(xts.size()+1,t);


    wtable->setMaximumSize(ini.getSize("table/size"));
    QList<QVector<int>> res;
    QVector<int> r(xts.size());
    auto v=xts[0]->getMatrix();
    for (int i = 0; i < v.size(); ++i) {
        wtable->insertRow(i);
        wtable->setItem(i,0,new QTableWidgetItem(v[i].at(0)));
        res.append(r);
    }
    int col=1;
    foreach (auto xt, xts) {
        auto v=xt->getMatrix();
        for (int i = 0; i < v.size(); ++i) {
            wtable->setItem(i,col,new QTableWidgetItem(v[i].at(1)));
            res[i][col-1]=v[i].at(1).toInt();
        }
        col++;
    }
    int row=0;
    foreach (auto rv, res) {
        QVector<int> ir(13);
        foreach (auto p, rv) {
            ir[p]++;
        }
        if (ir[0]==xts.size()){
            wtable->setItem(row,xts.size()+1,new QTableWidgetItem("По времени"));
        } else {
            int t=playKC(ir);
            if(t==0){
                wtable->setItem(row,xts.size()+1,new QTableWidgetItem("По времени"));
            } else {
                wtable->setItem(row,xts.size()+1,new QTableWidgetItem(QString::number(t)));
            }

        }
        row++;
    }
    wtable->resizeColumnsToContents();
    grid->addWidget(wtable,1,1);

}

void ViewPro::extTable()
{
    etable=new QTableWidget;
    etable->setColumnCount(2);
    QTableWidgetItem *t=new QTableWidgetItem("КС ");
    t->setCheckState(Qt::Unchecked);
    etable->setHorizontalHeaderItem(0,t);
    t=new QTableWidgetItem("План координации");
    t->setCheckState(Qt::Unchecked);
    etable->setHorizontalHeaderItem(1,t);


    etable->setMaximumSize(ini.getSize("table/small"));
    for (int i = 0; i < 12; ++i) {
        etable->insertRow(i);
        etable->setItem(i,0,new QTableWidgetItem(QString::number(project->external[i][0])));
        etable->setItem(i,1,new QTableWidgetItem(QString::number(project->external[i][1])));
    }
    etable->resizeColumnsToContents();
    grid->addWidget(etable,2,0);


}

int ViewPro::playKC(QVector<int> ir)
{
    for (int i = 0; i < project->prioryty.rows; ++i) {
        int t[3]={0,0,0};
        t[0]=ir[project->prioryty.prior[i][0]];
        t[1]=ir[project->prioryty.prior[i][1]];
        t[2]=ir[project->prioryty.prior[i][2]];
        if((t[0]+t[1]+t[2])==0) continue;
        if (t[0]>=t[1]&&t[0]>=t[2]) return project->prioryty.prior[i][0];
        if (t[1]>=t[2]) return project->prioryty.prior[i][1];
        return project->prioryty.prior[i][2];
    }
    return 0;
}


