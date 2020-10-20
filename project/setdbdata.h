#ifndef SETDBDATA_H
#define SETDBDATA_H

#include <QDialog>
#include <QObject>
#include <QDebug>
#include <QComboBox>
#include <QFrame>
#include <QCheckBox>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QList>
#include <QTableWidget>
#include "cross.h"
#include "../support.h"

class SetDBData : public QDialog
{
        Q_OBJECT
public:
    SetDBData(Cross *cross,QWidget *parent=0);
private slots:
    void Changed(const QString& sdate);
    void loadFromDb();
private:
    void table();
    Cross *cross;
    QComboBox *combo;
    QTableWidget *wtable;
    QList<QCheckBox*> checks;
    QMap<int,QList<QDate>> result;
    QVBoxLayout *list;

};

#endif // SETDBDATA_H
