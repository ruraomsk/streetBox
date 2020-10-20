#include "setup.h"



Setup::Setup(QString fullPathFile)
{
    file.setFileName(fullPathFile);
    bool ok=file.open(QIODevice::ReadOnly|QIODevice::Text);
    if (!ok){
        file.open(QIODevice::WriteOnly|QIODevice::Text);
        file.close();
        return;
    }
    QString json=file.readAll();
    file.close();
    QJsonParseError jError;
    QJsonDocument jdoc=QJsonDocument::fromJson(json.toUtf8(),&jError);
    if (jError.error!=QJsonParseError::NoError){
        return;
    }
    vars=jdoc.toVariant().toMap();
}

int Setup::getInt(QString name)
{
    return get(name,QVariant(0)).toInt();
}

QString Setup::getString(QString name)
{
    return get(name,QVariant("")).toString();
}

bool Setup::getBool(QString name)
{
    return get(name,QVariant(false)).toBool();
}

float Setup::getFloat(QString name)
{
    return get(name,QVariant(0.0)).toFloat();
}

QSize Setup::getSize(QString name)
{
    int w=get(name+"/w",QVariant(0)).toInt();
    int h=get(name+"/h",QVariant(0)).toInt();
    return QSize(w,h);
}

QUrl Setup::getUrl(QString name)
{
    return get(name,QVariant(QUrl())).toUrl();
}

QDate Setup::getDate(QString name)
{
    return get(name,QVariant(QUrl())).toDate();
}

QPoint Setup::getPoint(QString name)
{
    int x=get(name+"/x",QVariant(0)).toInt();
    int y=get(name+"/y",QVariant(0)).toInt();
    return QPoint(x,y);
}

void Setup::setInt(QString name, int value)
{
    set(name,QVariant(value));
}

void Setup::setString(QString name, QString value)
{
    set(name,QVariant(value));
}

void Setup::setBool(QString name, bool value)
{
    set(name,QVariant(value));
}

void Setup::setFloat(QString name, float value)
{
    set(name,QVariant(value));
}

void Setup::setSize(QString name, QSize value)
{
    set(name+"/w",QVariant(value.width()));
    set(name+"/h",QVariant(value.height()));
}

void Setup::setUrl(QString name, QUrl value)
{
    set(name,QVariant(value));
}

void Setup::setDate(QString name, QDate value)
{
    set(name,QVariant(value));
}

void Setup::setPoint(QString name, QPoint value)
{
    set(name+"/x",QVariant(value.x()));
    set(name+"/y",QVariant(value.y()));
}

void Setup::saveSetup()
{
    file.open(QIODevice::WriteOnly|QIODevice::Text);
    QVariant r(vars);
    QJsonDocument jdoc=QJsonDocument::fromVariant(r);
    file.write(jdoc.toJson());
    file.close();
}

QVariant Setup::get(QString name, QVariant def)
{
    auto list=name.split("/");
    if (list.isEmpty()){
        return def;
    }
    mutex.lock();
    if (list.size()==1){
        if(!vars.contains(list[0])){
            vars[list[0]]=QVariant(def);
            saveSetup();
        }
        mutex.unlock();
        return vars[list[0]];
    }
    bool needSave=false;
    QList<QVariant> lvars;
    lvars.append(QVariant(vars));
    QMap<QString,QVariant> map;
    int pos=0;
    while(pos<=list.size()-2){
        map=lvars[pos].toMap();
        if(map.isEmpty()){
            QMap<QString,QVariant> newmap;
            map[list[pos]]=newmap;
            lvars[pos]=QVariant(map);
            needSave=true;
        }
        if(!map.contains(list[pos])){
            QMap<QString,QVariant> newmap;
            map[list[pos]]=newmap;
            lvars[pos]=QVariant(map);
            needSave=true;
        }
        map=lvars[pos].toMap();
        lvars.append(QVariant(map[list[pos]]));
        pos++;
    }
    map=lvars[pos].toMap();
    if(map.isEmpty()){
        map[list[pos]]=def;
        lvars[pos]=QVariant(map);
        needSave=true;
    }
    if(!map.contains(list[pos])){
        map[list[pos]]=def;
        lvars[pos]=QVariant(map);
        needSave=true;
    }
    auto ret=map[list[pos]];
    if (needSave) {
        while(pos>0){
            map=lvars[pos].toMap();
            map[list[pos-1]]=lvars[pos];
            lvars[pos-1]=QVariant(map);
            pos--;
        }
        vars=lvars[0].toMap();
        saveSetup();
    }
    mutex.unlock();
    return ret;

}

void Setup::set(QString name, QVariant value)
{
    auto list=name.split("/");
    if (list.isEmpty()){
        return;
    }
    mutex.lock();
    if (list.size()==1){
        if(!vars.contains(list[0])){
            vars[list[0]]=QVariant(value);
        }
        vars[list[0]]=value;
        saveSetup();
        mutex.unlock();
        return;
    }
    QList<QVariant> lvars;
    lvars.append(QVariant(vars));
    QMap<QString,QVariant> map;
    int pos=0;
    while(pos<=list.size()-2){
        map=lvars[pos].toMap();
        if(map.isEmpty()){
            QMap<QString,QVariant> newmap;
            map[list[pos]]=newmap;
            lvars[pos]=QVariant(map);
        }
        if(!map.contains(list[pos])){
            QMap<QString,QVariant> newmap;
            map[list[pos]]=newmap;
            lvars[pos]=QVariant(map);
        }
        map=lvars[pos].toMap();
        lvars.append(QVariant(map[list[pos]]));
        pos++;
    }
    map=lvars[pos].toMap();
    if(map.isEmpty()){
        map[list[pos]]=value;
        lvars[pos]=QVariant(map);
    }
    if(!map.contains(list[pos])){
        map[list[pos]]=value;
        lvars[pos]=QVariant(map);
    }
    map[list[pos]]=value;
    lvars[pos]=QVariant(map);
    while(pos>0){
        map=lvars[pos-1].toMap();
        map[list[pos-1]]=lvars[pos];
        lvars[pos-1]=QVariant(map);
        pos--;
    }
    vars=lvars[0].toMap();
    saveSetup();
    mutex.unlock();
}
