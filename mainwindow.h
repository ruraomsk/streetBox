#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QResizeEvent>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QStyleFactory>
#include "setup.h"
#include "project/viewpro.h"
#include "viewsetup.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    virtual void resizeEvent(QResizeEvent *size);
    virtual void moveEvent(QMoveEvent *point);
    virtual void closeEvent(QCloseEvent *event);
private slots:
    void newFile();
    void open();
    void save();
    void saveAs();
    void addCrossDb();
    void addCrossCSV();
    void addCrossEmpty();
    void delCross();
    void crossDbData();
    void crossCSVData();
    void crossEmptyData();
    void crossAddHourData();
    void exportCrossToCSVData();
    void RepairDataFromCross();
    void ExitProgramm();
    void SetupEdit();
    void AddXtEmpty();
    void AddXtFromJSON();
    void SaveToJSON();
    void DeleteXt();
private:
    void createAtions();
    void createMenus();
    void createStatusBar();
public:

    void showStatus(QString last);

private:
    QMenu    *fileMenu;
    QMenu    *editMenu;
    QMenu    *xtMenu;
    QMenu    *setupMenu;

    QMenu    *addCrossMenu;
    QMenu    *addDataMenu;
    QMenu    *exportMenu;
    QMenu    *importMenu;
    QMenu    *repairMenu;
    QMenu    *addXtMenu;
    QMenu    *exportXt;

    QToolBar *fileToolBar;
    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *exitAct;
    ViewPro *viewPro;

    QAction *addingCrossDb;
    QAction *addingCrossEmpty;
    QAction *addingCrossCSV;
    QAction *deleteCross;
    QAction *crossAddDb;
    QAction *repairDataCross;

    QAction *crossAddCSV;
    QAction *crossAddEmpty;
    QAction *crossAddHour;
    QAction *exportCrossToCSV;
    QAction *importCrossFromCSV;

    QAction *addXtEmpty;
    QAction *addXtFromJSON;
    QAction *saveToJSON;
    QAction *deleteXt;
    QAction *editSetup;

    Ui::MainWindow *ui;

    Project project;
    QSqlDatabase db;

};
#endif // MAINWINDOW_H
