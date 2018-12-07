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

signals:
    void updated(QString);
    void resetDisplay();
    void updatedFine(QString);

public slots:
    void myStop();
    void myChangeValue(int);
    void myChangeValue2(int);
    void myRestart();
    void myStats();
    void myStart();
    void myStatsStart(QDateTime);
    void myStatsStop(QDateTime);

private Q_SLOTS:
    void handleClickedPoint(const QPointF &point);
    void updateChart();

private:
    QScatterSeries *m_scatter;
    std::vector<device> devices;
    std::vector<position> positions;
    time_t interval;
    time_t beginning;
    QDateTime statStart;
    QDateTime statStop;
    QTimer *timer;
    bool reset=false;
    bool slider;
    bool spinSlider;
    bool stat;
    bool timetravel;
};

#endif // GRAPHICSVIEW_H
