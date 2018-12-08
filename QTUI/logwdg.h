#ifndef LOGWDG_H
#define LOGWDG_H
#include <QMainWindow>
#include <vector>
#include <QString>
#include <QProcess>
#include <QTextEdit>

class logwdg : public QMainWindow
{
    Q_OBJECT
public:
    logwdg(QWidget *parent = 0);
    ~logwdg();

public slots:
    void processOutput();

private:
    QProcess *server;
    QTextEdit *edit;
};

#endif // LOGWDG_H
