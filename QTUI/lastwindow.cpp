#include "lastwindow.h"
#include <QtCore/QtMath>
#include <QtCore/QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtCore/QString>
#include <QLegendMarker>
#include <time.h>
#include <sstream>
#include <QtCharts/QChartGlobal>
#include <QtCharts/QChartView>
#include <QtCharts/QScatterSeries>
#include <vector>
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
    m_scatter = new QLineSeries;
    chart()->addSeries(m_scatter);
    chart()->legend()->markers(m_scatter)[0]->setVisible(false);
    QTextStream out(stdout);
    setRenderHint(QPainter::Antialiasing);
    QSqlQuery qry;
    chart()->createDefaultAxes();
    chart()->axisX()->setRange(0, 200); //todo inserire costante giusta
    chart()->axisY()->setRange(0, 200);
    //connect(m_scatter, &QScatterSeries::clicked, this, &chartview::handleClickedPoint);
}

void lastwindow::updateChart(time_t beginning, time_t end){
    int intervalnum=(end-beginning)/300;
    chart()->axisX()->setRange(0, intervalnum);
    QSqlQuery qry;
    QMap<QString, QList<time_t>> macsTimes;
    QTextStream out(stdout);
    QString query;
    QList<time_t> intervals;
    QList<int> numsDevices;
    for(int i=0; i<intervalnum; i++){
        intervals.insert(1000, (beginning+300*i));
        numsDevices.insert(1000,0);
    }
    query="SELECT * FROM devices WHERE timestamp<" +QString::number(end) + " AND timestamp>" + QString::number(beginning);

    if (qry.exec(query))
    {
       while(qry.next())
       {
           if(macsTimes.contains(qry.value(0).toString())){
               macsTimes.find(qry.value(0).toString())->insert(1000, qry.value(1).toLongLong());
           }
           else {
               QList<time_t> tmp;
               tmp.insert(0,qry.value(1).toLongLong());
               macsTimes.insert(qry.value(0).toString(), tmp);
           }
       }
    }
    else
    {
        qDebug() << qry.lastError()<< "\t" << query;
    }
    QMapIterator<QString, QList<time_t>> itermap(macsTimes);
    while(itermap.hasNext())
    {
       itermap.next();
       bool found=false;
       QList<time_t> tmptimes= itermap.value();
       int k=0;
       for(int i=0; i<tmptimes.size(); i+=k){
           for(int j=0; j<intervals.size(); j++){
               if(tmptimes[i]>intervals[j] && tmptimes[i]<intervals[j+1]){
                   for(k=i; tmptimes[k]<intervals[j+1]; k++){
                       if(intervals[j+1]-tmptimes[k]<30){
                           numsDevices[j+1]++;
                           break;
                       }
                   }
                   found=true;
               }
               if(found){
                   found=false;
                   break;
               }
           }
       }
       //qDebug() << Iter.key() << Iter.value();
    }
    for(int i=0; i<intervals.size(); i++){
        *m_scatter << QPointF( i, numsDevices[i]);
    }
}
