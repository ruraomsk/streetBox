#include "archstat.h"


DataStat::DataStat(int chanel, int status, int intensiv)
{
    this->Chanel=chanel;
    this->Status=status;
    this->Intensiv=intensiv;
}

DataStat::DataStat(QMap<QString, QVariant> map)
{
    Chanel=map["Chanel"].toInt();
    Status=map["Status"].toInt();
    Intensiv=map["Intensiv"].toInt();
}

QString DataStat::ToJSON()
{
    QString res;
    return res.asprintf("{\"Chanel\":%d,\"Status\":%d,\"Intensiv\":%d}",Chanel,Status,Intensiv);;
}

QString DataStat::ToCSV()
{
    QString res;
    return res.asprintf("%d",Intensiv);
}

bool DataStat::Compare(const DataStat &data1, const DataStat &data2)
{
    return data1.Chanel<data2.Chanel;
}

Statistic::Statistic(int period, int type, int len, int hour, int min)
{
    this->Period=period;
    this->Type=type;
    this->TLen=len;
    this->Hour=hour;
    this->Min=min;
}

Statistic::Statistic(QMap<QString, QVariant> map)
{
    Period=map["Period"].toInt();
    Type=map["Type"].toInt();
    TLen=map["TLen"].toInt();
    Hour=map["Hour"].toInt();
    Min=map["Min"].toInt();
    foreach (auto d, map["Datas"].toList()) {
        Datas.append(DataStat(d.toMap()));
    }
    std::sort(Datas.begin(),Datas.end(),DataStat::Compare);


}

void Statistic::AddData(DataStat data)
{
    Datas.append(data);
    std::sort(Datas.begin(),Datas.end(),DataStat::Compare);
}

QString Statistic::ToJSON()
{
    QString result;
    result.append(QString::asprintf("{\"Period\":%d,\"Type\":%d,\"TLen\":%d,\"Hour\":%d,\"Min\":%d,\"Datas\":[",Period,Type,TLen,Hour,Min));
    if(Datas.size()>0){
        foreach (auto var, Datas) {
            result.append(var.ToJSON());
            result.append(",");
        }
        result.remove(result.length()-1,1);
    }
    result.append("]}");
    return result;
}

QString Statistic::ToCSV()
{
    QString result;
    result.append(QString::asprintf("%d:%d;",Hour,Min));
    if(Datas.size()>0){
        foreach (auto var, Datas) {
            result.append(var.ToCSV());
            result.append(";");
        }
        result.remove(result.length()-1,1);
    }
    return result;
}

int Statistic::getTimeStart()
{
    return (Hour*60+Min);
}

void Statistic::FromVector(QVector<int> vector)
{
    while(Datas.size()<vector.size()-1){
        Datas.append(DataStat(Datas.size()+1,0,0));
    }
    for (int var = 1; var < vector.size(); ++var) {
        Datas[var-1].Intensiv=vector[var];
    }
}

bool Statistic::Compare(Statistic &stat1, Statistic &stat2)
{
    if (stat1.Hour!=stat2.Hour) return stat1.Hour<stat2.Hour;
    return stat1.Min<stat2.Min;
}

QVector<int> Statistic::ToVector()
{
    QVector<int> result(Datas.size()+1);
    result[0]=Hour==24?0:(Hour*60+Min);
    foreach (auto var, Datas) {
        result[var.Chanel]=var.Intensiv;
    }
    return result;
}

ArchStat::ArchStat(int region, int area, QDate date)
{
    this->Region=region;
    this->Area=area;
    this->ID=1;
    this->Date=date;
}

ArchStat::ArchStat(QString json)
{
    //    qDebug()<<json;
    error="";
    QJsonParseError jError;
    QJsonDocument jdoc=QJsonDocument::fromJson(json.toUtf8(),&jError);
    if (jError.error!=QJsonParseError::NoError){
        error=jError.errorString();
        return;
    }
    QMap<QString, QVariant>  map=jdoc.toVariant().toMap();
    Region=map["Region"].toInt();
    Area=map["Area"].toInt();
    ID=map["ID"].toInt();
    Date=QDate::fromString(map["Date"].toString(),"yyyy-MM-dd");
    foreach (auto s, map["Statistics"].toList()) {
        Statistics.append(Statistic(s.toMap()));
    }
    std::sort(Statistics.begin(),Statistics.end(),Statistic::Compare);
}

ArchStat::ArchStat(QMap<QString, QVariant> map)
{
    Region=map["Region"].toInt();
    Area=map["Area"].toInt();
    ID=map["ID"].toInt();
    Date=QDate::fromString(map["Date"].toString(),"yyyy-MM-dd");
    Comment=map["comment"].toString();
    foreach (auto s, map["Statistics"].toList()) {
        Statistics.append(Statistic(s.toMap()));
    }
    std::sort(Statistics.begin(),Statistics.end(),Statistic::Compare);
}

ArchStat::ArchStat()
{
    Region=1;
    Area=1;
    ID=1;
    Date=QDate();
}




void ArchStat::AddStatistic(Statistic stat)
{
    Statistics.append(stat);
    std::sort(Statistics.begin(),Statistics.end(),&Statistic::Compare);
}

void ArchStat::DropStat()
{
    Statistics.clear();
}

QString ArchStat::ToJSON()
{
    QString result;
    result.append(QString::asprintf("{\"Region\":%d,\"Area\":%d,\"ID\":%d,\"Date\":",Region,Area,ID));
    result.append("\""+Date.toString("yyyy-MM-dd")+"\"");
    result.append(",\"Statistics\":[");
    if(Statistics.size()>0){
        foreach (auto var, Statistics) {
            result.append(var.ToJSON());
            result.append(",");
        }
        result.remove(result.length()-1,1);
    }
    result.append("],\"comment\":\""+Comment+"\"}");
    return result;
}

QString ArchStat::ToCSV()
{
    QString result;
    if(Statistics.size()>0){
        foreach (auto var, Statistics) {
            result.append(Date.toString("dd-MM-yyyy")+";");
            result.append(var.ToCSV());
            result.append("\n");
        }
    }
    return result;

}

QList<QVector<int> > ArchStat::ToMatrix()
{
    QList<QVector<int> > result;
    foreach (auto var, Statistics) {
        result.append(var.ToVector());
    }
    return result;
}

void ArchStat::FromMatrix(QList<QVector<int> > matrix)
{
    foreach (auto var, matrix) {
//        if (Statistics.size()==0){
//            //Нет такой статистики
//        }
        for (int i = 0; i < Statistics.size(); ++i) {
            if (Statistics[i].getTimeStart()==var[0]){
                Statistics[i].FromVector(var);
            }
        }
    }
}


//bool ArchStat::Compare(ArchStat *archstat1, ArchStat *archstat2)
//{
//    if(archstat1->Region!=archstat2->Region) return archstat1->Region<archstat2->Region;
//    if(archstat1->Area!=archstat2->Area) return archstat1->Area<archstat2->Area;
//    if(archstat1->ID!=archstat2->ID) return archstat1->ID<archstat2->ID;
//    return archstat1->Date<archstat2->Date;
//}
