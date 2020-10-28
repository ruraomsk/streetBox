#ifndef XTCALCULATE_H
#define XTCALCULATE_H
#include <QtGlobal>
#include <QDebug>
#include <QString>

#include "project/project.h"
#include "xctrl.h"

class XTMatrix
{
public:
    XTMatrix();
    XTMatrix(QList<QVector<int>> matrix);
    int getFlow(QList<int>chanels,int beginTime,int endTime);
    void adaptHour(int step);
private:
    QList<QVector<int>> matrix;
};
class XTCalculate
{
public:
    XTCalculate(Project *project,Xctrl *xctrl,QString comment);
    XTCalculate(Project *project,Xctrl *xctrl,QDate date);
    XTCalculate(Project *project,Xctrl *xctrl); // Работаем по выставленным значениям
    void calculate();
    void calcAreal();
    bool error=false;
    QStringList protocol;
    QList<QVector<QString>> fin;
private:
    QList<int> setListDU();
    void controlStep();
    QMap<int,XTMatrix> dus;
    Project *project;
    Xctrl *xctrl;
    QString getDescription(int plan);
};

#endif // XTCALCULATE_H
