#ifndef SUMGRAPH_H
#define SUMGRAPH_H

#include <QWidget>
#include <QDate>
#include <QMap>
#include <QObject>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCore/QDateTime>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>
#include <QDateTime>
#include <QHBoxLayout>

#include "../setup.h"

QT_CHARTS_USE_NAMESPACE

extern Setup ini;
class DataGraph
{
public:
    DataGraph();
    DataGraph(QString comment);
    QChartView *getGraph();
    QVector<QVector<int>> datas;
    QString comment;
};

class SumGraph : public QWidget
{
public:
    SumGraph();
    void clearAll();
    void addData(DataGraph data);
private:
    QMap<QString,DataGraph> tables;
    QScrollArea *wgraph;
    QGridLayout *grid;
    void GraphRepaint();
};

#endif // SUMGRAPH_H
