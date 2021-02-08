#include "sumgraph.h"

DataGraph::DataGraph()
{

}
DataGraph::DataGraph( QString comment)
{
    this->comment=comment;
}
QChartView *DataGraph::getGraph()
{
    QChart *chart=new QChart();
    QDateTimeAxis *axisX=new QDateTimeAxis;
    axisX->setTickCount(25);
    axisX->setFormat("hh:mm");
    axisX->setTitleText("Время");
    QValueAxis *axisY=new QValueAxis;
    axisY->setLabelFormat("%i");
    axisY->setTitleText("Значения");
    chart->addAxis(axisX,Qt::AlignBottom);
    chart->addAxis(axisY,Qt::AlignLeft);
    QDate date=QDate::currentDate();
    int max=0;
    for (int col = 1; col < 3; ++col) {
        QLineSeries *series=new QLineSeries;
        for(int line=0;line<datas.size();++line){
            int hour=datas[line][0]/60;
            hour=hour==24?0:hour;
            int min=datas[line][0]%60;
            QTime time(hour,min);
            QDateTime dt(date,time);
            series->append(dt.toMSecsSinceEpoch(),datas[line][col]);
            max=qMax(max,datas[line][col]);
        }
        QTime time(0,0);
        QDateTime dt(date.addDays(1),time);
        axisX->setMax(dt);
        QDateTime dtm(date,time);
        axisX->setMin(dtm);

        series->setName(col==1?"Прямое":"Обратное");
        series->setPointLabelsVisible(true);
        series->setPointLabelsFormat("@yPoint");
        chart->addSeries(series);
        axisY->setMax(qreal(max+(0.1*max)));
        series->attachAxis(axisX);
        series->attachAxis(axisY);
    }
    chart->legend()->hide();
    chart->setTitle(comment);
    QChartView *viewchart=new QChartView(chart);
    viewchart->setRenderHint(QPainter::Antialiasing);
    viewchart->setMinimumSize(ini.getSize("chart/sizesmall"));
    return viewchart;
}
SumGraph::SumGraph()
{
    grid=new QGridLayout(this);
    wgraph=new QScrollArea;
    grid->setAlignment(Qt::AlignTop);
    GraphRepaint();
//    show();
}

void SumGraph::clearAll()
{
    tables.clear();
    GraphRepaint();
    update();
}

void SumGraph::addData(DataGraph data)
{
    tables[data.comment]=data;
    GraphRepaint();
    update();
}

void SumGraph::GraphRepaint()
{
    QWidget *local=new QWidget;
    delete wgraph;
    wgraph=new QScrollArea;
    QVBoxLayout *vbox=new QVBoxLayout;
    foreach (auto data, tables) {
        vbox->addWidget(data.getGraph());
    }
    local->setLayout(vbox);
    wgraph->setWidget(local);
    wgraph->setWindowTitle("Графики");
    wgraph->setMinimumSize(ini.getSize("chart/sizehuge"));
    grid->addWidget(wgraph,0,1);
}
