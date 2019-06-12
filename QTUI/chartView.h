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
    chartview(QWidget *parent = nullptr);
    ~chartview();
    std::list<std::pair<float, long long>> probableHiddenMatching(position, std::vector<position>);

public Q_SLOTS:
    void handleClickedPoint(const QPointF &point);
    void handleClickedPoint2(const QPointF &point);
    void updateChart(time_t, time_t);

private:
    QScatterSeries *m_scatter;
    QScatterSeries *m_scatter2;

    std::vector<device> devices;

    // usd when updating chart then cleared
    std::vector<position> positions;
    std::vector<position> hiddenPositions;

    // backup of positions and hiddenPositions,
    // used for retrieving labels' data
    std::vector<position> positions2;
    std::vector<position> hiddenPositions2;

    // all hidden mac, used for device hidden device matching
    std::vector<position> hiddenCollection;

    double xAxisMax = 1;
    double yAxisMax = 1;
};

#endif // GRAPHICSVIEW_H

