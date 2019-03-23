#include "mainwindow.h"
#include <QApplication>
#include <QSqlDatabase>
#include <QTextStream>
#include <QDebug>

int main(int argc, char *argv[])
{
    QTextStream out(stdout);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
