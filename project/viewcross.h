#ifndef VIEWCROSS_H
#define VIEWCROSS_H

#include <QWidget>
#include <QGridLayout>
#include <QFormLayout>
#include <QDebug>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QComboBox>
#include <QDateEdit>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCore/QDateTime>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>
#include <QDateTime>
#include <QScrollArea>
#include <QCheckBox>
#include "cross.h"
#include "project.h"
#include "../support.h"
#include "../setup.h"
//#include "viewpro.h"

QT_CHARTS_USE_NAMESPACE

extern Setup ini;
class ViewCross : public QWidget
{
    Q_OBJECT
public:
//explicit
    ViewCross(Project *project,Cross *cross,QWidget *parent = nullptr);
    void Update();
protected:
//    virtual void update();
signals:
private slots:
    void Changed(const QString&);
    void changeComment();
    void saveTable(int row,int column);
    void itemCkliked();
    void summChanged();
    void deleteDate();
    void changeDate();
    void ChangedInterval(const QString&);
public:
    Cross *cross;
    QList<bool> checked;
    QList<QVector<int> > tableMatrix;
    void repairData();
    QDate getCurrentDate();
private:
    void head();
    void table();
    void graph();
    QWidget *whead;
    QTableWidget *wtable;
    QScrollArea *wgraph;
    QComboBox *combo;
    QLineEdit *lcomment;
    QLineEdit *ldecrib;
    QLineEdit *lstep;
    QLineEdit *lchanels;
    QCheckBox *lsumm;
    QComboBox *linterval;
    QGridLayout *grid;
    QPushButton *bnDelete;
    QPushButton *bnChangeDate;
    Project *project;
};

#endif // VIEWCROSS_H
