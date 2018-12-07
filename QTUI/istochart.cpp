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

istochart::istochart(QWidget *parent) :
    QChartView(parent)
{
    interval = time (NULL);
    slider=false;
    stat=false;
    spinSlider=false;
    beginning = time(NULL);
    QTextStream out(stdout);
    device dev;
    setRenderHint(QPainter::Antialiasing);

    timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(updateChart()));
        timer->start(1000);

    m_scatter = new QLineSeries;
    chart()->addSeries(m_scatter);
    chart()->legend()->markers(m_scatter)[0]->setVisible(false);
    chart()->createDefaultAxes();
    chart()->axisX()->setRange(0, 10); //todo inserire costante giusta
    chart()->axisY()->setRange(0, 3);
}

void istochart::updateChart(){
    m_scatter->clear();
    QSqlQuery qry;
    QMap<int, int> countXtimestamp;
    QTextStream out(stdout);
    QString query;
    if(!slider){
        if(stat){
            query="SELECT timestamp, count(DISTINCT mac) AS counting FROM positions GROUP BY timestamp";
            //WHERE timestamp<" +
            //(QString::number(interval) + " AND timestamp>" +
            //QString::number(beginning) + "GROUP BY timestamp";
        }
        else{
            if(timetravel){
                beginning++;
                if(beginning>interval){
                    int tmp=beginning;
                    beginning=interval;
                    interval=tmp;
                    spinSlider=false;
                    timetravel=false;
                }
                query = "SELECT timestamp, count(DISTINCT mac) AS counting FROM positions GROUP BY timestamp";
                                //WHERE timestamp<" +
                                //(QString::number(beginning)+300) + " AND timestamp>" +
                                //QString::number(beginning) GROUP BY timestamp;
            }
            else{
                time_t now = time(NULL);
                query = "SELECT timestamp, count(DISTINCT mac) AS counting FROM positions GROUP BY timestamp";
                                //WHERE timestamp<" +
                                //(QString::number(beginning)+300) + " AND timestamp>" +
                                //QString::number(beginning);
                if(interval>now){
                    resetDisplay();
                }
                beginning= now;

                if(reset){
                    resetDisplay();
                    reset=false;
                }
            }
        }
    }
    else{
        query= "SELECT timestamp, count(DISTINCT mac) AS counting FROM positions GROUP BY timestamp";
                        //WHERE timestamp<" +
                        //QString::number(interval) + " AND timestamp>" +
                        //QString::number(beginning);
    }
    if (qry.exec(query))
    {
       while(qry.next())
       {
           //out << "valori delle query" << endl;
           //out << qry.value(0).toInt() << endl;
           //out << qry.value(1).toInt() << endl;
           countXtimestamp.insert(qry.value(0).toInt(), qry.value(1).toInt());
       }
    }
    else
    {
        qDebug() << qry.lastError();
    }
    int k=0;
    QMap<int, int>::const_iterator i = countXtimestamp.constBegin();
    while (i != countXtimestamp.constEnd()){
        k++;
        *m_scatter << QPointF( (time(NULL)-beginning+k), i.value());
        //out << "valori delle chiavi" << endl;
        //out << time(NULL)- beginning+k << endl;
        //out << i.value() << endl;
         ++i;
    }
    stat=false;
    slider=false;
    countXtimestamp.clear();
}

void istochart::myRestart()
{
    interval=time(NULL);
    beginning=time(NULL);
    updateChart();
    QTextStream out(stdout);
    out << "timer started." << endl;
    timer->start();
}

void istochart::myStop()
{
    timer->stop();
    QTextStream out(stdout);
    out << "timer stopped." << endl;
}

void istochart::myChangeValue(int val)
{
    timer->stop();
    if(!spinSlider){
        interval-= val;
        if(interval==time(NULL)){
            resetDisplay();
            reset=false;
        }
    }
    else
        beginning-=val;
    if(beginning>interval){
        int tmp= beginning;
        beginning=interval;
        interval=tmp;
        spinSlider=true;
    }
    if(interval<time(NULL) && spinSlider){
        timetravel=true;
        reset=true;
    }
    slider=true;
    updateChart();
}

void istochart::myChangeValue2(int val)
{
    timer->stop();
    if(!spinSlider){
        interval+= val;
        if(interval==time(NULL))
            resetDisplay();
            reset=false;
    }
    else{
        beginning+=val;
    }
    if(beginning>interval){
        int tmp= beginning;
        beginning=interval;
        interval=tmp;
        spinSlider=false;
    }
    slider=true;
    updateChart();
}

void istochart::myStats(){
    beginning= statStart.toTime_t();
    interval= statStop.toTime_t();
    QTextStream out(stdout);
    timer->stop();
    out << beginning << endl;
    out << interval << endl;
    stat=true;
    updateChart();
}

void istochart::myStart()
{
    timer->start();
}

void istochart::myStatsStart(QDateTime time)
{
    statStart= time;
}

void istochart::myStatsStop(QDateTime time)
{
    statStop=time;
}

istochart::~istochart()
{

}

