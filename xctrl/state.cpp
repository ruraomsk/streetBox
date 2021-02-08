#include "state.h"

State::State()
{

}

State::State(QMap<QString, QVariant> map)
{
    Region=map["region"].toInt();
    Area=map["area"].toInt();
    SubArea=map["subarea"].toInt();
    LastTime=QDateTime::fromString(map["ltime"].toString(),"yyyy-MM-ddThh:mm:ss.zzz");

    Step=map["step"].toInt();
    Remain=map["rem"].toInt();
    UseStrategy=map["use"].toBool();
    PKNow=map["pknow"].toInt();
    PKLast=map["pklast"].toInt();
    PKCalc=map["pkcalc"].toInt();
    foreach(auto s,map["xctrls"].toList()){
        xctrls.append(new Xctrl(s.toMap()));
    }
    prioryty.FromJSON(map["Prioryty"].toJsonArray());
    auto array=map["ext"].toJsonArray();
    for (int i = 0; i < 12; ++i) {
        for (int j = 0; j < 2; ++j) {
            external[i][j]=array.at(i).toArray().at(j).toInt();
        }
    }

}



QString State::extToJson()
{
    QString result;
    //    result.append("{");
    result.append("\"ext\":[");
    for (int i = 0; i < 12; ++i) {
        result.append("[");
        for (int j = 0; j < 2; ++j) {
            result.append(QString::number(external[i][j]));
            result.append(",");

        }
        result.remove(result.length()-1,1);
        result.append("],");
    }
    result.remove(result.length()-1,1);
    result.append("]");
    return result;

}

QString State::ToJSON()
{
    QString result;
    result.append(QString::asprintf("{\"region\":%d,\"area\":%d,\"subarea\":%d,",Region,Area,SubArea));
    result.append(QString::asprintf("\"switch\":%s,\"release\":%s,\"step\":%d,\"rem\":%d,",Switch?"true":"false",Release?"true":"false",Step,Remain));
    result.append(QString::asprintf("\"use\":%s,",UseStrategy?"true":"false"));
    result.append(QString::asprintf("\"pkcalc\":%d,\"pknow\":%d,\"pklast\":%d,",PKCalc,PKNow,PKLast));
    result.append("\"ltime\":\""+LastTime.toString("yyyy-MM-ddThh:mm:ss.zzz")+"\",");

    result.append(prioryty.ToJSON()+",");
    result.append(extToJson()+",");
    result.append("\"xctrls\":[");
    if (xctrls.size()>0){
        foreach (auto var, xctrls) {
            result.append(var->ToJSON());
            result.append(",");
        }
        result.remove(result.length()-1,1);
    }
    result.append("]");
    result.append("}");
    return result;

}

