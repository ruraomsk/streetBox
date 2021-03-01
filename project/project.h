#ifndef PROJECT_H
#define PROJECT_H

#include <QString>
#include <QFile>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QDebug>
#include <QJsonValue>
#include "cross.h"
#include "../xctrl/state.h"
#include "../xctrl/xctrl.h"
#include "../xctrl/prioryty.h"
#include "../support.h"
#include "../common.h"
//Хранилище проекта
class Project
{
public:
    Project();
    Project(QString nameFile);
    Project(QString nameFile,QString description);
    ~Project();
    bool saveProject();
    bool IsEmpty();
    void appendCross(Cross *cross);
    void updateFromState(State *state);
    State *makeState();
    void setNotChanged();
    QString ToJSON();
    QString getDesc();
    QString getNameFile();
    void ChangeNameFile(QString nameFile);
    QString error;
    QString extToJson();

    int Region=1;
    int Area=1;
    int SubArea=1;
    int StepDevice=5;   //Интервал сбора данных перекрестком
    int StepXT=15;      //Интервал расчета ХТ
    int Chanels=16;
    bool isChanged;
    bool Switch;
    bool Release;

    QString description="";
    QList<Cross *> crosses;
    QList<Xctrl *> xctrls;
    Prioryty prioryty;
    bool UseStrategy=false;             //true выполняем стратегию Лучи  иначе стратегия Области
    int external[12][2]={{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}};

private:
    bool isEmpty=true;
    QString nameFile="";
};

#endif // PROJECT_H
