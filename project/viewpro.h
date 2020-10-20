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

#include "project.h"
#include "formgetemptycross.h"
#include "viewcross.h"
#include "setemptydata.h"
#include "formgetdbcross.h"
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
    void DeleteXt();
    void SaveToJson();
private:
    void appendTabCross();
    void top();
    bool isCross();
    bool isXT();
    Project *project;
    QWidget *parent;
    //Редактируемые поля
    QLineEdit *lregion;         //Регион
    QLineEdit *larea;           //Район
    QLineEdit *lsubarea;        //Подрайон
    QLineEdit *ldesc;           //Описание проекта
    QLineEdit *lstep;           //Интервал времени
    QLineEdit *lchanels;        //колличество каналов статистики

    QWidget *wtop;              //Виджет редактирования параметров
    QTabWidget *tab;            //основной виджет редактора
};

#endif // VIEWPRO_H
