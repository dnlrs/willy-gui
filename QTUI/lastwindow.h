#ifndef LASTWINDOW_H
#define LASTWINDOW_H
#include <QTCharts/QChartView>
#include <QtCharts/QChartGlobal>
#include <QtCharts/QChartView>
#include <QtCharts/QScatterSeries>
#include <QAreaSeries>
#include <QDateTime>
#include "device.h"
#include "position.h"
#include <vector>
#include <QLineSeries>

QT_CHARTS_USE_NAMESPACE

class lastwindow : public QChartView
{
    Q_OBJECT

public:
    lastwindow(QWidget *parent = 0);

public Q_SLOTS:
    void updateChart(time_t, time_t);

private:
    QLineSeries *top_series = nullptr;
    QLineSeries *bottom_series = nullptr;

    QAreaSeries *series = nullptr;

    int yAxisMax = 1;

    QString title;

    unsigned long long interval_width = 3; // seconds
};

#endif // LASTWINDOW_H
