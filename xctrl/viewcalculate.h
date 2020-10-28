#ifndef VIEWCALCULATE_H
#define VIEWCALCULATE_H

#include <QGroupBox>
#include <QObject>
#include <QRadioButton>
#include <QTableWidget>
#include <QTextEdit>
#include <QWidget>
#include "../project/project.h"
#include "../setup.h"
#include "viewpoints.h"
#include "viewstrategy.h"
#include "xtcalculate.h"
#include "voronoi.h"
#include "sumgraph.h"
extern Setup ini;

class ViewCalculate : public QWidget
{
    Q_OBJECT
public:
    explicit ViewCalculate(Project *project,Xctrl *xctrl,QWidget *parent = nullptr);
    QVector<Point> getSprays();
    QVector<QString> getNames();
    DataGraph getData();
signals:
    void newSpray();
private slots:
    void loadData();
    void calculateData();
    void pushSpray();
private:
    QStringList getAllDates();
    QStringList getAllComments();
    void emptyTable();
    void table();
    Project *project;
    Xctrl *xctrl;
    QList<QVector<QString>> matrix;
    XTCalculate *calcData;
    QTextEdit *ltext;
    QTableWidget *wtable;
    QComboBox *cdates;
    QComboBox *ccomments;
    QGroupBox *group;
    QRadioButton *gdate;
    QRadioButton *gcomment;
    QRadioButton *gdefault;
    QGridLayout *grid;
    QPushButton *btnLoad;
    QPushButton *btnCalculate;
    QPushButton *btnPush;

    QGroupBox *gstyle;
    QRadioButton *garea;
    QRadioButton *gbox;

    QString addString;


};

#endif // VIEWCALCULATE_H
