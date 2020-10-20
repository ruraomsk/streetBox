#ifndef CROSS_H
#define CROSS_H
#include <QMap>
#include <QDate>
#include <QFile>
#include "stat/archstat.h"
class Cross
{
public:
    Cross(QMap<QString, QVariant> map);
    Cross(int number,QString description);
    void Attach(ArchStat arch);
    void SaveCrossToCSV(QString nameFile);
    void SetData(QDate date,bool rnd);         //Добавляет статистику с нулевыми или случайными значениями
    void setDataFromMatrix(QDate date,QList<QVector<int>> matrix);
    QList<QVector<int>> getMatrix(QString comment);
    QList<QVector<int>> getMatrix(QDate date);
    QStringList getComments();
    bool IsEmpty();
    QString ToJSON();
    QString ToCSV();
    int Number;
    QString Description;
    int Region;
    int Area;
    int SubArea;
    int Step;
    int Chanels;
    bool Rnd;
    QDate now;          //Текущая отображамемая дата
    bool isEmpty=true;
    bool summ=false;    //Если суммирование то нельзя редактировать
    int SummInreval=60;                    //Новый интервал суммирования
    QMap<QDate,ArchStat> archs;
private:
};

#endif // CROSS_H
