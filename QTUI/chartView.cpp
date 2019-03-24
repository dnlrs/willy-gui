#include "chartview.h"
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
#include <QTextStream>
#include <QLabel>
#include <QSize>
#include <QPushButton>
#include <QButtonGroup>

QT_CHARTS_USE_NAMESPACE

chartview::chartview(QWidget *parent) :
    QChartView(parent)
{
    QTextStream out(stdout);
    float maxX=0;
    float maxY=0;
    device dev;
    setRenderHint(QPainter::Antialiasing);
    QSqlQuery qry;
    if (qry.exec("SELECT * FROM devices"))
    {
       while(qry.next())
       {
           dev.setX(qry.value(1).toFloat());
           dev.setId(qry.value(0).toInt());
           dev.setY(qry.value(2).toFloat());
           devices.push_back(dev);
       }
    }
    else
    {
        qDebug() << qry.lastError();
    }
    for(device d : devices){
        if(d.getX()>maxX){
            maxX=d.getX();
        }
        if(d.getY()>maxY){
            maxY=d.getX();
        }
    }
    m_scatter = new QScatterSeries();
    chart()->addSeries(m_scatter);
    chart()->legend()->markers(m_scatter)[0]->setVisible(false);
    chart()->createDefaultAxes();
    chart()->axisX()->setRange(0, maxX+5); //todo inserire costante giusta
    chart()->axisY()->setRange(0, maxY+5);
    connect(m_scatter, &QScatterSeries::clicked, this, &chartview::handleClickedPoint);
}

void chartview::handleClickedPoint(const QPointF &point)
{
    QPointF clickedPoint = point;
    QSqlQuery qry;
    // Find the closest point from series 1
    QPointF closest(INT_MAX, INT_MAX);
    qreal distance(INT_MAX);
    const auto points = m_scatter->points();
    for (const QPointF &currentPoint : points) {
        qreal currentDistance = qSqrt((currentPoint.x() - clickedPoint.x())
                                      * (currentPoint.x() - clickedPoint.x())
                                      + (currentPoint.y() - clickedPoint.y())
                                      * (currentPoint.y() - clickedPoint.y()));
        if (currentDistance < distance) {
            distance = currentDistance;
            closest = currentPoint;
        }
    }
    QString query= "SELECT * FROM devices WHERE x==" +
                    QString::number(closest.x()) + " AND y==" +
                    QString::number(closest.y());
    QTextStream out(stdout);
    out << query << endl;
    QWidget *wdg = new QWidget;
    wdg->resize(400,300);
    QLabel *label= new QLabel(wdg);
    if (qry.exec(query))
    {
         qry.first();
         label->setText("MAC: "+ qry.value(0).toString() +".\n"
                        "X: "+ qry.value(1).toString() +".\n"
                        "Y: "+ qry.value(2).toString() +".\n"
                        //"RSSI: "+ qry.value(3).toString() +".\n"
                        //"SSID: "+ qry.value(4).toString() +".\n"
                        //"MAC: "+ qry.value(5).toString() +".\n"
                        "TIMESTAMP: "+ qry.value(3).toString() +".\n");
                        //"SEQUENCE NUMBER: "+ qry.value(7).toString() +".\n");
    }
    else
    {
        qDebug() << qry.lastError();
    }

    wdg->show();
}

void chartview::updateChart(time_t beginning, time_t end){
    m_scatter->clear();
    QSqlQuery qry, qry2;
    position p;
    QTextStream out(stdout);
    QString query;
    query="SELECT DISTINCT mac, pos_x , pos_y FROM devices WHERE timestamp<" +QString::number(end) + " AND timestamp>" + QString::number(beginning)+" GROUP BY mac HAVING MAX(timestamp)";

    if (qry.exec(query))
    {
       while(qry.next())
       {
            query="SELECT * FROM packets WHERE mac=" + qry.value(0).toString() + "AND timestamp=" + qry.value(3).toString();
            if (qry2.exec(query))
            {
               if(qry2.first())
               {
                    p.setHash(qry2.value(0).toString());
                    p.setX(qry.value(1).toFloat());
                    p.setY(qry.value(2).toFloat());
                    p.setRssi(qry2.value(3).toInt());
                    p.setSsid(qry2.value(4).toString());
                    p.setMac(qry2.value(5).toString());
                    //ChartView::positions[i].setChannel(qry.value(6).toInt());
                    p.setTimestamp(qry2.value(7).toInt());
                    p.setSequence_number(qry2.value(8).toInt());
                    positions.push_back(p);
                }
            }
            else
            {
                qDebug() << qry2.lastError() << "\t" << query;
            }
       }
    }
    else
    {
        qDebug() << qry.lastError()<< "\t" << query;
    }
    for(position p : positions){
        *m_scatter << QPointF( p.position::getX(), p.position::getY());
    }
    positions.clear();
}

chartview::~chartview()
{

}

