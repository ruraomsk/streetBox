#ifndef BOGKO_H
#define BOGKO_H
//Пересчитываем нашу таблицу в таблицу другую
#include "xctrl.h"
//Отсюда нам нужен Point
#include "voronoi.h"
#include "../support.h"

class Bogko
{
public:
    Bogko(QList<Strategy> strates);

    QVector<Point> centrals; //Результат новых центров областей
    bool error;
private:
    QList<Strategy> strates; //Наша таблица

};

#endif // BOGKO_H
