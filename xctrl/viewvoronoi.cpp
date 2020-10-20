#include "viewvoronoi.h"

#include <QMouseEvent>



ViewVoronoi::ViewVoronoi(Voronoi *v, QWidget *parent)
{
    this->parent=parent;
    this->v=v;
    setMouseTracking(true);
    setFixedSize(v->planMap->width(),v->planMap->height());
    CreateColor();
    text=new QLabel;
    update();
}

void ViewVoronoi::paintEvent(QPaintEvent *)
{
    QPainter *painter = new QPainter(this);
    QPen pen;
    pen.setWidth(1);
    painter->translate(QPoint(0,0));
    for(int x=0;x<v->planMap->width();x++){
        for (int y = 0; y < v->planMap->height(); ++y) {
            pen.setColor(color[v->planMap->get(x,y)]);
            painter->setPen(pen);
            painter->drawPoint(QPoint(x,v->planMap->height()-y));
//            qDebug()<<x<<y<<color[v->planMap->get(x,y)];
        }
    }
    painter->setPen(qRgb(0,0,0));
    painter->setBrush(QBrush(Qt::black,Qt::SolidPattern));
    foreach (auto p, v->points) {
        int x=p.x,y=v->planMap->height()-p.y;
//        qDebug()<<x<<y<<p.x<<p.y<<p.x_<<p.y_;
        painter->setFont(QFont("san-serif",-1,10));
        QString w=QString::asprintf("(%d,%d)",p.x_,p.y_);
        painter->drawText(QPoint(x-23,y),w);
        painter->drawEllipse(x,y,4,4);
    }
    foreach (auto p, v->sprayes) {
        int x=p.x,y=v->planMap->height()-p.y;
        painter->drawEllipse(x,y,3,3);
    }

    painter->end();
}

void ViewVoronoi::mouseMoveEvent(QMouseEvent *event)
{
    int x=event->pos().x();
    int y=600-event->pos().y();
    if(x>0&&x<800 &&y>0&&y<600){
        y=(int)((float)y)/v->delH;
        x=(int)((float)x)/v->delW;
        text->setText(QVariant(y).toString()+":"+QVariant(x).toString());
        int i=0;
        foreach (Point p, v->sprayes) {
            if(p.near(y,x)){
                text->setText(QVariant(p.x_).toString()+":"+QVariant(p.y_).toString()+"->"+v->nameSprayes[i]);
                break;
            }
            i++;
        }

    }else {
        text->setText(" ");
    }

}

void ViewVoronoi::CreateColor()
{
    foreach (auto s,v->sts ) {
        color[s.PlanL]=(qRgb(rand() % 200 + 50, rand() % 200 + 55, rand() % 200 + 50));
        color[s.PlanR]=(qRgb(rand() % 200 + 50, rand() % 200 + 55, rand() % 200 + 50));
        color[s.PlanM]=(qRgb(rand() % 200 + 50, rand() % 200 + 55, rand() % 200 + 50));
    }
}



