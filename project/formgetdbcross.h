#ifndef FORMGETDBCROSS_H
#define FORMGETDBCROSS_H

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
class FormGetDbCross : public QDialog
{
public:
    FormGetDbCross( Project *proj, QWidget *parent=0);
    Cross *cross;
private slots:
    void moveData();
    void ChangedDK(const QString &name);
private:
    QLineEdit *lname;           //Имя перекрестка
    QLineEdit *ldesc;           //Описание проекта
    QLineEdit *lregion;         //Регион
    QLineEdit *larea;           //Район
    QLineEdit *lsubarea;        //Подрайон
    QLineEdit *lstep;           //Интервал
    QLineEdit *lchanel;         //Число каналов
    Project *project;
    QComboBox *combo;

};

#endif // FORMGETDBCROSS_H
