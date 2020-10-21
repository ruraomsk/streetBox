#include "voronoi.h"


Voronoi::Voronoi()
{

}

Voronoi::Voronoi(int height,int width)
{
    delH=1.0;
    delW=1.0;
    this->width_=width;
    this->height_=height;
    this->width=width;
    this->height=height;
    if(width!=MAX_W) {
        delW=(float)MAX_W/(float)width;
        this->width_=MAX_W;

    }
    if(height!=MAX_H) {
        delH=(float)MAX_H/(float)height;
        this->height_=MAX_H;

    }
    planMap=new PlanMap(this->width_,this->height_);
    sprayes.clear();
    nameSprayes.clear();
}

Voronoi::~Voronoi()
{
//    delete planMap;
}
int Voronoi::RandomBetween(int low, int high)
{
    return (QRandomGenerator::global()->generate() % ((high + 1) - low) + low);
}

void Voronoi::addSet(Strategy st)
{
    Point p(st.L,st.R);
    p.x=p.x*delW;
    p.y=p.y*delH;
    points.append(p);
    st.L=st.L*delH;
    st.R=st.R*delW;
    sts.append(st);
}
void Voronoi::addSpray(Point point,QString name)
{
    point.x=point.x*delW;
    point.y=point.y*delH;
    sprayes.append(point);
    nameSprayes.append(name);
    if(sprayes.size()!=nameSprayes.size()){
        qDebug()<<sprayes.size()<<nameSprayes.size();
    }
}

void Voronoi::clearSpray()
{
    sprayes.clear();
}

void Voronoi::makeDiagramm()
{
//    int w=width;
//    int h=height;
    std::sort(sts.begin(),sts.end(),Strategy::Compare);

    for (int hh = 1; hh < height_; ++hh) {
        for (int ww = 1; ww < width_; ++ww) {
            foreach (auto s, sts) {
                if(ww<=s.R&&hh<=s.L){
                    int plan=s.PlanM;
                    float f=((float)ww/(float)hh);
                    if(f<=s.Fl) plan=s.PlanL;
                    if(f>=s.Fr) plan=s.PlanR;
//                    qDebug()<<plan<<f;
                    planMap->SetPlan(ww,hh,plan);
                    break;
                }
            }
        }
    }
}

uint8_t Voronoi::GetPlan(int w, int h)
{

    return planMap->get(w/delW,h/delH);
}

QVector<Point> Voronoi::CreatePoints(int count)
{
    QVector<Point> points;
    for (int i = 0; i < count; i++) {
        points.push_back({ RandomBetween(0,width_) , RandomBetween(0,height_) });
    }
    return points;
}


PlanMap::PlanMap()
{

}

PlanMap::PlanMap(int w, int h)
{
    width_=w;
    height_=h;

    QVector<uint8_t> t(h,1);
    for (int i = 0; i < w; ++i) {
        map.append(t);
    }
}

PlanMap::~PlanMap()
{
//    map.clear();
}

void PlanMap::SetPlan(int w, int h, uint8_t plan)
{
    if(w>width_) return;
    if(h>height_) return;
    map[w][h]=plan;
}

uint8_t PlanMap::get(int w, int h)
{
    if(w>width_) return 0;
    if(h>height_) return 0;
    return map[w][h];

}

Point::Point(int x, int y)
{
    this->x=y;
    this->y=x;
    x_=x;
    y_=y;
}


bool Point::near(int rx, int ry)
{
//    qDebug()<<x_<<y_<<rx<<ry<<(x_>=(rx-10) && (x_<=(rx-10)) && y_>=(ry-10) &&(y_<=(ry+10)));
    return (x_>=(rx-10) && (x_<=(rx+10)) && y_>=(ry-10) &&(y_<=(ry+10)));
}

