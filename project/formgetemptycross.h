#ifndef FORMGETEMPTYCROSS_H
#define FORMGETEMPTYCROSS_H

#include <QDialog>
#include <QObject>
#include <QLineEdit>
#include <QFormLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QErrorMessage>
#include <QString>
#include "cross.h"
#include "project.h"
#include "../support.h"

#include "../setup.h"
extern Setup ini;

class FormGetEmptyCross : public QDialog
{
    Q_OBJECT
public:
    FormGetEmptyCross( Project *proj, QWidget *parent=0);

    Cross *cross;
private slots:
    void moveData();

private:
    QLineEdit *lname;           //Имя перекрестка
    QLineEdit *ldesc;           //Описание проекта
    QLineEdit *lregion;         //Регион
    QLineEdit *larea;           //Район
    QLineEdit *lsubarea;        //Подрайон
    QLineEdit *lstep;           //Интервал
    QLineEdit *lchanel;         //Число каналов
    QCheckBox *lrnd;            //Признак случайной генерации
    Project *project;
};

#endif // FORMGETEMPTYCROSS_H
