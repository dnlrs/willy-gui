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
#include <algorithm>
#include <utility>
#include <set>
#include <QtCharts/QChartGlobal>
#include <QtCharts/QChartView>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QValueAxis>
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
    device dev;
    setRenderHint(QPainter::Antialiasing);

    m_scatter = new QScatterSeries();
    m_scatter2 = new QScatterSeries();
    m_scatter2->setColor("red");

    chart()->addSeries(m_scatter);
    chart()->addSeries(m_scatter2);

    chart()->legend()->markers(m_scatter)[0]->setVisible(false);
    chart()->legend()->markers(m_scatter2)[0]->setVisible(false);

    chart()->createDefaultAxes();

    // setup xAxis
    QValueAxis *axis = (QValueAxis *) chart()->axes(Qt::Horizontal).first();
    axis->setTickType(QValueAxis::TickType::TicksDynamic);
    axis->setTickAnchor(0);
    axis->setTickInterval(1);
    axis->setRange(0, xAxisMax);
    axis->setTitleText("Distance (m)");
    axis->setLabelFormat("%d");

    // setup yAxis
    axis = (QValueAxis *) chart()->axes(Qt::Vertical).first();
    axis->setTickType(QValueAxis::TickType::TicksDynamic);
    axis->setTickAnchor(0);
    axis->setTickInterval(1);
    axis->setRange(0, yAxisMax);
    axis->setTitleText("Distance (m)");
    axis->setLabelFormat("%d");

    // setup chart title
    chart()->setTitle("Localized Devices");

    connect(m_scatter, &QScatterSeries::clicked, this, &chartview::handleClickedPoint);
    connect(m_scatter2, &QScatterSeries::clicked, this, &chartview::handleClickedPoint2);
}

void chartview::handleClickedPoint(const QPointF &point)
{
    QPointF clickedPoint = point;

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

    QWidget *wdg = new QWidget;
    wdg->resize(400,300);

    QLabel *label= new QLabel(wdg);
    for(position p : positions2){
        if( abs(p.getX() - closest.x()) < 0.001 && abs(p.getY() - closest.y()) < 0.001){

            QDateTime datetime;
            datetime.setTime_t(uint(p.getTimestamp()));
            QString s = datetime.toString("yyyy-MM-dd  HH:mm:ss");

            label->setText("MAC:  "+ QString::fromStdString(device::mac2str(p.getMac()))+"\n"
                           "X: "+ QString::number(p.getX()) +"\n"
                           "Y: "+ QString::number(p.getY()) +"\n"
                           "RSSI: "+ QString::number(p.getRssi()) +"\n"
                           "SSID: "+ p.getSsid() +"\n"
                           "TIMESTAMP: "+ s +"\n"
                           "SEQUENCE NUMBER: "+ QString::number(p.getSequence_number()) +"\n");
        }
    }

    wdg->show();
}

void chartview::handleClickedPoint2(const QPointF &point)
{
    QPointF clickedPoint = point;

    // Find the closest point from series 1
    QPointF closest(INT_MAX, INT_MAX);
    qreal distance(INT_MAX);

    const auto points = m_scatter2->points();
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

    QWidget *wdg = new QWidget;
    wdg->resize(400,300);

    QLabel *label= new QLabel(wdg);
    for(position p : hiddenPositions2){
        if(abs(p.getX() - closest.x()) < 0.001 && abs(p.getY() - closest.y()) < 0.001){
            QDateTime datetime;
            datetime.setTime_t(uint(p.getTimestamp()));
            QString s = datetime.toString("yyyy-MM-dd  HH:mm:ss");

            std::list<std::pair<float, long long>> probableMatching;
            probableMatching = chartview::probableHiddenMatching(p, hiddenCollection);
            
            /* sort in reverse order (descending) */
            probableMatching.sort([](const std::pair<float, long long>& p1, const std::pair<float, long long>& p2) -> bool {
                return (p1.first > p2.first);
            });

            size_t n = 3;
            auto listEnd= std::next(probableMatching.begin(), std::min(n, probableMatching.size()));
            std::list<std::pair<float, long long>> max3(probableMatching.begin(), listEnd);

            QString myString = "";
            for(std::pair<float, long long> pai : max3){
                myString+= QString::fromStdString(device::mac2str(pai.second)) +": "+QString::number(pai.first) +"%\n";
            }

            label->setText("MAC: "+QString::fromStdString(device::mac2str(p.getMac())) +"\n"
                           "X: "+ QString::number(p.getX()) +"\n"
                           "Y: "+ QString::number(p.getY()) +"\n"
                           "RSSI: "+ QString::number(p.getRssi()) +"\n"
                           "SSID: "+ p.getSsid() +"\n"
                           "TIMESTAMP: "+ s +"\n"
                           "SEQUENCE NUMBER: "+ QString::number(p.getSequence_number()) +"\n"
                           "-*-*-*-*-*-*-*-PROBLABLE MATCHING-*-*-*-*-*-*-*-\n"
                           + myString
                           );
        }
    }

    wdg->show();
}

void chartview::updateChart(time_t beginning, time_t end)
{
    double xMax = 0;
    double yMax = 0;

    // in real time consider only last 3 seconds
    if (beginning == 0) {
        beginning = end - 5;
    }


    QSqlQuery qry, qry2;
    QString query;
    query = "SELECT mac as outermac, pos_x, pos_y, timestamp "
            "FROM devices "
            "WHERE timestamp = (SELECT MAX(timestamp)"
                                "FROM devices "
                                "WHERE mac = outermac AND timestamp <  \"" + QString::number(end) + "\" AND timestamp >  \"" + QString::number(beginning) + "\");";

    // clear positions and hidden positions
    positions.clear();
    hiddenPositions.clear();

    if (qry.exec(query)) {

       while(qry.next()) {
           double pos_x = qry.value(1).toDouble();
           double pos_y = qry.value(2).toDouble();

            // update axis max values
           if (pos_x > xMax) xMax = pos_x;
           if (pos_y > yMax) yMax = pos_y;

            query="SELECT * "
                  "FROM packets "
                  "WHERE mac = \"" + qry.value(0).toString() + "\" AND timestamp = \"" + qry.value(3).toString() + "\";";

            if (qry2.exec(query)) {
               if(qry2.first()) {
                    position p;

                    p.setHash(qry2.value(0).toString());
                    p.setX(pos_x);
                    p.setY(pos_y);
                    p.setRssi(qry2.value(2).toInt());
                    p.setSsid(qry2.value(1).toString());
                    p.setMac(qry2.value(3).toLongLong());
                    p.setTimestamp(qry2.value(6).toULongLong());
                    p.setSequence_number(qry2.value(5).toInt());

                    long long mac = p.getMac();
                    if(((char*) &mac)[5] & (1 << 1)) {
                        hiddenPositions.push_back(p);
                    }
                    else {
                        positions.push_back(p);
                    }
                }
            }
            else {
                qDebug() << qry2.lastError() << "in chartView::updateChart, inner query";
            }
       }
    }
    else {
        qDebug() << qry.lastError() << "in chartView::updateChart, outer query";
    }

    // update x-Axis max value if needed
    if (xMax > xAxisMax) {
        xAxisMax = xMax;
        chart()->axes(Qt::Horizontal).first()->setMax(qCeil(xMax+1));
        ((QValueAxis*) chart()->axes(Qt::Horizontal).first())->setTickCount(qCeil(xMax+1) > 5 ? qCeil(xMax+1) : 5);
    }

    // update y-Axis max value if needed
    if (yMax > yAxisMax) {
        yAxisMax = yMax;
        chart()->axes(Qt::Vertical).first()->setMax(qCeil(yMax+1));
        ((QValueAxis*) chart()->axes(Qt::Vertical).first())->setTickCount(qCeil(yMax+1) > 5 ? qCeil(yMax+1) : 5);
    }

    // update scatter series
    m_scatter->clear();
    for(position p : positions){
        *m_scatter << QPointF( p.position::getX(), p.position::getY());
    }

    // update hidden scatter series
    m_scatter2->clear();
    for(position p : hiddenPositions){
        *m_scatter2 << QPointF( p.position::getX(), p.position::getY());
    }

    // store all encountered hidden macs
    for( position p : hiddenPositions){
         hiddenCollection.push_back(p);
    }

    // backup positions and hidden positions
    positions2 = positions;
    hiddenPositions2 = hiddenPositions;
}


std::list<std::pair<float, long long>> chartview::probableHiddenMatching( position p, std::vector<position>& hiddenCollections){

    const static int SSID_WEIGHT = 50;
    const static int POS_WEIGHT = 30;
    const static int SEQ_WEIGHT = 19;

    std::list<std::pair<float, long long>> resultList;
    std::set<long long> consideredMacs;
    float curr_weight;

    for(position px : hiddenCollections){

        // ignore comparing the device with itself
        if (p.getMac() == px.getMac())
            continue;

        // ignore macs that have already been considered
        if (consideredMacs.find(px.getMac()) != consideredMacs.end())
            continue;

        curr_weight = 0;
        if(!p.getSsid().isEmpty() && p.getSsid() == px.getSsid()){
            curr_weight += SSID_WEIGHT;
        }

        if(abs(p.getX() - px.getX()) < 7 && abs(p.getY() - px.getY()) < 7)
            curr_weight += POS_WEIGHT;

        if((p.getSequence_number() - px.getSequence_number() > 0) && p.getSequence_number() - px.getSequence_number() < 10)
            curr_weight += SEQ_WEIGHT;

        std::pair<float, long long> pai;
        pai.second = px.getMac();
        pai.first = curr_weight;
        resultList.push_back(pai);

        consideredMacs.insert(pai.second);
    }

    return resultList;
}


chartview::~chartview()
{

}

