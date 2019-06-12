#include "lastwindow.h"
#include <QtCore/QtMath>
#include <QtCore/QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtCore/QString>
#include <QLegendMarker>
#include <time.h>
#include <string>
#include <QtCharts/QChartGlobal>
#include <QtCharts/QChartView>
#include <QtCharts/QScatterSeries>
#include <QVector>
#include <QMap>
#include <QTextStream>
#include <QLabel>
#include <QSize>
#include <QPushButton>
#include <QButtonGroup>

QT_CHARTS_USE_NAMESPACE

lastwindow::lastwindow(QWidget *parent) :
    QChartView(parent)
{
    top_series = new QLineSeries();
    bottom_series = new QLineSeries();
    series = new QAreaSeries(top_series, bottom_series);

    chart()->addSeries(series);
    chart()->legend()->markers(series)[0]->setVisible(false);

    setRenderHint(QPainter::Antialiasing);

    chart()->createDefaultAxes();
    chart()->axes(Qt::Horizontal).first()->setRange(0, 1);
    chart()->axes(Qt::Vertical).first()->setRange(0, yAxisMax);

    chart()->setTitle("Working...");
}

void lastwindow::updateChart(time_t start, time_t end)
{
    // in real time consider only last 5 minutes
    if (start == 0) {
        start = end - (5*60);
    }

    unsigned long long intervalNum = (end-start) / interval_width;
    chart()->axes(Qt::Horizontal).first()->setRange(0, intervalNum + 1);

    QVector<int> devicesCount;
    devicesCount.resize(int(intervalNum + 1));

    // read from DB the number of distinc mac for each interval
    QSqlQuery qry;
    QString query = "SELECT timestamp, COUNT(DISTINCT mac) "
                    "FROM devices "
                    "WHERE timestamp < \"" +QString::number(end) + "\" AND timestamp >\"" + QString::number(start) + "\" "
                    "GROUP BY timestamp;";

    if (qry.exec(query)) {
        while (qry.next()) {
            unsigned long long timestamp = qry.value(0).toULongLong();
            int devices = qry.value(1).toInt();

            int interval = int((timestamp - start) / interval_width);
            devicesCount[interval] += devices;
        }
    }
    else {
        qDebug() << qry.lastError() << "in lastwindow::updateChart, first query";
    }

    // read from DB the total number of distinct devices
    // within all considered intervals
    int totalDevicesNum = 0;
    query = "SELECT COUNT(DISTINCT mac)"
            "FROM devices "
            "WHERE timestamp < \"" +QString::number(end) + "\" AND timestamp >\"" + QString::number(start) + "\";";

    if (qry.exec(query)) {
        if (qry.next()) {
            totalDevicesNum = qry.value(0).toInt();
        }
    }
    else {
        qDebug() << qry.lastError() << "in lastwindow::updateChart, second query";
    }

    top_series->clear();
    for (int i = 0; i < int(intervalNum); i++) {
        *top_series << QPointF(i, 0)
                    << QPointF(i, devicesCount[i])
                    << QPointF(i+1, devicesCount[i])
                    << QPointF(i+1, 0);

        if (devicesCount[i] >= yAxisMax) {
            yAxisMax = devicesCount[i];
            chart()->axes(Qt::Vertical).first()->setMax(yAxisMax + 1);
        }
    }

    bottom_series->clear();
    *bottom_series << QPointF(0, 0) << QPointF((intervalNum + 1), 0);
    chart()->axes(Qt::Horizontal).first()->setMax(intervalNum);

    // update tooltip
    chart()->setTitle("Total number of devices: " + QString::number(totalDevicesNum));

}
