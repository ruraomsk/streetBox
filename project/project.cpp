#include "project.h"

Project::Project()
{
    isEmpty=true;
}

Project::Project(QString nameFile)
{
    this->nameFile=nameFile;
    isEmpty=false;
    error="";
    QFile file;
    file.setFileName(nameFile);
    if (!file.open(QIODevice::ReadOnly|QIODevice::Text)){
        error=file.error();
        return;
    }
    QString json=file.readAll();
    QJsonParseError jError;
    QJsonDocument jdoc=QJsonDocument::fromJson(json.toUtf8(),&jError);
    if (jError.error!=QJsonParseError::NoError){
        error=jError.errorString();
        return;
    }
    QMap<QString, QVariant>  map=jdoc.toVariant().toMap();
    Region=map["region"].toInt();
    Area=map["area"].toInt();
    SubArea=map["subarea"].toInt();
    Step=map["step"].toInt();
    Chanels=map["chanels"].toInt();
    description=map["description"].toString();
    foreach (auto c, map["crosses"].toList()) {
        crosses.append(new Cross(c.toMap()));
    }
    foreach (auto c, map["xctrls"].toList()) {
        xctrls.append(new Xctrl(c.toMap()));
    }

    isChanged=false;
}

Project::Project(QString nameFile, QString description)
{
    this->nameFile=nameFile;
    this->description=description;
    xctrls.append(new Xctrl(Region,Area,SubArea));
    error="";
    isEmpty=false;
}

Project::~Project()
{
}

bool Project::saveProject()
{
    error="";
    QFile file;
    file.setFileName(nameFile);
    if (!file.open(QIODevice::WriteOnly|QIODevice::Text)){
        error=file.error();
        return false;
    }
    if (file.write(ToJSON().toUtf8())<0){
        error="ошибка записи в файл "+nameFile;
    }
    file.close();
    if (error.size()!=0) return false;
    isChanged=false;
    return true;
}

bool Project::IsEmpty()
{
    return isEmpty;
}

void Project::appendCross(Cross *cross)
{
    crosses.append(cross);
    isChanged=true;
}

QString Project::ToJSON()
{
    QString result;
    result.append(QString::asprintf("{\"region\":%d,\"area\":%d,\"subarea\":%d,\"description\":\"",Region,Area,SubArea));
    result.append(description+"\",\"crosses\":[");
    if (crosses.size()>0){
        foreach (auto var, crosses) {
            result.append(var->ToJSON());
            result.append(",");
        }
        result.remove(result.length()-1,1);
    }
    result.append("],");
    result.append("\"xctrls\":[");
    if (xctrls.size()>0){
        foreach (auto var, xctrls) {
            result.append(var->ToJSON());
            result.append(",");
        }
        result.remove(result.length()-1,1);
    }
    result.append("],");
    result.append(QString::asprintf("\"step\":%d,\"chanels\":%d",Step,Chanels));
    result.append("}");
    return result;
}

QString Project::getDesc()
{
    return description;
}

QString Project::getNameFile()
{
    return nameFile;
}

void Project::ChangeNameFile(QString nameFile)
{
    this->nameFile=nameFile;
}

