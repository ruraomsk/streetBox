#ifndef VIEWVORONOI_H
#define VIEWVORONOI_H

#include <QColor>
#include <QMap>
#include <QObject>
#include <QPainter>
#include <QVector>
#include <QWidget>
#include <QPen>
#include <QLabel>
#include <QtGlobal>

#include "voronoi.h"
class ViewVoronoi : public QWidget
{
    Q_OBJECT
public:
public:
    explicit ViewVoronoi(Voronoi *v,QWidget *parent = nullptr);
    QLabel *text;

protected:
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *event);
signals:


private:
    void CreateColor();
    void CreateSitesPoint();
    void makeAllSites();
    QWidget *parent;
    Voronoi *v;
    QMap<uint8_t,QColor> color;

};

#endif // VIEWVORONOI_H
