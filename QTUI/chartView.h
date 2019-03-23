#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H
#include <QTCharts/QChartView>
#include <QtCharts/QChartGlobal>
#include <QtCharts/QChartView>
#include <QtCharts/QScatterSeries>
#include <QDateTime>
#include "device.h"
#include "position.h"
#include <vector>

QT_CHARTS_USE_NAMESPACE

class chartview : public QChartView
{
    Q_OBJECT

public:
    chartview(QWidget *parent = 0);
    ~chartview();

public Q_SLOTS:
    void handleClickedPoint(const QPointF &point);
    void updateChart(time_t, time_t);

private:
    QScatterSeries *m_scatter;
    std::vector<device> devices;
    std::vector<position> positions;
};

#endif // GRAPHICSVIEW_H
