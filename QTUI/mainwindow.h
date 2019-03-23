#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <confwdg.h>
#include <logwdg.h>
#include <QMainWindow>
#include <QProcess>
#include <QTimer>
#include <QDateTime>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionImport_Positions_triggered();

    void on_actionExit_triggered();

    void on_actionShow_Logs_triggered();

    void on_actionConfiguration_triggered();

    void update();

    void sliderManagment(int);

    void swapMode();

    void statStopChange(QDateTime);

    void statStartChange(QDateTime);

    void statistiche();

    void restart();

    void mminizio(int);

    void mmfine(int);

    void gginizio(int);

    void ggfine(int);

    void hhinizio(int);

    void hhfine(int);
private:
    Ui::MainWindow *ui;
    confwdg wdg;
    logwdg log;
    QProcess* server;
    QTimer *timer;
    int sliderValue;
    time_t beginning;
    time_t end;
    bool statsMode;
};

#endif // MAINWINDOW_H
