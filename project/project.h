#ifndef PROJECT_H
#define PROJECT_H

#include <QString>
#include <QFile>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QDebug>
#include "cross.h"
#include "../xctrl/xctrl.h"

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
    void setNotChanged();
    QString ToJSON();
    QString getDesc();
    QString getNameFile();
    void ChangeNameFile(QString nameFile);
    QString error;

    int Region=1;
    int Area=1;
    int SubArea=1;
    int Step=5;
    int Chanels=16;
    bool isChanged;
    QString description="";
    QList<Cross *> crosses;
    QList<Xctrl *> xctrls;
private:
    bool isEmpty=true;
    QString nameFile="";
};

#endif // PROJECT_H
