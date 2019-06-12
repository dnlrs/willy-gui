#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "chartView.h"
#include <QLineEdit>
#include <QHBoxLayout>
#include <QRect>
#include <QSpinBox>
#include <QSqlDatabase>
#include <QTextStream>
#include <QDebug>
#include <QtCore/QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDir>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QTextStream out(stdout);
    QDateTime datetime;
    datetime.setTime_t(uint(time(nullptr)));
    statsMode = false;

    beginning = time(nullptr);
    end = time(nullptr);

    QString s = datetime.toString("yyyy-MM-dd  HH:mm:ss");
    sliderValue=0;

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("../server/database.db");

    bool ok = db.open();
    if(!ok){
        out << "not connected" << endl;
    }
    else{
        //out << "Connected" << endl;
    }

    ui->setupUi(this);
    ui->endTime->setDateTime(datetime);
    ui->startTime->setDateTime(datetime);
    ui->inizio->display(s);
    ui->fine->display(s);

    out << QDir::currentPath() << endl;

    ui->comboBox->update(this->beginning, this->end);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(3000);
}

void MainWindow::update()
{
    QDateTime datetime;
    end = time(nullptr);
    datetime.setTime_t(uint(time(nullptr)));
    QString s = datetime.toString("yyyy-MM-dd  HH:mm:ss");

    ui->fine->display(s);
    ui->chartView->updateChart(0, end);
    ui->comboBox->update(0, end);
    ui->graphicsView->updateChart(0, end);
}

void MainWindow::sliderManagment(int newVal)
{
    int diff= newVal-sliderValue;
    diff=diff*3000;
    sliderValue=newVal;
    long long t = time(nullptr);
    t= t+diff;
    QDateTime datetime;
    datetime.setTime_t(uint(t));
    QString s = datetime.toString("yyyy-MM-dd  HH:mm:ss");
    ui->fine->display(s);
}

void MainWindow::swapMode(){
    ui->label_16->setText("");
    if(!statsMode){
        ui->label_2->setEnabled(true);
        ui->label_3->setEnabled(true);
        //ui->label_6->setEnabled(true);
        //ui->label_5->setEnabled(true);
        ui->startTime->setEnabled(true);
        ui->pushButton->setEnabled(true);
        ui->endTime->setEnabled(true);
        //ui->horizontalSlider_2->setEnabled(true);
        ui->pushButton_2->setText("Exit Stats");
        statsMode= true;
    }
    else{
        ui->label_2->setEnabled(false);
        ui->label_3->setEnabled(false);
        ui->label_5->setEnabled(false);
        ui->startTime->setEnabled(false);
        ui->endTime->setEnabled(false);
        ui->pushButton->setEnabled(false);
        ui->pushButton_2->setText("Stats");
        ui->pushButton_3->setEnabled(true);
        statsMode= false;
        ui->pushButton_2->setEnabled(true);
    }
}

void MainWindow::statStartChange(QDateTime time){
    QDateTime datetime;
    datetime = ui->endTime->dateTime();
    //datetime.setTime_t(end);
    if(datetime> time){
        ui->pushButton->setEnabled(true);
    }
    else {
        ui->pushButton->setEnabled(false);
    }
}

void MainWindow::statistiche()
{
    timer->stop();
    if( ui->startTime->dateTime().toTime_t() < ui->endTime->dateTime().toTime_t()){
        ui->label_16->setText("");

        beginning = ui->startTime->dateTime().toTime_t();
        end = ui->endTime->dateTime().toTime_t();

        QDateTime datetime;
        datetime.setTime_t(uint(beginning));
        QString s = datetime.toString("yyyy-MM-dd  HH:mm:ss");
        ui->inizio->display(s);

        datetime.setTime_t(uint(end));
        s = datetime.toString("yyyy-MM-dd  HH:mm:ss");
        ui->fine->display(s);

        ui->chartView->updateChart(beginning, end);
        ui->comboBox->update(beginning, end);
        ui->graphicsView->updateChart(beginning, end);

        ui->comboBox->setEnabled(true);
        ui->pushButton_3->setEnabled(false);
    }
    else {
        ui->label_16->setText("Invalid interval.");
    }
}

void MainWindow::restart()
{
    QDateTime datetime;
    datetime.setTime_t(uint(time(nullptr)));
    beginning = time(nullptr);
    end = time(nullptr);
    QString s = datetime.toString("yyyy-MM-dd  HH:mm:ss");
    ui->inizio->display(s);
    ui->fine->display(s);
    timer->start(3000);
}

void MainWindow::mminizio(int mm)
{
    QDateTime datetime;
    if(beginning+(mm*60)<end){
        beginning+= (mm*60);
        datetime.setTime_t(beginning);
        QString s = datetime.toString("yyyy-MM-dd  HH:mm:ss");
        ui->inizio->display(s);
        ui->startTime->setDateTime(datetime);

    }
    else{
        ui->label_16->setText("Inizio periodo precedente\n alla fine.");
    }
}

void MainWindow::mmfine(int mm)
{
    QDateTime datetime;
    if(end+(mm*60)>beginning){
        end+= (mm*60);
        datetime.setTime_t(end);
        QString s = datetime.toString("yyyy-MM-dd  HH:mm:ss");
        ui->fine->display(s);
        ui->endTime->setDateTime(datetime);

    }
    else
    {
        ui->label_16->setText("Inizio precedente\n a fine.");
    }
}

void MainWindow::gginizio(int gg)
{
    QDateTime datetime;
    if(beginning + (gg*86400)<end){
        beginning+= (gg*86400);
        datetime.setTime_t(beginning);
        QString s = datetime.toString("yyyy-MM-dd  HH:mm:ss");
        ui->inizio->display(s);
        ui->startTime->setDateTime(datetime);
    }
    else {
        ui->label_16->setText("Periodo di inizio precedente\n a fine");
    }
}

void MainWindow::ggfine(int gg)
{
    QDateTime datetime;
    if(end + (gg*86400)>beginning){
        end+= (gg*86400);
        datetime.setTime_t(end);
        QString s = datetime.toString("yyyy-MM-dd  HH:mm:ss");
        ui->fine->display(s);
        ui->endTime->setDateTime(datetime);

    }else {
        ui->label_16->setText("Periodo di inizio precedente\n a fine.");
    }
}

void MainWindow::hhinizio(int hh)
{
    QDateTime datetime;
    if(beginning+(hh*3600)<end){
        beginning+= (hh*3600);
        datetime.setTime_t(beginning);
        QString s = datetime.toString("yyyy-MM-dd  HH:mm:ss");
        ui->inizio->display(s);
        ui->startTime->setDateTime(datetime);

    }
    else{
        ui->label_16->setText("Periodi inizio precedente\n a periodo fine");
    }
}

void MainWindow::hhfine(int hh)
{
    QDateTime datetime;
    if(end+(hh*3600)>beginning){
        end+= (hh*3600);
        datetime.setTime_t(end);
        QString s = datetime.toString("yyyy-MM-dd  HH:mm:ss");
        ui->fine->display(s);
        ui->endTime->setDateTime(datetime);
    }
    else{
        ui->label_16->setText("Periodo inizio precedente\n a periodo fine.");
    }
}

void MainWindow::statStopChange(QDateTime time){
    QDateTime datetime;
    //datetime.setTime_t(beginning);
    datetime = ui->startTime->dateTime();
    if(!(datetime> time)){
        ui->pushButton->setEnabled(true);
    }
    else {
        ui->pushButton->setEnabled(false);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
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
