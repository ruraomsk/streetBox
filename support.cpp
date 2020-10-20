#include "support.h"


int Support::RandomBetween(int low, int high)
{
    return (QRandomGenerator::global()->generate() % ((high + 1) - low) + low);
}

void Support::ErrorMessage(QString message)
{
    QErrorMessage err;
    err.setModal(true);
    err.showMessage(message);
    err.exec();
    return;
}

void Support::Message(QString message)
{
    QMessageBox msg;
    msg.setModal(true);
    msg.setText(message);
    msg.exec();
    return;
}


QStringList Support::listCrosses(int region, int area)
{
    QStringList result;
    QString w;
    QSqlQuery query;
    w.append(QString::asprintf("select id,describ,subarea from public.cross where region=%d and area=%d order by id;",region,area));
    query.exec(w);
    while(query.next()){
        QString ww;
        ww.append(QString::number(query.value(0).toInt()));
        ww.append(":");
        ww.append(query.value(1).toString());
        ww.append(":");
        ww.append(QString::number(query.value(2).toInt()));
        result.append(ww);
    }
    return result;
}

QMap<int, QList<QDate> > Support::listStatistics(int region, int area)
{
    QMap<int, QList<QDate> > result;
    QString w;
    QSqlQuery query;
    w.append(QString::asprintf("select id,date from public.statistics where region=%d and area=%d order by id,date;",region,area));
    query.exec(w);
    while(query.next()){
        if (result.find(query.value(0).toInt())== result.end()){
            QList<QDate> r;
            result[query.value(0).toInt()]=r;
        }
        QList<QDate> r=result[query.value(0).toInt()];
        r.append(query.value(1).toDate());
        result[query.value(0).toInt()]=r;
    }
    return result;
}

QString Support::getArchStat(int region, int area, int id, QDate date)
{
    QString result="{}";
    QString w;
    QSqlQuery query;
    w.append(QString::asprintf("select stat from public.statistics where region=%d and area=%d and id=%d and date='",region,area,id));
    w.append(date.toString(Qt::ISODate)+"';");
    query.exec(w);
    while(query.next()){
        return query.value(0).toString();
    }
    return result;
}

QList<QVector<int> > Support::Summator(QList<QVector<int> > matrix, int oldstep, int newstep)
{
//    qDebug()<<oldstep<<newstep;
    QList<QVector<int> > result;
    if(oldstep>newstep) return result;

    int len=0;
    foreach (auto line, matrix) {
        len=len<line.size()?line.size():len;
    }
    int time=0;
    while(time<24*60){
        QVector<int> data(len);
        data[0]=time;
        time+=newstep;
        result.append(data);
    }
    for (int i = 0; i < result.size(); ++i) {
        for (int j = 0; j < matrix.size(); ++j) {

            if( compare(matrix[j][0],result[i][0],newstep)){
                for (int k = 1; k < result[i].size()&&k < matrix[j].size(); ++k) {
                    result[i][k]+=matrix[j][k];
                }
            }
        }
    }
    return result;
}

QList<QString> Support::getElite(int step)
{
    switch (step) {
    case 1: return {"5","10","15","20","30","60"};
    case 5: return {"10","15","20","30","60"};
    case 10: return {"20","30","60"};
    case 15: return {"30","60"};
    case 20: return {"40","60"};
    case 30: return {"60"};
    default: return {};
    }
}

bool Support::isVerStep(int step)
{
    switch (step){
    case 1:
    case 5:
    case 10:
    case 15:
    case 20:
    case 30:
    case 60:
        return true;
    default: return false;
    }
}

QString Support::CorrectNameFile(QString namefile,QString list)
{
    QFileInfo info(namefile);
    if (info.completeSuffix().size()!=0) return namefile;
    namefile+="."+list.split(" ")[0].remove("*.");
    return namefile;
}

bool Support::compare(int time, int ntime, int step)
{
    if (time+step>24*60 && ntime==0){
        return true;
    }
    return time<=ntime&&time>ntime-step;

}


