#include "bogko.h"


Bogko::Bogko(QList<Strategy> strs)
{
    if (strs.size()==0) return;
    this->strates=strs;
    centrals.clear();
    std::sort(strates.begin(),strates.end(),&Strategy::Compare);
    int l=0,r=0;
    error=false;
    foreach (auto st, strates) {
        if(st.L>l && st.R>r){
            l=st.L;
            r=st.R;
        } else {
            Support::ErrorMessage(QString::asprintf("Нарушена монотонность %d>%d %d>%d",st.L,l,st.R,r));
            error=true;
            return;
        }
    }
    if(error) return;
    //Вычисляем центры на главной последовательности
    Point center(0,0);
    int pos=0;
    auto st=strates[0];
    auto point=center.mediana(st.R,st.L);
    if(!point.isGood()){
        Support::ErrorMessage(QString::asprintf("Ошибка на этапе %d %d",st.L,st.R));
        error=true;
        return;
    }
    centrals.append(point);
    while(pos<strates.size()-1){

        auto pn=point.summ(st.R,st.L,strates[pos+1].R,strates[pos+1].L);
        if(!pn.isGood()){
            Support::ErrorMessage(QString::asprintf("Ошибка на этапе %d %d",st.L,st.R));
            error=true;
            return;
        }
        centrals.append(pn);
        point=pn;
        st=strates[++pos];
    }
    //Теперь проходим по векторам если они есть
    pos=-1;
    foreach (auto st, strates) {
        ++pos;
        if(st.Fl>st.Fr||st.Fl>1||st.Fr<1){
            Support::ErrorMessage(QString::asprintf("Проверьте коээфициенты на этапе %d %d",st.L,st.R));
            error=true;
            return;
        }
        //По прямой интенсивности
        if(st.Fl<1.0){
                auto p=centrals[pos].simetr(st.Fl);
                centrals.append(p);
        }
        if(st.Fr>1.0){
                auto p=centrals[pos].simetr(st.Fr);
                centrals.append(p);
        }
    }
}
