#include "mainwindow.h"
#include "ui_mainwindow.h"

extern Setup ini;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setStyle(QStyleFactory::create("Fusion"));
    resize(ini.getSize("window/size"));
    setGeometry(ini.getPoint("window/point").x(),ini.getPoint("window/point").y(),ini.getSize("window/size").width(),ini.getSize("window/size").height());
    createAtions();
    createMenus();
    createStatusBar();

    db=QSqlDatabase::addDatabase("QPSQL");
    db.setHostName(ini.getString("db/hostname"));
    db.setPort(ini.getInt("db/port"));
    db.setDatabaseName(ini.getString("db/dbname"));
    db.setUserName(ini.getString("db/username"));
    db.setPassword(ini.getString("db/password"));
    bool ok=db.open();
    if (!ok){
        QSqlError last=db.lastError();
        return;
    }


}
void MainWindow::createAtions(){
    newAct=new QAction(QIcon(":/images/filenew.png"),tr("&Новый"),this);
    newAct->setShortcut((QKeySequence::New));
    newAct->setStatusTip(tr("Создать новый проект"));
    connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));
    openAct=new QAction(QIcon(":/images/fileopen.png"),tr("&Открыть"),this);
    openAct->setShortcut((QKeySequence::Open));
    openAct->setStatusTip(tr("Открыть существуюший проект"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));
    saveAct=new QAction(QIcon(":/images/filesave.png"),tr("&Сохранить"),this);
    saveAct->setShortcut((QKeySequence::Save));
    saveAct->setStatusTip(tr("Сохранить проект"));
    saveAct->setVisible(false);
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));
    saveAsAct=new QAction(tr("Сохранить &Как"),this);
    saveAsAct->setShortcut((QKeySequence::SaveAs));
    saveAsAct->setStatusTip(tr("Сохранить проект с другим именем"));
    saveAsAct->setVisible(false);
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));
    exitAct=new QAction(tr("&Выход"),this);
    exitAct->setShortcut((QKeySequence::Quit));
    exitAct->setStatusTip(tr("Завершение программы"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(ExitProgramm()));
    addingCrossDb=new QAction(QIcon(":/images/bd.png"),tr("Из &Базы"),this);
    addingCrossDb->setStatusTip("Добавление перекрестка в проект из базы данных");
    connect(addingCrossDb, SIGNAL(triggered()), this, SLOT(addCrossDb()));
    addingCrossCSV=new QAction(QIcon(":/images/csv.png"),tr("Из &Текстового файла"),this);
    addingCrossCSV->setStatusTip("Добавление перекрестка в проект из тектового файла");
    connect(addingCrossCSV, SIGNAL(triggered()), this, SLOT(addCrossCSV()));
    addingCrossEmpty=new QAction(QIcon(":/images/addcross.png"),tr("&Пустой"),this);
    addingCrossEmpty->setStatusTip("Добавление пустого перекрестка в проект ");
    connect(addingCrossEmpty, SIGNAL(triggered()), this, SLOT(addCrossEmpty()));
    deleteCross=new QAction(QIcon(":/images/removeCross.png"),tr("&Удалить перекресток"),this);
    deleteCross->setStatusTip("Удаление перекрестка из проекта");
    connect(deleteCross, SIGNAL(triggered()), this, SLOT(delCross()));
    crossAddDb=new QAction(QIcon(":/images/bd.png"),tr("Данные из &Базы"),this);
    crossAddDb->setStatusTip("Добавление данных из базы данных");
    connect(crossAddDb, SIGNAL(triggered()), this, SLOT(crossDbData()));
    crossAddCSV=new QAction(QIcon(":/images/csv.png"),tr("Из &Текстового файла"),this);
    crossAddCSV->setStatusTip("Добавление данных из текстового файла");
    connect(crossAddCSV, SIGNAL(triggered()), this, SLOT(crossCSVData()));
    crossAddEmpty=new QAction(QIcon(":/images/addcross.png"),tr("&Нулевые данные"),this);
    crossAddEmpty->setStatusTip("Добавление нулевых данных ");
    connect(crossAddEmpty, SIGNAL(triggered()), this, SLOT(crossEmptyData()));
    crossAddHour=new QAction(tr("&Почасовые данные"),this);
    crossAddHour->setStatusTip("Добавление почасовых данных ");
    connect(crossAddHour, SIGNAL(triggered()), this, SLOT(crossAddHourData()));

    exportCrossToCSV=new QAction(QIcon(":/images/csv.png"),tr("&перекрестка в CSV"),this);
    exportCrossToCSV->setStatusTip("Экспорт данных перекрестка в тестовый файл с разделителями");
    connect(exportCrossToCSV, SIGNAL(triggered()), this, SLOT(exportCrossToCSVData()));
    repairDataCross=new QAction(QIcon(":/images/textjustify.png"),tr("Исправить данные перекрестка"),this);
    repairDataCross->setStatusTip("Будут исправлены ошибочные данные на перекрестке");
    connect(repairDataCross, SIGNAL(triggered()), this, SLOT(RepairDataFromCross()));
    editSetup=new QAction(QIcon(":/images/setup.png"),tr("Настройка"),this);
    editSetup->setStatusTip("Изменение настройки программы");
    connect(editSetup, SIGNAL(triggered()), this, SLOT(SetupEdit()));

    addXtEmpty=new QAction(QIcon(":/images/addXtEmpty.png"),"Добавить пустой XT",this);
    addXtEmpty->setStatusTip("Добавляется пустой ХТ");
    connect(addXtEmpty,SIGNAL(triggered()),this,SLOT(AddXtEmpty()));
    addXtFromJSON=new QAction(QIcon(":/images/addXtFromJson.png"),"Добавить XT из файла JSON",this);
    addXtFromJSON->setStatusTip("Добавляется новый ХТ из файла в формате JSON");
    connect(addXtFromJSON,SIGNAL(triggered()),this,SLOT(AddXtFromJSON()));
    addXtFromBD=new QAction(QIcon(":/images/bd.png"),"Добавить XT из БД",this);
    addXtFromBD->setStatusTip("Добавляется новый ХТ из БД");
    connect(addXtFromBD,SIGNAL(triggered()),this,SLOT(AddXtFromBD()));
    deleteXt=new QAction(QIcon(":/images/removeCross.png"),"Удалить текущий ХТ",this);
    deleteXt->setStatusTip("Удаляется текущий ХТ из проекта");
    connect(deleteXt,SIGNAL(triggered()),this,SLOT(DeleteXt()));
    saveToJSON=new QAction(QIcon(":/images/addXtFromJson.png"),"Сохранить XT в файл JSON",this);
    saveToJSON->setStatusTip("Сохраняется текущий ХТ в файл в формате JSON");
    connect(saveToJSON,SIGNAL(triggered()),this,SLOT(SaveToJSON()));
    saveXTToBD=new QAction(QIcon(":/images/bd.png"),"Сохранить XT в БД",this);
    saveXTToBD->setStatusTip("Сохраняется текущий ХТ в БД");
    connect(saveXTToBD,SIGNAL(triggered()),this,SLOT(SaveXTToBD()));

}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&Проект"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);
    editMenu = menuBar()->addMenu(tr("&Перекресток"));
    editMenu->setDisabled(true);
    addCrossMenu=new QMenu("&Добавить перекресток",editMenu);
    addCrossMenu->addAction(addingCrossDb);
    addCrossMenu->addAction(addingCrossCSV);
    addCrossMenu->addAction(addingCrossEmpty);
    editMenu->addMenu(addCrossMenu);
    addDataMenu=new QMenu("Д&обавить данные",editMenu);
    addDataMenu->addAction(crossAddDb);
    addDataMenu->addAction(crossAddCSV);
    addDataMenu->addAction(crossAddEmpty);
    addDataMenu->addAction(crossAddHour);
    editMenu->addMenu(addDataMenu);
    xtMenu=menuBar()->addMenu("&Управление ХТ");
    xtMenu->setDisabled(true);
    xtMenu->addAction(addXtEmpty);
    xtMenu->addAction(addXtFromJSON);
    xtMenu->addAction(addXtFromBD);
    xtMenu->addAction(deleteXt);
    xtMenu->addAction(saveToJSON);
    xtMenu->addAction(saveXTToBD);

    exportMenu=new QMenu("&Экпорт");
    exportMenu->addAction(exportCrossToCSV);
    //    repairMenu=new QMenu("&Исправить ошибки");
    //    importMenu->addAction(importCrossFromCSV);
    editMenu->addAction(repairDataCross);
    editMenu->addAction(deleteCross);
    editMenu->addMenu(exportMenu);
    //    editMenu->addMenu(importMenu);
    setupMenu=menuBar()->addMenu((tr("&Настройка")));
    setupMenu->addAction(editSetup);
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Готов"));
}

void MainWindow::showStatus(QString last)
{
    statusBar()->showMessage("Проект "+project.getDesc()+" файл "+project.getNameFile()+last);
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *size)
{
    ini.setSize("window/size",size->size());
    QMainWindow::resizeEvent(size);
}

void MainWindow::moveEvent(QMoveEvent *point)
{
    ini.setPoint("window/point",point->pos());
    QMainWindow::moveEvent(point);

}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(project.IsEmpty()) {
        QMainWindow::closeEvent(event);
        return;
    }
    if(!project.isChanged) {
        QMainWindow::closeEvent(event);
        return;
    }
    //    qDebug()<<"close"<<project.IsEmpty()<<project.isChanged;
    QMessageBox *msg=new QMessageBox(this);
    msg->setText("Есть несохраненные измения");
    msg->setInformativeText("Может все-таки их сохраним?");
    msg->setStandardButtons(QMessageBox::Save | QMessageBox::Discard );
    msg->setDefaultButton(QMessageBox::Save);
    int ret = msg->exec();
    msg->close();
    if (ret==QMessageBox::Save){
        save();
    }
    QMainWindow::closeEvent(event);
}

void MainWindow::newFile()
{
    if(!project.IsEmpty()&&project.isChanged){
        Support::ErrorMessage("Предыдущий проект изменен! Сохраните его.");
        return;
    }
    QString nameFile=ini.getString("projects/path")+"/"+ini.getString("projects/defName");
    project=Project(nameFile,ini.getString("projects/defDesc"));
    if (project.error.size()==0){
        saveAct->setVisible(true);
        saveAsAct->setVisible(true);
        editMenu->setEnabled(true);
        xtMenu->setEnabled(true);
        viewPro=new ViewPro(this,&project);
        setCentralWidget(viewPro);
        showStatus(" создан");
        project.isChanged=false;
    } else {
        showStatus(" ошибка "+project.error);
    }
}

void MainWindow::open()
{
    if(!project.IsEmpty()&&project.isChanged){
        Support::ErrorMessage("Предыдущий проект изменен! Сохраните его.");
        return;
    }
    QString fileName=QFileDialog::getOpenFileName(this,"Выберите проект",ini.getString("projects/path"),ini.getString("projects/ext"));
    if(fileName.size()==0) return;
    project=Project(fileName);
    if (project.error.size()==0){
        saveAct->setVisible(true);
        saveAsAct->setVisible(true);
        editMenu->setEnabled(true);
        xtMenu->setEnabled(true);
        viewPro=new ViewPro(this,&project);
        setCentralWidget(viewPro);
        showStatus(" загружен");
        project.isChanged=false;
    } else {
        showStatus(" ошибка "+project.error);
    }
}

void MainWindow::save()
{
    viewPro->moveData();
    project.saveProject();
    if (project.error.size()==0) {
        project.isChanged=false;
        showStatus(" сохранен");
    }
    else  showStatus(" ошибка "+project.error);
    return;
}

void MainWindow::saveAs()
{
    QFileDialog dialog(this,"Укажите имя файла для сохранения проекта",ini.getString("projects/path"));
//    dialog.setDefaultSuffix(".prj");
    dialog.selectFile(project.description);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setNameFilter(ini.getString("projects/ext"));
    if(dialog.exec()!=QDialog::Accepted){
        showStatus(" не сохранен");
        return;
    }
    QStringList nameFiles=dialog.selectedFiles();
    if (nameFiles.size()==1) {
        project.ChangeNameFile(Support::CorrectNameFile(nameFiles[0],ini.getString("projects/ext")));
        viewPro->moveData();
        project.saveProject();
        showStatus(" сохранен");
        project.isChanged=false;
    } else{
        Support::ErrorMessage("Указано для сохранения более одного файла!");
        showStatus(" не сохранен");
    }
    return;
}

void MainWindow::addCrossDb()
{
    if(project.IsEmpty()) return;
    viewPro->addCross(1);
}

void MainWindow::addCrossCSV()
{
    if(project.IsEmpty()) return;
    viewPro->addCross(2);
}

void MainWindow::addCrossEmpty()
{
    if(project.IsEmpty()) return;
    viewPro->addCross(0);
}


void MainWindow::delCross()
{
    if(project.IsEmpty()) return;
    viewPro->delCross();

}

void MainWindow::crossDbData()
{
    if(project.IsEmpty()) return;
    viewPro->addData(1);
}

void MainWindow::crossCSVData()
{
    if(project.IsEmpty()) return;
    viewPro->addData(2);
}

void MainWindow::crossEmptyData()
{
    if(project.IsEmpty()) return;
    viewPro->addData(0);
}

void MainWindow::crossAddHourData()
{
    if(project.IsEmpty()) return;
    viewPro->addData(3);
}

void MainWindow::exportCrossToCSVData()
{
    if(project.IsEmpty()) return;
    viewPro->exportCrossDataToCSV();

}

void MainWindow::RepairDataFromCross()
{
    viewPro->repairDataCross();
}

void MainWindow::ExitProgramm()
{
    if(project.IsEmpty()) {
        emit close();
        return;
    }
    if(!project.isChanged) {
        emit close();
        return;
    }
    //    qDebug()<<"close"<<project.IsEmpty()<<project.isChanged;
    QMessageBox *msg=new QMessageBox(this);
    msg->setText("Есть несохраненные измения");
    msg->setInformativeText("Может все-таки их сохраним?");
    msg->setStandardButtons(QMessageBox::Save | QMessageBox::Discard );
    msg->setDefaultButton(QMessageBox::Save);
    int ret = msg->exec();
    msg->close();
    if (ret==QMessageBox::Save){
        save();
    }
    emit close();
}

void MainWindow::SetupEdit()
{
    ViewSetup vs(this);
    vs.exec();
}

void MainWindow::AddXtEmpty()
{
    viewPro->AddXtEmpty();
}

void MainWindow::AddXtFromJSON()
{
    viewPro->AddXtFromJSON();

}

void MainWindow::AddXtFromBD()
{
    viewPro->AddXtFromBD();
}

void MainWindow::SaveToJSON()
{
    viewPro->SaveToJson();
}

void MainWindow::SaveXTToBD()
{
    viewPro->SaveXTToBD();
}

void MainWindow::DeleteXt()
{
    viewPro->DeleteXt();
}


