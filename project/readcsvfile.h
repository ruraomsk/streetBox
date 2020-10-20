#ifndef READCSVFILE_H
#define READCSVFILE_H

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
#include "project.h"
extern Setup ini;

class ReadCSVFile : public QDialog
{
    Q_OBJECT
public:
    explicit ReadCSVFile(Project *project,QWidget *parent = nullptr);
    ReadCSVFile(Project *project,QString nameFile);
    void loadData();
    QStringList lines;
    QString simbol=";";
    QString formatDate="dd-MM-yyyy";
    QString formatTime="h:m";
    bool fileLoaded=false;
    int colDate=0;
    int colTime=1;
    int colData=2;
    int rowStart=0;
    int rows=100;
signals:
private slots:
    void redraw();
    void redrawFormatDate(const QString &name);
    void redrawFormatTime(const QString &name);
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
    QComboBox *ftime;       //Выбор формата времени
    QLabel    *lcolumns;    //Сколько всего колонок

    QLineEdit *lname;           //Имя перекрестка
    QLineEdit *ldesc;           //Описание проекта
    QLineEdit *lregion;         //Регион
    QLineEdit *larea;           //Район
    QLineEdit *lsubarea;        //Подрайон
    QLineEdit *lstep;           //Интервал
    QLineEdit *lchanel;         //Число каналов

    QTextEdit *ltext;
    QTableWidget *wtable;

    Project *project;
    QList<QVector<QString> >    tableMatrix;
};

#endif // READCSVFILE_H
