#include "istochart.h"
#include <QtCore/QtMath>
#include <QtCore/QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtCore/QString>
#include <QLegendMarker>
#include <QTimer>
#include <time.h>
#include <sstream>
#include <QtCharts/QChartGlobal>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QMap>
#include <QTextStream>
#include <QValueAxis>
#include <QLabel>
#include <QSize>
#include <QPushButton>
#include <QButtonGroup>

QT_CHARTS_USE_NAMESPACE

istochart::istochart(QWidget *parent, long long mac_id, QString mac, time_t beg, time_t ending) :
    QChartView(parent)
{
    QSqlQuery qry;
    QString query;
    int lastInterval=0;
    QList<long long> intervals;
    int intervalsNum=0;
    setRenderHint(QPainter::Antialiasing);

    long long end = ending;
    long long begg = beg;
    long long diff = (end-begg)/30;
    for(int i=0; i<diff; i++){
        intervalsNum++;
        intervals.push_back(begg+(i*30));
    }

    QVariant qv = end-begg;
    m_scatter = new QLineSeries;

    chart()->addSeries(m_scatter);
    chart()->legend()->markers(m_scatter)[0]->setVisible(false);

    chart()->createDefaultAxes();
    chart()->axes(Qt::Horizontal).first()->setRange(0, qv);
    chart()->axes(Qt::Vertical).first()->setRange(0, 1.2);

    chart()->axes(Qt::Vertical).first()->setTitleText("Presence");
    chart()->axes(Qt::Horizontal).first()->setTitleText("Time (s)");

    chart()->axes(Qt::Vertical).first()->setLabelsVisible(false);

    chart()->setTitle(mac);

    QString start = QString::number(begg);
    QString stop  = QString::number(end);

    query= "SELECT DISTINCT timestamp "
           "FROM devices "
           "WHERE mac = \"" + QString::number(mac_id) + "\" AND timestamp > \""+  start + "\" AND timestamp < \"" + stop + "\" ORDER BY timestamp;";

    QList<long long> timestamp;
    if (qry.exec(query)) {
        while(qry.next())
            timestamp.push_back(qry.value(0).toLongLong());
    }
    else {
        qDebug() << qry.lastError() << "in istoChart::istoChart";
    }

    *m_scatter << QPointF(0, 0);
    int i=0;
    QList<bool> present;
    for(long long interval : intervals){
        present.push_back(false);
    }
    for (auto ts : timestamp) {
        if(ts<intervals[lastInterval]){
            if(!present[lastInterval]){
                *m_scatter << QPointF((ts - begg), 0);
                *m_scatter << QPointF((ts - begg), 1);
                *m_scatter << QPointF((ts - begg), 0);
                present.replace(lastInterval, true);
            }
        }
        else{
            present.replace(lastInterval, false);
            while(ts>intervals[lastInterval]){
                lastInterval++;
            }
            *m_scatter << QPointF((ts - begg), 0);
            *m_scatter << QPointF((ts - begg), 1);
            *m_scatter << QPointF((ts - begg), 0);
            present.replace(lastInterval, true);
        }
    }
    *m_scatter << QPointF(end-begg, 0);

}

istochart::~istochart()
{

}
