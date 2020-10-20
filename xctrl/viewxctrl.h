#ifndef VIEWXCTRL_H
#define VIEWXCTRL_H

#include <QObject>
#include <QWidget>
#include <QLineEdit>
#include <QTabWidget>
#include <QGridLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QScrollArea>

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

#include "../project/project.h"
#include "../setup.h"
#include "viewpoints.h"
#include "viewstrategy.h"
#include "viewcalculate.h"
#include "viewvoronoi.h"
#include "voronoi.h"

extern Setup ini;

QT_CHARTS_USE_NAMESPACE

class ViewXctrl : public QWidget
{
    Q_OBJECT
public:
    explicit ViewXctrl(Project *project,Xctrl *xctrl,QWidget *parent = nullptr);
    QString getName();
    void SaveToJSON(QString namefile);
signals:

private slots:
    void ChangeXT();
    void updated();
    void addSpray();
    void clearSpray();
public:

private:
    void top();
//    void graph();

    Project *project;
    Xctrl *xctrl;
    QLineEdit *lregion;         //Регион
    QLineEdit *larea;           //Район
    QLineEdit *lsubarea;        //Подрайон
    QLineEdit *lname;           //Описание XT
    QLineEdit *lstep;           //Интервал времени
    QLineEdit *lLeftRel;        //Отношение для прямого направления
    QLineEdit *lRightRel;        //Отношение для обратного направления

    QWidget *wtop;              //Виджет редактирования параметров
//    QWidget *wgraph;
    QWidget *strateg;
    QWidget *points;
    ViewVoronoi *voroni;
    Voronoi *vor;

    QTabWidget *tab;            //основной виджет редактора
    QGridLayout *grid;
    ViewStrategy *vstrategy;
    ViewPoints *vpoints;
    ViewCalculate *vcalc;

};

#endif // VIEWXCTRL_H
