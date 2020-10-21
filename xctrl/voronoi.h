#ifndef VORONOI_H
#define VORONOI_H
#include <QVector>
#include <QDebug>
#include <QRandomGenerator>

#include "xctrl.h"
#define MAX_W 1000
#define MAX_H 1000

class PlanMap
{
public:
    PlanMap();
    PlanMap(int w,int h);
    ~PlanMap();
    void SetPlan(int x,int y,uint8_t planl);
    int width(){return width_;}
    int height(){return height_;}
    uint8_t get(int x,int y);
    QVector<QVector<uint8_t>> map;
private:
    int width_,height_;
};

class Point{
public:
    Point(int x, int y);
    bool near(int x, int y);

    int x,y;
    int x_,y_;
};

class Voronoi
{
public:
    Voronoi();
    Voronoi(int width,int height);
    ~Voronoi();
    int RandomBetween(int low, int high);
    void addSet(Strategy st);
    void addSpray(Point point,QString name);
    void clearSpray();
    void makeDiagramm();
    uint8_t GetPlan(int w,int h);
    QList<Strategy> sts;
    QVector<Point>  CreatePoints(int count);
    QVector<Point> points;
    QVector<Point> coords;
    QVector<Point> sprayes;
    QVector<QString> nameSprayes;
    PlanMap *planMap;
    float delW;
    float delH;

private:
    int width;
    int height;
    int width_;
    int height_;


};

#endif // VORONOI_H
