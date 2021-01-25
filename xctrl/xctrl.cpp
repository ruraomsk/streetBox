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

Xctrl::Xctrl(int region, int area, int subarea)
{
    Region=region;
    Area=area;
    SubArea=subarea;
    LastTime=QDateTime::currentDateTime();
}

Xctrl::Xctrl(QMap<QString, QVariant> map)
{
//    QJsonParseError jError;
//    QJsonDocument jdoc=QJsonDocument::fromJson(json.toUtf8(),&jError);
//    if (jError.error!=QJsonParseError::NoError){
//        error=jError.errorString();
//        return;
//    }
//    QMap<QString, QVariant>  map=jdoc.toVariant().toMap();
    Region=map["region"].toInt();
    Area=map["area"].toInt();
    SubArea=map["subarea"].toInt();
    Step=map["step"].toInt();
    Remain=map["rem"].toInt();
    Switch=map["switch"].toBool();
    Release=map["release"].toBool();
    UseStrategy=map["use"].toBool();
    PKNow=map["pknow"].toInt();
    PKLast=map["pklast"].toInt();
    PKCalc=map["pkcalc"].toInt();
    Left=map["left"].toFloat();
    Right=map["right"].toFloat();
    name=map["name"].toString();
    LastTime=QDateTime::fromString(map["ltime"].toString(),"yyyy-MM-ddThh:mm:ss.zzz");
    foreach (auto s, map["status"].toList()) {
        Status.append(s.toString());
    }
    foreach (auto r, map["Results"].toList()) {
        Results.append(Result(r.toMap()));
    }
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
    result.append(QString::asprintf("{\"region\":%d,\"area\":%d,\"subarea\":%d,",Region,Area,SubArea));
    result.append(QString::asprintf("\"switch\":%s,\"release\":%s,\"step\":%d,\"rem\":%d,",Switch?"true":"false",Release?"true":"false",Step,Remain));
    result.append(QString::asprintf("\"use\":%s,",UseStrategy?"true":"false"));
    result.append(QString::asprintf("\"pkcalc\":%d,\"pknow\":%d,\"pklast\":%d,",PKCalc,PKNow,PKLast));
    result.append(QString::asprintf("\"left\":%d,\"right\":%d,\"status\":[",Left,Right));
    if (Status.size()>0){
        foreach (QString var, Status) {
            result.append("\"");
            result.append(var);
            result.append("\",");
        }
        result.remove(result.length()-1,1);
    }
    result.append("],\"name\":\""+name+"\",");
    result.append("\"ltime\":\""+LastTime.toString("yyyy-MM-ddThh:mm:ss.zzz")+"\",");
    result.append("\"Results\":[");
    if (Results.size()>0){
        foreach (auto var, Results) {
            result.append(var.ToJSON());
            result.append(",");
        }
        result.remove(result.length()-1,1);
    }
    result.append("],");
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

void Xctrl::AddResult(Result result)
{
    Results.append(result);
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

Result::Result(int il, int ir)
{
    Ileft=il;
    Iright=ir;
}

Result::Result(QMap<QString, QVariant> map)
{
    Ileft=map["il"].toInt();
    Iright=map["ir"].toInt();
}

QString Result::ToJSON()
{
    QString result;
    result.append(QString::asprintf("{\"il\":%d,\"ir\":%d}",Ileft,Iright));
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
