#ifndef ARCHSTAT_H
#define ARCHSTAT_H

#include <QList>
#include <QDate>
#include <QJsonDocument>
#include <QJsonParseError>

#include "support.h"

class DataStat{
public:
    DataStat(int chanel,int status,int intensiv);
    DataStat(QMap<QString, QVariant> map);
    QString ToJSON();
    QString ToCSV();
    static bool Compare(const DataStat &data1,const DataStat &data2);
    int Chanel;     //Номер канала
    int Status;     // Состояние 0-исправен 1-обрыв 2 - замыкание
    int Intensiv;   //Интенсивность
private:

};

class Statistic{
public:
    Statistic(int period,int type,int len,int hour,int min);
    Statistic(QMap<QString, QVariant> map);
    void AddData(DataStat data);
    QString ToJSON();
    QString ToCSV();
    int getTimeStart();
    void FromVector(QVector<int> vector);
    static bool Compare(Statistic &stat1,Statistic &stat2);
    QVector<int> ToVector();
//    void SortData();
private:
    int Period;                     //Номер периода усреднения от начала суток
    int Type;                       //Тип статистики 1-интенсивность скорость
    int TLen;                       //Величина времения усреднения мин
    int Hour;                       //Час окончания периода
    int Min;                        //Минуты окончания периода
    QList<DataStat> Datas;          //собственно данные
};

class ArchStat
{
public:
    ArchStat(int region,int area,QDate date);
    ArchStat(QString json);
    ArchStat(QMap<QString, QVariant>  map);
    ArchStat();

    void AddStatistic(Statistic stat);
    void DropStat();
    QString ToJSON();
    QString ToCSV();
    QList<QVector<int>> ToMatrix();
    void FromMatrix(QList<QVector<int>> matrix);
//    static bool Compare(ArchStat *archstat1,ArchStat *archstat2);
    QString error ;
//    void SortData();
    QDate Date;                     //Дата
    QString Comment;                //Коментарий специалиста
    QList<Statistic> Statistics;    //Накопленная статистика
    int Region;                     //Регион
    int Area;                       //Район
    int ID;                         //Номер перекрестка
private:
};

#endif // ARCHSTAT_H
