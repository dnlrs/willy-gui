#include "macstats.h"
#include <QTextEdit>
#include <QLayout>
#include <QDebug>
#include <QComboBox>
#include <QSqlQuery>
#include <QSqlError>
#include <istochart.h>

macstats::macstats(QWidget *parent) : QComboBox(parent)
{
    connect(this, QOverload<const QString &>::of(&QComboBox::activated),
        [=](const QString &text){ showItemStat(text); });
}

macstats::~macstats()
{

}

void macstats::update(time_t start, time_t end)
{
    interval=end;
    beginning=start;
    QSqlQuery qry;
    QTextStream out(stdout);

    //if (qry.exec("SELECT distinct mac FROM devices WHERE timestamp<=" + QString::number(end) + " AND timestamp>="+ QString::number(start)))
    if (qry.exec("SELECT distinct mac FROM devices WHERE timestamp<=1557929701  AND timestamp>=1557928733"))
    {
       while(qry.next())
       {
           addItem("MAC:  "+qry.value(0).toString());
           //out << qry.value(0).toString() << endl;
       }
    }
    else
    {
        qDebug() << qry.lastError();
    }
}

void macstats::showItemStat(const QString & text)
{
    QTextStream out(stdout);
    istochart *isto;
    isto = new istochart(this, text, beginning, interval);
    win->setCentralWidget(isto);
    win->setFixedSize(500,500);
    win->show();
    out << "cliccato" << endl;
}
