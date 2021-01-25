#include "viewxctrl.h"

ViewXctrl::ViewXctrl(Project *project,Xctrl *xctrl,QWidget *parent) : QWidget(parent)
{
    this->project=project;
    this->xctrl=xctrl;
    QWidget *base=new QWidget(this);
    tab=new QTabWidget;
    tab->setTabPosition(QTabWidget::West);
    vstrategy=new ViewStrategy(project,xctrl);
    connect(vstrategy,SIGNAL(updated()),this,SLOT(updated()));
    grid=new QGridLayout(this);
    grid->addWidget(vstrategy,1,1);
    vor=new Voronoi(qMax(xctrl->Left,xctrl->Right),qMax(xctrl->Left,xctrl->Right));
    QList<Strategy> strates;
    foreach(auto st,xctrl->Strategys){
        vor->addSet(st);
        strates.append(st);
    }
    vor->makeDiagramm();
    voroni=new ViewVoronoi(vor);
    grid->addWidget(voroni,0,2,2,2);
    top();
    grid->addWidget(wtop,0,1);
    base->setLayout(grid);
    tab->addTab(base,"Базовые");
    vpoints=new ViewPoints(project,xctrl);
    connect(vpoints,SIGNAL(updated()),this,SLOT(updated()));
    vcalc=new ViewCalculate(project,xctrl);
    connect(vcalc,SIGNAL(newSpray()),this,SLOT(addSpray()));
    vsum=new SumGraph();
    areal=new ViewAreal(project,xctrl);

//    tab->addTab(vstrategy,"Стратегии");
    tab->addTab(areal,"Области");
    tab->addTab(vpoints,"Точки");
    tab->addTab(vcalc,"Расчет");
    tab->addTab(vsum,"Графики");

    QGridLayout *maingrid=new QGridLayout(this);
    maingrid->addWidget(tab,0,1);
    setLayout(maingrid);
    this->show();


}

QString ViewXctrl::getName()
{
    return "XT:"+xctrl->name;
}

void ViewXctrl::SaveToJSON(QString nameFile)
{
    QFile file;
    file.setFileName(nameFile);
    if (!file.open(QIODevice::WriteOnly|QIODevice::Text)){
        Support::ErrorMessage("Ошибка создания файла "+nameFile);
        return;
    }
    if (file.write(xctrl->ToJSON().toUtf8())<0){
        Support::ErrorMessage("Ошибка записи в файл"+nameFile);
    }
    file.close();

}

QString ViewXctrl::SaveToJSON()
{
    return xctrl->ToJSON().toUtf8();
}

Xctrl* ViewXctrl::getXctrl()
{
    return xctrl;
}

void ViewXctrl::ChangeXT()
{
    if(xctrl->name!=lname->text()){
        foreach (auto xt, project->xctrls) {
            if(lname->text()==xt->name){
                Support::ErrorMessage("Такое имя уже используется");
                return;
            }
        }
    }
    xctrl->name=lname->text();
    xctrl->Region=lregion->text().toInt();
    xctrl->Area=larea->text().toInt();
    xctrl->SubArea=lsubarea->text().toInt();
    if(!Support::isVerStep(lstep->text().toInt())){
        Support::ErrorMessage("Интервал вычисления не верный");
        return;
    }
    xctrl->Step=lstep->text().toInt();
    xctrl->Left=lLeftRel->text().toInt();
    xctrl->Right=lRightRel->text().toInt();
    project->isChanged=true;
    updated();
}

void ViewXctrl::updated()
{
    auto temp=vor->sprayes;
    auto tempn=vor->nameSprayes;
    auto tempb=vor->bogoses;
    delete vor;
    delete voroni->text;
    delete voroni;
    vor=new Voronoi(qMax(xctrl->Left,xctrl->Right),qMax(xctrl->Left,xctrl->Right));
    foreach(auto st,xctrl->Strategys){
        vor->addSet(st);
    }
    vor->makeDiagramm();
    vor->sprayes=temp;
    vor->nameSprayes=tempn;
    vor->bogoses=tempb;
    voroni=new ViewVoronoi(vor);
    grid->addWidget(voroni,0,2,2,2);
    top();
    grid->addWidget(wtop,0,1);
    update();
}

void ViewXctrl::addSpray()
{
    int i=0;
    auto names=vcalc->getNames();
    foreach (auto p, vcalc->getSprays()) {
        areal->vor->addSpray(p,names[i]);
        vor->addSpray(p,names[i++]);
    };
    areal->redraw();
    vsum->addData(vcalc->getData());
}

void ViewXctrl::clearSpray()
{
    vor->clearSpray();
    areal->vor->clearSpray();
    areal->redraw();
    vsum->clearAll();
    updated();
}

void ViewXctrl::makeBogko()
{
    QList<Strategy> strates;
    foreach(auto st,xctrl->Strategys){
        strates.append(st);
    }
    Bogko *b=new Bogko(strates);
    if (b->error) return;
    vor->addBogoses(b->centrals);
    updated();
}

void ViewXctrl::top()
{
#define maxSize 500,100
    wtop=new QWidget;
//    xctrl->Left=0;

//    xctrl->Right=0;
//    foreach (auto s, xctrl->Strategys) {
//        xctrl->Left=qMax(xctrl->Left,s.L);
//        xctrl->Right=qMax(xctrl->Right,s.R);
//    }
    lregion=new QLineEdit(QString::number(xctrl->Region),this);
    lregion->setMaximumSize(maxSize);
    larea=new QLineEdit(QString::number(xctrl->Area),this);
    larea->setMaximumSize(maxSize);
    lsubarea=new QLineEdit(QString::number(xctrl->SubArea),this);
    lsubarea->setMaximumSize(maxSize);
    lname=new QLineEdit(xctrl->name,this);
    lname->setMaximumSize(maxSize);
    lstep=new QLineEdit(QString::number(xctrl->Step),this);
    lstep->setMaximumSize(maxSize);
    lLeftRel=new QLineEdit(QString::number(xctrl->Left),this);
    lLeftRel->setMaximumSize(maxSize);
    lRightRel=new QLineEdit(QString::number(xctrl->Right),this);
    lRightRel->setMaximumSize(maxSize);
    QFormLayout *hbox=new QFormLayout();
    hbox->addRow("Наименование XT",lname);
    hbox->addRow("Регион",lregion);
    hbox->addRow("Район",larea);
    hbox->addRow("Подрайон",lsubarea);
    hbox->addRow("Интервал времени",lstep);
    hbox->addRow("Максимум прямого",lLeftRel);
    hbox->addRow("Максимум обратного",lRightRel);
    QPushButton *okBtn=new QPushButton("Принять");
    okBtn->setMaximumSize(100,100);
    connect(okBtn,SIGNAL(clicked()),this,SLOT(ChangeXT()));
    QPushButton *bClearSpray=new QPushButton("Почистить");
    connect(bClearSpray,SIGNAL(clicked()),this,SLOT(clearSpray()));
    bClearSpray->setMaximumSize(100,100);
    QPushButton *bBogko=new QPushButton("Старая система");
    connect(bBogko,SIGNAL(clicked()),this,SLOT(makeBogko()));
    bBogko->setMaximumSize(100,100);
    QHBoxLayout *btn=new QHBoxLayout;
    btn->addWidget(okBtn);
    btn->addWidget(bClearSpray);
    btn->addWidget(bBogko);
    hbox->addRow(btn);
    hbox->addRow(voroni->text);
    wtop->setLayout(hbox);
}

