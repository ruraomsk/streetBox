#ifndef SETEMPTYDATA_H
#define SETEMPTYDATA_H

#include <QObject>
#include <QDialog>
#include <QObject>
#include <QLineEdit>
#include <QFormLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QErrorMessage>
#include <QDateTimeEdit>
#include <QLabel>
#include "../setup.h"
extern Setup ini;
#include "cross.h"
class SetEmptyData : public QDialog
{
    Q_OBJECT
public:
    SetEmptyData(Cross *cross, QWidget *parent=0);

    Cross *cross;
private slots:
    void moveData();
private:
    QCheckBox *lrnd;
    QDateTimeEdit *ldate;
};

#endif // SETEMPTYDATA_H
