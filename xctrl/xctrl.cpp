#include "xctrl.h"


Calc::Calc(int region, int area, int id)
{
    Region=region;
    Area=area;
    ID=id;
}

Calc::Calc(QMap<QString, QVariant> map)
{
    Region=map["region"].toInt();
    Area=map["area"].toInt();
    ID=map["id"].toInt();
    foreach (auto l, map["chanL"].toList()) {
        ChanL.append(l.toInt());
    }
    foreach (auto r, map["chanR"].toList()) {
        ChanR.append(r.toInt());
    }

}

QString Calc::ToJSON()
{
    QString result;
    result.append(QString::asprintf("{\"region\":%d,\"area\":%d,\"id\":%d,\"chanL\":[",Region,Area,ID));
    if(ChanL.size()>0){
        foreach (auto var, ChanL) {
            result.append(QString::asprintf("%d",var));
            result.append(",");
        }
        result.remove(result.length()-1,1);
    }
    result.append("],\"chanR\":[");
    if(ChanR.size()>0){
        foreach (auto var, ChanR) {
            result.append(QString::asprintf("%d",var));
            result.append(",");
        }
        result.remove(result.length()-1,1);
    }
    result.append("]}");
    return result;
}


Xctrl::Xctrl()
{

}

Xctrl::Xctrl(QMap<QString, QVariant> map)
{
    Left=map["left"].toFloat();
    Right=map["right"].toFloat();
    name=map["name"].toString();
    foreach(auto s,map["StrategyB"].toList()){
        Strategys.append(Strategy(s.toMap()));
    }
    foreach(auto s,map["StrategyA"].toList()){
        Areals.append(Areal(s.toMap()));
    }
    foreach(auto s,map["Calculates"].toList()){
        Calculates.append(Calc(s.toMap()));
    }
}

QString Xctrl::ToJSON()
{
    QString result;
//    result.append("\"xctrl\":");
    result.append(QString::asprintf("{\"left\":%d,\"right\":%d,\"status\":[",Left,Right));
    if (Status.size()>0){
        foreach (QString var, Status) {
            result.append("\"");
            result.append(var);
            result.append("\",");
        }
        result.remove(result.length()-1,1);
    }
    result.append("],\"name\":\""+name+"\",");

    result.append("\"StrategyB\":[");
    if (Strategys.size()>0){
        foreach (auto var, Strategys) {
            result.append(var.ToJSON());
            result.append(",");
        }
        result.remove(result.length()-1,1);
    }
    result.append("],");
    result.append("\"StrategyA\":[");
    if (Areals.size()>0){
        foreach (auto var, Areals) {
            result.append(var.ToJSON());
            result.append(",");
        }
        result.remove(result.length()-1,1);
    }
    result.append("],");
    result.append("\"Calculates\":[");
    if (Calculates.size()>0){
        foreach (auto var, Calculates) {
            result.append(var.ToJSON());
            result.append(",");
        }
        result.remove(result.length()-1,1);
    }
    result.append("]}");
    return result;
}

void Xctrl::AddStrategy(Strategy strat)
{
    Strategys.append(strat);
}

void Xctrl::AddAreal(Areal areal)
{
    Areals.append(areal);
}

void Xctrl::AddCalc(Calc calc)
{
    Calculates.append(calc);
}

void Xctrl::AddStatus(QString status)
{
    Status.append(status);
}


Strategy::Strategy(int l, int r, int planl, int planm, int planr, float fl, float fr, QString description)
{
    L=l;
    R=r;
    PlanL=planl;
    PlanM=planm;
    PlanR=planr;
    Fl=fl;
    Fr=fr;
    Description=description;

}

Strategy::Strategy(QMap<QString, QVariant> map)
{
    L=map["xleft"].toInt();
    R=map["xright"].toInt();
    PlanL=map["pkl"].toInt();
    PlanR=map["pkr"].toInt();
    PlanM=map["pks"].toInt();
    Fl=map["vleft"].toFloat();
    Fr=map["vright"].toFloat();
    Description=map["desc"].toString();
}

bool Strategy::Compare(Strategy &st1, Strategy &st2)
{
    return st1.R<st2.R;
}

QString Strategy::ToJSON()
{
    QString result;
    result.append(QString::asprintf("{\"xleft\":%d,\"xright\":%d,\"pkl\":%d,\"pkr\":%d,\"pks\":%d,\"vleft\":%f,\"vright\":%f,\"desc\":\"",L,R,PlanL,PlanR,PlanM,Fl,Fr));
    result.append(Description+"\"}");
    return result;
}

Areal::Areal(int l, int r, int plan, QString description)
{
    L=l;
    R=r;
    Plan=plan;
    Description=description;

}

Areal::Areal(QMap<QString, QVariant> map)
{
    L=map["xleft"].toInt();
    R=map["xright"].toInt();
    Plan=map["pk"].toInt();
    Description=map["desc"].toString();
}

QString Areal::ToJSON()
{
    QString result;
    result.append(QString::asprintf("{\"xleft\":%d,\"xright\":%d,\"pk\":%d,\"desc\":\"",L,R,Plan));
    result.append(Description+"\"}");
    return result;
}

bool Areal::Compare(Areal &ar1, Areal &ar2)
{
    return ar1.L<ar2.L;
}
