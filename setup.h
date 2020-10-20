#ifndef SETUP_H
#define SETUP_H

#include <QString>
#include <QVariant>
#include <QFile>
#include <QApplication>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QMutex>

class Setup
{
public:
    Setup(QString fullPathFile);
    //Getters
    int getInt(QString name);
    QString getString(QString name);
    bool getBool(QString name);
    float getFloat(QString name);
    QSize getSize(QString name);
    QUrl  getUrl(QString name);
    QDate getDate(QString name);
    QPoint getPoint(QString name);
    //Setters
    void setInt(QString name,int value);
    void setString(QString name,QString value);
    void setBool(QString name,bool value);
    void setFloat(QString name,float value);
    void setSize(QString name,QSize value);
    void setUrl(QString name,QUrl value);
    void setDate(QString name,QDate value);
    void setPoint(QString name,QPoint value);

private:
    QMap<QString,QVariant> vars;
    QFile file;
    void saveSetup();
    QVariant get(QString name,QVariant def);
    void set(QString name,QVariant value);
    QMutex mutex;

};

#endif // SETUP_H
