#include "project.h"
extern Common common;
Project::Project()
{
    isEmpty=true;
    common.stepXT=15;
    common.use=true;
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
//    QJsonObject jsonObj = jdoc.object();
//    QJsonArray array= jsonObj.value("Prioryty").toArray();
    QMap<QString, QVariant>  map=jdoc.toVariant().toMap();
    Region=map["region"].toInt();
    Area=map["area"].toInt();
    SubArea=map["subarea"].toInt();
    StepDevice=map["stepdev"].toInt();
    StepXT=map["stepxt"].toInt();
    Chanels=map["chanels"].toInt();
    UseStrategy=map["use"].toBool();
    description=map["description"].toString();
    foreach (auto c, map["crosses"].toList()) {
        crosses.append(new Cross(c.toMap()));
    }
    foreach (auto c, map["xctrls"].toList()) {
        xctrls.append(new Xctrl(c.toMap()));
    }
    prioryty.FromJSON(map["Prioryty"].toJsonArray());
    auto array=map["ext"].toJsonArray();
    for (int i = 0; i < 12; ++i) {
        for (int j = 0; j < 2; ++j) {
            external[i][j]=array.at(i).toArray().at(j).toInt();
        }
    }

    isChanged=false;
    common.stepXT=StepXT;
    common.use=UseStrategy;
}

Project::Project(QString nameFile, QString description)
{
    this->nameFile=nameFile;
    this->description=description;
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

void Project::updateFromState(State *state)
{
    Region=state->Region;
    Area=state->Area;
    SubArea=state->SubArea;
    StepXT=state->Step;
    UseStrategy=state->UseStrategy;
    xctrls.clear();
    foreach(auto s,state->xctrls){
        xctrls.append(s);
    }
    prioryty=state->prioryty;
    for (int i = 0; i < 12; ++i) {
        for (int j = 0; j < 2; ++j) {
            external[i][j]=state->external[i][j];
        }
    }

}

State *Project::makeState()
{
    State *state=new State();
    state->Region=Region;
    state->Area=Area;
    state->SubArea=SubArea;
    state->Step=StepXT;
    state->LastTime.setTime_t(0);
    state->Remain=0;
    state->UseStrategy=UseStrategy;
    state->PKNow=0;
    state->PKLast=0;
    state->PKCalc=0;
    foreach(auto s,xctrls){
        state->xctrls.append(s);
    }
    state->prioryty=prioryty;
    for (int i = 0; i < 12; ++i) {
        for (int j = 0; j < 2; ++j) {
            state->external[i][j]=external[i][j];
        }
    }
    return state;
}

QString Project::ToJSON()
{
    QString result;
    result.append(QString::asprintf("{\"region\":%d,\"area\":%d,\"subarea\":%d,\"description\":\"",Region,Area,SubArea));
    result.append(description+"\",\"use\":"+(UseStrategy?"true":"false")+",\"crosses\":[");
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
    result.append(prioryty.ToJSON()+",");
    result.append(extToJson()+",");
    result.append(QString::asprintf("\"stepdev\":%d,\"stepxt\":%d,\"chanels\":%d",StepDevice,StepXT,Chanels));
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

QString Project::extToJson()
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

