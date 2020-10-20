#ifndef ADDEMPTYXT_H
#define ADDEMPTYXT_H

#include <QDialog>
#include <QObject>
#include <QLineEdit>
#include <QFormLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QErrorMessage>
#include <QString>

#include "../project/project.h"
#include "../support.h"

#include "../setup.h"
extern Setup ini;

class DialogAddEmptyXT : public QDialog
{
    Q_OBJECT

public:
    DialogAddEmptyXT(Project *project, QWidget *parent=0);

private slots:
    void setData();
private:
    Project *project;
    QLineEdit *lname;           //Имя XT
    QLineEdit *lregion;         //Регион
    QLineEdit *larea;           //Район
    QLineEdit *lsubarea;        //Подрайон
    QLineEdit *lstep;           //Интервал


};

#endif // ADDEMPTYXT_H
