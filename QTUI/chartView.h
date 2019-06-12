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
    std::list<std::pair<float, long long>> probableHiddenMatching(position, std::vector<position>&);

public Q_SLOTS:
    void handleClickedPoint(const QPointF &point);
    void handleClickedPoint2(const QPointF &point);
    void updateChart(time_t, time_t);

private:
    QScatterSeries *m_scatter;
    QScatterSeries *m_scatter2;
    std::vector<device> devices;
    std::vector<position> positions;
    std::vector<position> hiddenCollection;
    std::vector<position> positions2;
    std::vector<position> hiddenPositions;
    std::vector<position> hiddenPositions2;
};

#endif // GRAPHICSVIEW_H

