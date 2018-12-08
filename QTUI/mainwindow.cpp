#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "chartview.h"
#include <QLineEdit>
#include <QHBoxLayout>
#include <QRect>
#include <QSpinBox>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionImport_Positions_triggered()
{

}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::on_actionShow_Logs_triggered()
{
    log.show();
}

void MainWindow::on_actionConfiguration_triggered()
{
    wdg.show();
}
