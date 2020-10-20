#ifndef SETCSVDATA_H
#define SETCSVDATA_H

#include <QObject>
#include <QWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QStringList>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QLabel>
#include <QTextEdit>
#include <QGridLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QList>
#include <QVector>
#include <QTableWidget>

#include "../setup.h"
#include "cross.h"
extern Setup ini;


class SetCSVData : public QDialog
{
    Q_OBJECT
public:
    SetCSVData(Cross *cross,QWidget *parent = nullptr);
    void loadData();
    QStringList lines;
    QString simbol=";";
    QString formatDate="dd.MM.yyyy";
    bool fileLoaded=false;
    int colDate=0;
    int colTime=1;
    int colData=2;
    int rowStart=0;
    int rows=100;
    int chanel=1;
signals:
private slots:
    void redraw();
    void redrawFormatDate(const QString &name);
    void getChanges();
    void getSpinBoxChanges(int value);
    void createCross();
    void rejectCross();
private:

    int countColumns(int row);
    int columns;
    void makeText();
    void makeTable();
    QList<QVector<QString>> makeMatrix();

    QLineEdit *lsimbol;
    QSpinBox *lrows;        //Сколько строк читать и выводить как образец
    QSpinBox *lrow;         //С какой строки начинаются данные
    QSpinBox *ldate;        //В какой колонке дата
    QSpinBox *ltime;        //В какой колонке время
    QSpinBox *ldata;        //c какой колонки начинаются данные измерений
    QComboBox *fdate;       //Выбор формата даты

    QSpinBox *lchanel;         //Каким каналом принять

    QTextEdit *ltext;
    QTableWidget *wtable;

    Cross *cross;
    QList<QVector<QString> >    tableMatrix;

};

#endif // SETCSVDATA_H
