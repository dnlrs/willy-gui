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
    int end= static_cast<int>(ending);
    int begg= static_cast<int>(beg);
    QVariant qv= end-begg;
    m_scatter = new QLineSeries;
    chart()->addSeries(m_scatter);
    chart()->legend()->markers(m_scatter)[0]->setVisible(false);
    chart()->createDefaultAxes();
    chart()->axisX()->setRange(0, qv);
    chart()->axisY()->setRange(0, 1);
    for(int i=begg; i<end; i++){
        query= "SELECT * FROM devices WHERE mac="+ mac +" AND timestamp="+ i;
        if (qry.exec(query))
        {
           if(qry.first())
           {
               k++;
               n++;
               if(k==3){
                *m_scatter << QPointF( i, 1);
               }
               if(n==5){
                n=0;
                k=0;
               }
           }
           else {
               n++;
               if(n==5){
                *m_scatter << QPointF( i, 0);
                n=0;
                k=0;
               }
           }
        }
        else
        {
            qDebug() << qry.lastError();
        }
    }
}
istochart::~istochart()
{

}
