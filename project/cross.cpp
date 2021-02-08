#include "cross.h"

Cross::Cross(QMap<QString, QVariant> map)
{
    archs.clear();
    Number=map["number"].toInt();
    Description=map["desc"].toString();
    Region=map["region"].toInt();
    Area=map["area"].toInt();
    SubArea=map["subarea"].toInt();
    Step=map["step"].toInt();
    Chanels=map["chanels"].toInt();
    summ=map["summ"].toBool();
    SummInreval=map["inter"].toInt();
    foreach (auto a, map["archs"].toList()) {
        ArchStat arch=a.toMap();
        //        if (arch.Statistics.size()==0) continue;
        if(arch.Date.toString().size()==0) continue;
        archs[arch.Date]=arch;
        //        qDebug()<<arch.Date;
    }
    foreach (auto a, map["comments"].toList()) {
    }
    isEmpty=false;
}

Cross::Cross(int number, QString description)
{
    Number=number;
    Description=description;
    isEmpty=false;
    archs.clear();
}

void Cross::Attach(ArchStat arch)
{
    archs[arch.Date]=arch;
    isEmpty=false;
}

void Cross::SetData(QDate date, bool rnd)
{
    auto arch=archs[date];
    arch.DropStat();
    int min=0;
    int count=0;
    while(min<24*60){
        min+=Step;
        count++;
        int hour=min/60;
        hour=hour==24?0:hour;
        int m=min%60;
        auto stat=Statistic(count,1,Step,hour,m);
        for (int k = 0; k < Chanels; ++k) {
            if (rnd) stat.AddData(DataStat(k+1,0,Support::RandomBetween(1,35)));
            else stat.AddData(DataStat(k+1,0,0));
        }
        arch.AddStatistic(stat);
    }
    archs[date]=arch;
}

void Cross::setDataFromMatrix(QDate date, QList<QVector<int> > matrix)
{
    ArchStat arch(Region,Area,date);
    arch.ID=Number;
    int period=0;
    foreach (auto line, matrix) {
        int hour=line[0]/60;
        hour=hour==24?0:hour;
        int min=line[0]%60;
        Statistic stat(period++,1,Step,hour,min);
        for (int i = 0; i < Chanels; ++i) {
            DataStat data(i+1,0,line[i+1]);
            stat.AddData(data);
        }
        arch.AddStatistic(stat);
    }
    Attach(arch);
}

QList<QVector<int> > Cross::getMatrix(QString comment)
{
    QList<QVector<int> > result;
    foreach (auto arch, archs) {
        if(arch.Comment.compare(comment)){
            return arch.ToMatrix();
        }
    }
    return result;
}

QList<QVector<int> > Cross::getMatrix(QDate date)
{
    QList<QVector<int> > result;
    if(!archs.contains(date)) return result;
    return archs[date].ToMatrix();
}

QStringList Cross::getComments()
{
    QStringList result;
    foreach (auto arch, archs) {
        if(arch.Comment.size()!=0){
            result.append(arch.Comment);
        }
    }
    return result;
}

bool Cross::IsEmpty()
{
    return isEmpty;
}

QString Cross::ToJSON()
{
    QString result;
    result.append("{\"number\":"+QString::number(Number)+",\"desc\":\""+Description+"\",\"archs\":[");
    if(archs.size()>0){
        foreach (auto var, archs) {
            //            if (var.Date.toString().size()==0) continue;
            result.append(var.ToJSON());
            result.append(",");
        }
        result.remove(result.length()-1,1);
    }
    result.append("],");
    result.append(QString::asprintf("\"region\":%d,\"area\":%d,\"subarea\":%d,\"step\":%d,\"chanels\":%d,\"inter\":%d,\"summ\":",Region,Area,SubArea,Step,Chanels,SummInreval));
    result.append(summ?"true":"false");
    result.append("}");
    return result;
}
void Cross::SaveCrossToCSV(QString nameFile)
{
//    qDebug()<<"save to csv"<<nameFile;
    QFile file;
    file.setFileName(nameFile);
    if (!file.open(QIODevice::WriteOnly|QIODevice::Text)){
        Support::ErrorMessage("Ошибка создания файла "+nameFile);
        return;
    }
    if (file.write(ToCSV().toUtf8())<0){
        Support::ErrorMessage("Ошибка записи в файл"+nameFile);
    }
    file.close();
}

QString Cross::ToCSV()
{
    QString result;
    result.append(QString::number(Number)+" "+Description+"\n");
    result.append(QString::asprintf("Регион %d Район %d Подрайон %d Интервал %d Направлений %d\n",Region,Area,SubArea,Step,Chanels));
    if(archs.size()>0){
        foreach (auto var, archs) {
            //            if (var.Date.toString().size()==0) continue;
            result.append(var.ToCSV());
            result.append("\n");
        }
    } else result.append("\n");
    result.append("\n");
    return result;

}
