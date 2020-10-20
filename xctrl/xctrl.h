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
class Result{
public:
    Result(int il,int ir);
    Result(QMap<QString, QVariant> map);
    QString ToJSON();

    int Ileft  ;                    //Интенсивность прямого направления `json:"il"`
    int Iright ;                    //Интенсивность обратного направления `json:"ir"`
};

class Xctrl
{
public:
    Xctrl(int region,int area,int subarea);
    Xctrl(QMap<QString, QVariant> map);
    QString ToJSON();
    void AddStrategy(Strategy strat);
    void AddCalc(Calc calc);
    void AddResult(Result result);
    void AddStatus(QString status);

    int Region;                     //Регион `json:"region"`
    int Area;                       //Район `json:"area"`
    int SubArea;                    //`json:"subarea"`
    QString name="ХТ подрайона";               //Описание ХТ
    bool Switch=false;                    //true призводим расчет нового плана `json:"switch"`
    bool Release=false;                   //true выполняем план `json:"release"`
    int Step=15;                       //Время цикла для данного подрайона `json:"step"`
    int Remain=Step;                     //Остаток времени для нового расчета `json:"rem"`
    QDateTime LastTime ;            //Последний расчет характерной точки `json:"ltime"`
    int PKCalc=0;                     //Расчитанный ПК `json:"pkcalc"`
    int PKNow=0;                      //Текущий ПК `json:"pknow"`
    int PKLast=0;                     //Предыдущий ПК `json:"pklast"`
    int Left=1000;                  //Максимум для прямого направления `json:"left"`
    int Right=1000;                 //Максимум для обратного направления `json:"right"`
    QStringList Status;             //Состояние расчетов и итоги проверки `json:"status"`
    QList<Strategy> Strategys;      //Правила перехода
    QList<Calc> Calculates;         //Правила расчета характерной точки
    QList<Result> Results;          //Промежуточные результаты
private:

};

#endif // XCTRL_H
