#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <confwdg.h>
#include <QMainWindow>

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

private:
    Ui::MainWindow *ui;
    confwdg wdg;
};

#endif // MAINWINDOW_H
