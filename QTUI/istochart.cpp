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
#include <QLabel>
#include <QSize>
#include <QPushButton>
#include <QButtonGroup>

QT_CHARTS_USE_NAMESPACE

istochart::istochart(QWidget *parent, QString mac, time_t beg, time_t ending) :
    QChartView(parent)
{
    QSqlQuery qry;
    QString query;
    int k=0;
    int n=0;
    QTextStream out(stdout);
    setRenderHint(QPainter::Antialiasing);
    out << "ISTOCHART CREATO" << endl;
    //ending = 1557929701;
    //beg = 1557928733;
    long long end= ending;
    long long begg= beg;
    QVariant qv= end-begg;
    m_scatter = new QLineSeries;
    chart()->addSeries(m_scatter);
    chart()->legend()->markers(m_scatter)[0]->setVisible(false);
    chart()->createDefaultAxes();
    chart()->axisX()->setRange(0, qv);
    chart()->axisY()->setRange(0, 1);
    QString start  = QString::number(begg);
    QString stop  = QString::number(end);
    QList<QString> timestamp;
    query= "SELECT * FROM devices WHERE mac=" + mac.mid(6) +" AND timestamp>"+  start + " AND timestamp<"+stop;
    qDebug() << query << "";
    if (qry.exec(query))
    {
        while(qry.next())
            timestamp.push_back(qry.value(1).toString());
    }
    else
    {
        qDebug() << qry.lastError() << query;
    }
    long diff= end-begg;
    for(long i=0; i<diff; i++){
            if(timestamp.contains(QString::number(begg+i)))
            {
                //k++;
                //n++;
                //if(k==3){
                 *m_scatter << QPointF( i, 1);
                //}
                //if(n==5){
                 //n=0;
                 //k=0;
                //}
            }
            else {
                //n++;
                //if(n==5){
                 *m_scatter << QPointF( i, 0);
                 //n=0;
                 //k=0;
                //}
            }
    }
}
istochart::~istochart()
{

}
