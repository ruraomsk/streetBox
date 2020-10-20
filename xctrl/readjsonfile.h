#ifndef READJSONFILE_H
#define READJSONFILE_H

#include <QDialog>
#include <QObject>
#include <QFileDialog>
#include <QMessageBox>
#include <QStringList>
#include <QLineEdit>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QFormLayout>
#include <QPushButton>
#include <QHBoxLayout>
#include "../support.h"
#include "../setup.h"
#include "../project/project.h"
#include "xctrl.h"
extern Setup ini;

class ReadJSONFile : public QDialog
{
public:
    ReadJSONFile(Project *project,QWidget *parent = nullptr);

private slots:
    void setData();
private:
    bool isNameCorrect(QString name);
    QLineEdit *lname;           //Имя XT
    QStringList lines;
    Xctrl *xt;
    Project *project;
};

#endif // READJSONFILE_H
