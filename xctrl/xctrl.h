#ifndef XCTRL_H
#define XCTRL_H
#include <QList>
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonParseError>

class Calc{
public:
    Calc(int region,int area,int id);
    Calc(QMap<QString, QVariant> map);
    QString ToJSON();

    int Region;                     //Регион `json:"region"`
    int Area;                       //Район `json:"area"`
    int ID;                         //Номер перекрестка по которому принимается решение `json:"id"`
    QList<int> ChanL;               //`json:"chanL"`
    QList<int> ChanR;               //`json:"chanR"`
};
class Areal{
public:
    Areal(int l,int r,int plan,QString description);
    Areal(QMap<QString, QVariant> map);
    QString ToJSON();
    static bool Compare(Areal &ar1,Areal &ar2);
    int L;                          //Интенсивность в прямом направлении
    int R;                          //Интенсивногсть в обратном направлении
    int Plan;                       // Назначенный план прямой
    QString Description;
};

class Strategy{
public:
    Strategy(int l,int r,int planl,int planm,int planr,float fl,float fr,QString description);
    Strategy(QMap<QString, QVariant> map);
    static bool Compare(Strategy &st1,Strategy &st2);

    QString ToJSON();

    int L;                          //Интенсивность в прямом направлении
    int R;                          //Интенсивногсть в обратном направлении
    int PlanL;                       // Назначенный план прямой
    int PlanM;                       // Назначенный план средний
    int PlanR;                       // Назначенный план обратный
    float Fl;
    float Fr;
    QString Description;
};

class Xctrl
{
public:
    Xctrl();
    Xctrl(QMap<QString, QVariant> map);
    QString ToJSON();
    void AddStrategy(Strategy strat);
    void AddAreal(Areal areal);

    void AddCalc(Calc calc);
    void AddStatus(QString status);

    QString name="ХТ подрайона";               //Описание ХТ
    int Left=1000;                  //Максимум для прямого направления `json:"left"`
    int Right=1000;                 //Максимум для обратного направления `json:"right"`
    QStringList Status;             //Состояние расчетов и итоги проверки `json:"status"`

    QList<Strategy> Strategys;      //Правила перехода циклограммой B
    QList<Areal> Areals;            //Правила перехода по областям
    QList<Calc> Calculates;         //Правила расчета характерной точки
private:

};

#endif // XCTRL_H
