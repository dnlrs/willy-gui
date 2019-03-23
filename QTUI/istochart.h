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
    istochart(QWidget *parent = 0, QString mac="", time_t beginning=time(NULL), time_t ending=time(NULL));
    ~istochart();
private:
    QLineSeries *m_scatter;
};
