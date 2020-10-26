#include "xtcalculate.h"

XTCalculate::XTCalculate(Project *project,Xctrl *xctrl,QString comment)
{
    this->project=project;
    this->xctrl=xctrl;
    controlStep();
    foreach (auto val, setListDU()) {
        foreach (auto cr, project->crosses) {
            if(val!=cr->Number) continue;
            auto m=cr->getMatrix(comment);
            if (m.size()==0){
                protocol.append(QString::asprintf("Перекресток %d не содержит ",cr->Number)+comment);
                error=true;
            } else {
                dus[val]=XTMatrix(m);
                dus[val].adaptHour(xctrl->Step);
            }
        }
    }
    if(error)   protocol.insert(0,"Есть ошибки!");
    else protocol.insert(0,"Ошибок нет.");
}

XTCalculate::XTCalculate(Project *project, Xctrl *xctrl, QDate date)
{
    this->project=project;
    this->xctrl=xctrl;
    controlStep();
    foreach (auto val, setListDU()) {
        foreach (auto cr, project->crosses) {
            if(val!=cr->Number) continue;
            auto m=cr->getMatrix(date);
            if (m.size()==0){
                protocol.append(QString::asprintf("Перекресток %d не содержит ",cr->Number)+date.toString("dd.MM.yyyy"));
                error=true;
            } else {
                dus[val]=XTMatrix(m);
                dus[val].adaptHour(xctrl->Step);
            }
        }
    }
    if(error)   protocol.insert(0,"Есть ошибки!");
    else protocol.insert(0,"Ошибок нет.");
}

XTCalculate::XTCalculate(Project *project,Xctrl *xctrl)
{
    this->project=project;
    this->xctrl=xctrl;
    controlStep();
    foreach (auto val, setListDU()) {
        foreach (auto cr, project->crosses) {
            if(val!=cr->Number) continue;
            auto m=cr->getMatrix(cr->now);
            protocol.append(QString::asprintf("Перекресток %d будет расчитан по ",cr->Number)+cr->now.toString("dd.MM.yyyy"));
            if (m.size()==0){
                protocol.append(QString::asprintf("Перекресток %d не содержит ",cr->Number)+cr->now.toString("dd.MM.yyyy"));
                error=true;
            } else {
                dus[val]=XTMatrix(m);
                dus[val].adaptHour(xctrl->Step);
            }
        }
    }
    if(error)   protocol.insert(0,"Есть ошибки!");
    else protocol.insert(0,"Ошибок нет.");
}
//Собственно расчет планов XT
void XTCalculate::calculate()
{
    int endTime=xctrl->Step;
    int beginTime=0;
    while(endTime<24*60){
        int l=0,r=0;
        QVector<QString> v(10);
        v[0]=QString::asprintf("%02d:%02d",endTime/60,endTime%60);
        QString line;
        QVector<float> fs;
        foreach (auto c, xctrl->Calculates) {
            l=qMax(l,dus[c.ID].getFlow(c.ChanL,beginTime,endTime));
            line+=" ("+QString::number(dus[c.ID].getFlow(c.ChanL,beginTime,endTime))+",";
            r=qMax(r,dus[c.ID].getFlow(c.ChanR,beginTime,endTime));
            line+=QString::number(dus[c.ID].getFlow(c.ChanR,beginTime,endTime))+")";
            if(r==0)r=999999;
            if(l==0)l=1;
            float f=(float)l/(float)r;
            fs.append(f);
        }
        protocol.append(v[0]+" "+line);
        v[1]=QString::number(l);
        v[2]=QString::number(r);
        float f=0.0;
        int plan=0;
        foreach (auto st, xctrl->Strategys) {
            if(l<=st.L&&r<=st.R){
                int c[3]={0,0,0};
                foreach (float f, fs) {
                    if(f<=st.Fl) c[0]++;
                    if(f>=st.Fr) c[2]++;
                    if(f>st.Fl&&f<st.Fr) c[1]++;
                }
                if(r==0)r=999999;
                if(l==0)l=1;
                f=(float)r/(float)l;
                plan=st.PlanM;
                if (c[0]>c[1]&&c[0]>c[2])plan=st.PlanL;
                if (c[2]>c[0]&&c[2]>c[1]) plan=st.PlanR;
                break;
            }
        }
        if (plan==0)    v[3]="Нет плана";
        else            v[3]=QString::number(plan)+" "+getDescription(plan);
        v[4]=QString::asprintf("%.2f",f);
        fin.append(v);
        beginTime=endTime;
        endTime+=xctrl->Step;
    }
}
QList<int> XTCalculate::setListDU()
{
    QList<int> result;
    foreach (auto c, xctrl->Calculates) {
        result.append(c.ID);
    }
    return result;
}
void XTCalculate::controlStep()
{
    foreach (auto val, setListDU()) {
        foreach (auto cr, project->crosses) {
            if(val!=cr->Number) continue;
            if(cr->Step>xctrl->Step){
                protocol.append(QString::asprintf("Перекресток %d имеет интервал %d больше чем нужно",cr->Number,cr->Step));
                error=true;
            }
        }
    }
}

QString XTCalculate::getDescription(int plan)
{
    foreach (auto st, xctrl->Strategys) {
        if(st.PlanL==plan) return st.Description;
        if(st.PlanM==plan) return st.Description;
        if(st.PlanR==plan) return st.Description;
    }
    return "Нет такого описания";
}


XTMatrix::XTMatrix()
{
    matrix.clear();
}

XTMatrix::XTMatrix(QList<QVector<int> > matrix)
{
    this->matrix=matrix;
}
//Вычисляет часовой поток по интервалу времени суммируя каналы
int XTMatrix::getFlow(QList<int> chanels, int beginTime, int endTime)
{
     int result=0;
     foreach (auto line, matrix) {
         if(line[0]>beginTime&&line[0]<=endTime){
             foreach (auto ch, chanels) {
                 if(ch<line.size()) result+=line[ch];
             }
         }
     }
     return result;
}
// Привести все показания к часу
void XTMatrix::adaptHour(int step)
{
    int k=60/step;
    for (int line = 0; line < matrix.size(); ++line) {
        for (int j = 1; j < matrix[line].size(); ++j) {
            matrix[line][j]=matrix[line][j]*k;
        }
    }
}

