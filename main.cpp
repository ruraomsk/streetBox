#include "mainwindow.h"

#include <QApplication>
#include <QDebug>
#include <QList>
#include <QRandomGenerator>
#include <QStyleFactory>

Setup ini("streetBox.ini");

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
        MainWindow w;
        w.setStyle(QStyleFactory::create("Fusion"));
        w.setWindowTitle(ini.getString("name"));
        w.setWindowIcon(QIcon("/images/streetBox.png"));

        w.show();
        return a.exec();
}
