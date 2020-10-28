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
    text->setText("Координаты");
    update();
}

void ViewVoronoi::paintEvent(QPaintEvent *)
{
    QPainter *painter = new QPainter(this);
    QPen pen;
    pen.setWidth(1);
//    for(int x=0;x<v->planMap->width();x++){
//        for (int y = 0; y < v->planMap->height(); ++y) {
//            pen.setColor(qRgb(255,255,255));
//            painter->setPen(pen);
//            painter->drawPoint(QPoint(x,v->planMap->height()-y));
//        }
//    }
    painter->translate(QPoint(0,0));
    for(int w=0;w<v->planMap->width();w++){
        for (int h = 0; h < v->planMap->height(); ++h) {
                pen.setColor(color[v->planMap->get(w,h)]);
            painter->setPen(pen);
            painter->drawPoint(QPoint(w,v->planMap->height()-h));
//            qDebug()<<x<<y<<color[v->planMap->get(x,y)];
        }
    }
    painter->setPen(qRgb(0,0,0));
    painter->setBrush(QBrush(Qt::black,Qt::SolidPattern));
    foreach (auto p, v->points) {
        int w=p.w,h=v->planMap->height()-p.h;
//        qDebug()<<x<<y<<p.x<<p.y<<p.x_<<p.y_;
        painter->setFont(QFont("san-serif",-1,10));
        QString ww=QString::asprintf("(%d,%d)",p.h_,p.w_);
        painter->drawText(QPoint(w-23,h),ww);
        painter->drawEllipse(w,h,4,4);
    }
    foreach (auto p, v->bogoses) {
        int w=p.w,h=v->planMap->height()-p.h;
//        qDebug()<<x<<y<<p.x<<p.y<<p.x_<<p.y_;
        painter->setFont(QFont("san-serif",-1,10));
        QString ww=QString::asprintf("(%d,%d)",p.h_,p.w_);
        painter->drawText(QPoint(w-23,h),ww);
        painter->drawEllipse(w,h,4,4);
    }
    foreach (auto p, v->sprayes) {
        int w=p.w,h=v->planMap->height()-p.h;
        painter->drawEllipse(w,h,3,3);
    }

    painter->end();
}

void ViewVoronoi::mouseMoveEvent(QMouseEvent *event)
{
    int w=event->pos().x();
    int h=v->planMap->height()-event->pos().y();
    if(w>0&&w<v->planMap->width() &&h>0&&h<v->planMap->height()){
        h=(int)((float)h)/v->delH;
        w=(int)((float)w)/v->delW;
        text->setText(QVariant(h).toString()+":"+QVariant(w).toString());
        int i=0;
        if(v->sprayes.size()!=v->nameSprayes.size()){
//            qDebug()<<v->sprayes.size()<<v->nameSprayes.size();
        }

        foreach (Point p, v->sprayes) {
            if(p.near(w,h)){
                text->setText(QVariant(p.h_).toString()+":"+QVariant(p.w_).toString()+"->"+v->nameSprayes[i]);
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
    color[0]=qRgb(255,255,255);
    if(v->sts.size()!=0){
        foreach (auto s,v->sts ) {
            color[s.PlanL]=(qRgb(rand() % 200 + 50, rand() % 200 + 55, rand() % 200 + 50));
            color[s.PlanR]=(qRgb(rand() % 200 + 50, rand() % 200 + 55, rand() % 200 + 50));
            color[s.PlanM]=(qRgb(rand() % 200 + 50, rand() % 200 + 55, rand() % 200 + 50));
        }
    }
    if(v->plans.size()!=0){
        foreach (auto p, v->plans) {
            color[p]=(qRgb(rand() % 200 + 50, rand() % 200 + 55, rand() % 200 + 50));
        }
    }
}




