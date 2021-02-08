#ifndef PRIORYTY_H
#define PRIORYTY_H

#include <QMap>
#include <QString>
#include <QVariant>
#include <QJsonArray>
class Prioryty
{
public:
    Prioryty();

    void FromJSON (QJsonArray array);
    QString ToJSON();

    int rows=4;
    int columns=3;
    int prior[4][3]={{8,7,9},{5,4,6},{2,1,3},{0,0,0}};



};

#endif // PRIORYTY_H
