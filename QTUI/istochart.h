#include <QTCharts/QChartView>
#include <QtCharts/QChartGlobal>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QDateTime>
#include "device.h"
#include "position.h"


QT_CHARTS_USE_NAMESPACE

class istochart : public QChartView
{
    Q_OBJECT

public:
    istochart(QWidget *parent = 0);
    ~istochart();

signals:
    void resetDisplay();

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
    void updateChart();

private:
    QLineSeries *m_scatter;
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
