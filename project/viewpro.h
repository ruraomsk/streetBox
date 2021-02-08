#ifndef VIEWPRO_H
#define VIEWPRO_H

#include <QWidget>
#include <QGridLayout>
#include <QFormLayout>
#include <QDebug>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTabWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QList>
#include "project.h"
#include "formgetemptycross.h"
#include "viewcross.h"
#include "setemptydata.h"
#include "formgetdbcross.h"
#include "formgetdbxctrl.h"
#include "readcsvfile.h"
#include "setdbdata.h"
#include "setcsvdata.h"
#include "sethourdata.h"
#include "../xctrl/viewxctrl.h"
#include "../xctrl/dialogaddemptyxt.h"
#include "../xctrl/readjsonfile.h"
extern Setup ini;
class ViewPro : public QWidget
{
    Q_OBJECT
public:
    explicit ViewPro(QWidget *parent = nullptr,Project *project=nullptr);

public slots:
    void moveData();
    void loadTable();
signals:
    void changeData(QString name);
public:
    void addCross(int flag);
    void delCross();
    void addData(int flag);
    void exportCrossDataToCSV();
    void repairDataCross();
    void AddXtEmpty();
    void AddXtFromJSON();
    void AddXtFromBD();
    void SaveXTToBD();
    void DeleteXt();
    void SaveToJson();
private:
    void appendTabCross();
    void top();
    bool isCross();
    bool isXT();
    void table();
    void resultTable();
    void extTable();
    int playKC(QVector<int> ir);
    Project *project;
    QWidget *parent;
    //Редактируемые поля
    QLineEdit *lregion;         //Регион
    QLineEdit *larea;           //Район
    QLineEdit *lsubarea;        //Подрайон
    QLineEdit *ldesc;           //Описание проекта
    QLineEdit *lstepdev;           //Интервал времени
    QLineEdit *lstepxt;           //Интервал времени
    QLineEdit *lchanels;        //колличество каналов статистики
    QGroupBox *gstyle;
    QRadioButton *garea;
    QRadioButton *gbox;
    QGridLayout *grid;
    QWidget *wtop;              //Виджет редактирования параметров
    QTabWidget *tab;
    QTabWidget *restab;
    QTableWidget *ptable;
    QTableWidget *etable;
};

#endif // VIEWPRO_H
