#ifndef SUPPORT_H
#define SUPPORT_H

#include <QtGlobal>
#include <QApplication>
#include <QString>
#include <QErrorMessage>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMap>
#include <QList>
#include <QtSql>
#include <QMessageBox>
#include <QRandomGenerator>
#include "stat/archstat.h"
class Support
{

public:
    static int RandomBetween(int low, int high);
    static int RandomBetween(int low, int high, int seed);
    static void ErrorMessage(QString message);
    static void Message(QString message);
    static QStringList listCrosses(int region,int area);
    static QMap<int,QList<QDate>> listStatistics(int region,int area);
    static QString getArchStat(int region,int area,int id,QDate date);
    static QList<QVector<int>> Summator(QList<QVector<int>> matrix,int oldstep,int newstep);
    static QList<QString> getElite(int step);
    static bool isVerStep(int step);
    static QString CorrectNameFile(QString namefile,QString list);
private:
    static bool compare(int time,int ntime,int step);
};


#endif // SUPPORT_H
