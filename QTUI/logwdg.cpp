#include "logwdg.h"
#include <QTextEdit>
#include <QLayout>
#include <QDebug>

logwdg::logwdg(QWidget *parent) : QMainWindow(parent)
{
    QTextStream out(stdout);
    this->setWindowTitle("Log");
    QWidget *wdg = new QWidget(this);
    this->setCentralWidget(wdg);
    edit= new QTextEdit;
    edit->setReadOnly(true);
    QGridLayout *HLayout = new QGridLayout;
    HLayout->addWidget(edit, 0,0);
    wdg->setLayout (HLayout);

    this->server = new QProcess(this);
    this->server->setWorkingDirectory("..\\server");
    out<< server->pid() << endl;
    connect(this->server, SIGNAL(readyReadStandardOutput()), this, SLOT(processOutput()));  // connect process signals with your code
    connect(this->server, SIGNAL(readyReadStandardError()), this, SLOT(processOutput()));  // same here
    server->start("..\\server\\wifi_watchdog_analyzer.exe");
    out<< server->pid() << endl;
    out << server->state() << endl;
}

logwdg::~logwdg()
{

}

void logwdg::processOutput()
{
    edit->append( server->readAllStandardOutput());
    edit->append( server->readAllStandardError());
    qDebug() << server->readAllStandardOutput();  // read normal output
    qDebug() << server->readAllStandardError();  // read error channel
}
