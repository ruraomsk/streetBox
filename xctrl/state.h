#ifndef STATE_H
#define STATE_H
#include "xctrl.h"
#include "prioryty.h"
#include <QList>
class State
{
public:
    State();
    State(QMap<QString, QVariant>  map);
    void addPrioryty(Prioryty prioryty);
    void addXctrl(Xctrl *xctrl);
    Prioryty getPrioryty();
    QList<Xctrl *> getXctrls();
    QString extToJson();
    QString ToJSON();

    int Region;
    int Area;
    int SubArea;
    int PKCalc=0;                     //Расчитанный ПК `json:"pkcalc"`
    int PKNow=0;                      //Текущий ПК `json:"pknow"`
    int PKLast=0;                     //Предыдущий ПК `json:"pklast"`
    bool Switch=false;                    //true призводим расчет нового плана `json:"switch"`
    bool Release=false;                   //true выполняем план `json:"release"`
    bool UseStrategy=false;             //true выполняем стратегию Лучи  иначе стратегия Области
    int Step=15;                       //Время цикла для данного подрайона `json:"step"`
    int Remain=Step;                     //Остаток времени для нового расчета `json:"rem"`
    int LastTime ;            //Последний расчет характерной точки `json:"ltime"`
    int external[12][2]={{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}};
    Prioryty prioryty;
    QList<Xctrl *> xctrls;

};

#endif // STATE_H
