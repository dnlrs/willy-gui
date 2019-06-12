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
    connect(this, QOverload<int>::of(&QComboBox::activated),
        [=](int index){ showItemStat(index); });
}

macstats::~macstats()
{

}

void macstats::update(time_t start, time_t end)
{
    // in real time consider only last 5 minutes
    if (start == 0) {
        start = end - (60*5);
    }

    // update own variables
    interval  = end;
    beginning = start;

    // query database
    QSqlQuery qry;
    QString query = "SELECT mac, COUNT(DISTINCT timestamp) AS freq "
                    "FROM devices "
                    "WHERE timestamp < \"" + QString::number(end) + "\" AND timestamp > \""+ QString::number(start) + "\" "
                    "GROUP BY mac "
                    "ORDER BY freq DESC;";

    if (qry.exec(query)) {

        // remove all items
        while (count() > 0) {
            removeItem(0);
        }

        // insert new items
        int i = 0;
        while(qry.next() && i < 10) {
           long long mac     = qry.value(0).toLongLong();
           QString   mac_str = QString::fromStdString(device::mac2str(mac));
           int frequency = qry.value(1).toInt();
           addItem(mac_str + " (" + QString::number(frequency) + ")", mac);
           i++;
       }
    }
    else {
        qDebug() << qry.lastError() << "in macstats::update";
    }
}

void macstats::showItemStat(int index)
{
    QString text = itemText(index);
    long long mac_id = itemData(index).toLongLong();

    istochart *isto;
    isto = new istochart(this, mac_id, text, beginning, interval);

    win->setCentralWidget(isto);
    win->setFixedSize(500,500);
    win->show();
}
