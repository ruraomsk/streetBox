#ifndef FORMGETDBXCTRL_H
#define FORMGETDBXCTRL_H

#include <QDialog>
#include <QObject>
#include <QComboBox>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFormLayout>
#include "project.h"
#include "../setup.h"

extern Setup ini;

class FormGetDBXctrl:public QDialog
{
public:
    FormGetDBXctrl( Project *proj, QWidget *parent=0);
    Xctrl *xctrl;
private slots:
    void moveData();
    void ChangedXT(const QString &name);
private:
    void getParams(const QString &name);
    int getInt(const QString name);
    int lregion;         //Регион
    int larea;           //Район
    int lsubarea;        //Подрайон
    Project *project;
    QComboBox *combo;
};

#endif // FORMGETDBXCTRL_H
