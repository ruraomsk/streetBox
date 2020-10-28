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
    bogoses.clear();
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
    Point p(st.R,st.L);
    p.w=p.w*delW;
    p.h=p.h*delH;
    points.append(p);
    st.L=st.L*delH;
    st.R=st.R*delW;
    sts.append(st);
}

void Voronoi::addAreal(Areal ar)
{
    Point p(ar.R,ar.L);
    p.w=p.w*delW;
    p.h=p.h*delH;
    points.append(p);
    plans.append(ar.Plan);
}

void Voronoi::addBogoses(QVector<Point> bogs)
{
    bogoses.clear();
    foreach(Point p,bogs){
        p.w=p.w*delW;
        p.h=p.h*delH;
        bogoses.append(p);
    }
}
void Voronoi::addSpray(Point point,QString name)
{
    point.w=point.w*delW;
    point.h=point.h*delH;
    sprayes.append(point);
    nameSprayes.append(name);
    if(sprayes.size()!=nameSprayes.size()){
//        qDebug()<<sprayes.size()<<nameSprayes.size();
    }
}

void Voronoi::clearSpray()
{
    sprayes.clear();
    nameSprayes.clear();
    bogoses.clear();
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

void Voronoi::makeAreals()
{
    ulong d;
    for (int hh = 0; hh < height; ++hh) {
        for (int ww = 0; ww < width; ++ww) {
            int ind=-1;
            ulong dist=__INT_MAX__;
            for (int it=0;it<points.size();++it){
                d=points[it].DistanceSqrd(ww,hh);
                if (d<dist){
                    dist=d;
                    ind=it;
                }
            }
            if(ind>-1){
                planMap->SetPlan(ww,hh,plans[ind]);
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
    width_=MAX_W;
    height_=MAX_H;

    QVector<uint8_t> t(MAX_H,0);
    for (int i = 0; i < MAX_W; ++i) {
        map.append(t);
    }

}

PlanMap::PlanMap(int w, int h)
{
    width_=w;
    height_=h;

    QVector<uint8_t> t(h,0);
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

Point::Point(int w, int h)
{
    this->w=w;
    this->h=h;
    w_=w;
    h_=h;
}


bool Point::near(int rw, int rh)
{
//    qDebug()<<x_<<y_<<rx<<ry<<(x_>=(rx-10) && (x_<=(rx-10)) && y_>=(ry-10) &&(y_<=(ry+10)));
    return (w_>=(rw-10) && (w_<=(rw+10)) && h_>=(rh-10) &&(h_<=(rh+10)));
}

Point Point::mediana(int nw, int nh)
{
    Point p((nw-w)/2,(nh-h)/2);
    return p;
}

ulong Point::DistanceSqrd(int w, int h)
{
    int wd=w-this->w;
    int hd=h-this->h;
    return ((ulong)wd*(ulong)wd)+((ulong)hd*(ulong)hd);
}

Point Point::summ(int nw, int nh,int bw,int bh)
{
    float f=(float)bh/(float)bw;
//        qDebug()<<bh<<bw<<f;
    Point p(((float)(nw-w)/f)+nw,f*((float)(nh-h))+nh);
//    qDebug()<<bh<<bw<<f;
    return p;
}

Point Point::simetr(float f)
{
    //Поиск точки симметричной оносительно прямой h=(1/f)w+0
    // Считаем коэффициент к
//    qDebug()<<"------------------------------";
//    qDebug()<<h<<w;

    float a1=1.0/f;
    float b1=0.0;
//    qDebug()<<a1<<b1;
    float a2=-(1/a1);
    //Считаем b
    float b2=h-a2*w;
//    qDebug()<<a2<<b2;
    //Теперь мы знаем прямую перпендикулярную
    //Первая координата
    int w1=(int)((b2-b1)/(a1-a2));
    int h1=(int)((1.0/f)*(float)w1);
//    qDebug()<<h1<<w1;

    int w2=2*w1-w;
    int h2=2*h1-h;
//    qDebug()<<h2<<w2;

    Point p(w2,h2);
    return p;
}

bool Point::isGood()
{
    return !(w<=0 ||h<=0);
}

