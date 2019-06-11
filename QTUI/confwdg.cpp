#include "confwdg.h"
#include <QMainWindow>
#include <QWidget>
#include <QTextStream>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include <QGridLayout>
#include <QLineEdit>
#include <QFile>
#include <QMessageBox>


confwdg::confwdg(QWidget *parent) : QMainWindow(parent)
{
    devicesNum = 3;
    this->setWindowTitle("Configuration");
    QWidget *wdg = new QWidget(this);
    this->setCentralWidget(wdg);
    QLabel *label = new QLabel;
    label->setText ("Numero dispositivi: ");
    label->setGeometry(5, 10, 8, 60);
    QSpinBox *lineedit = new QSpinBox;
    lineedit->setGeometry(15, 10, 8, 60);
    lineedit->setValue(3);
    QLabel *label2 = new QLabel;
    label2->setText ("Indirizzi MAC: ");
    label2->setGeometry(25, 10, 8, 60);
    QLineEdit *lineedit2 = new QLineEdit;
    lineedit2->setGeometry(35, 10, 8, 60);
    lineedit2->setPlaceholderText("AAAAAAAA, BBBBBBBB, ...");
    QRegExp rx("^([0-9a-fA-F]{2}[:-]){5}[0-9a-fA-F]{2}(,([0-9a-fA-F]{2}[:-]){5}[0-9a-fA-F]{2})*$");
    lineedit2->setValidator(new QRegExpValidator(rx, lineedit));
    QLabel *label3 = new QLabel;
    label->setGeometry(45, 10, 8, 60);
    label3->setText ("Coordinate: ");
    QLineEdit *lineedit3 = new QLineEdit;
    lineedit3->setGeometry(55, 10, 8, 60);
    lineedit3->setPlaceholderText("(x1,y1);(x2,y2);...");
    QPushButton *btn= new QPushButton;
    btn->setText("Configura");
    QGridLayout *HLayout = new QGridLayout;
    HLayout->addWidget (label, 0,0);
    HLayout->addWidget (lineedit, 0,1);
    HLayout->addWidget (label2, 1,0);
    HLayout->addWidget (lineedit2, 1, 1);
    HLayout->addWidget (label3, 2, 0);
    HLayout->addWidget (lineedit3, 2, 1);
    HLayout->addWidget(btn, 4,0);
    wdg->setLayout (HLayout);
    connect(btn, SIGNAL(released()), this, SLOT(configurationclicked()));
    connect(lineedit, SIGNAL(valueChanged(int)), this, SLOT(setDev(int)));
    connect(lineedit2, SIGNAL(textChanged(QString)), this, SLOT(setMacs(QString)));
    connect(lineedit3, SIGNAL(textChanged(QString)), this, SLOT(setCords(QString)));
}

confwdg::~confwdg()
{

}

void confwdg::configurationclicked()
{
    QString fileName = "C:/Users/aiman/Desktop/wifi_watchdog_analyzer_gui/server/config/configuration.txt";
    QFile file(fileName);
    if (!file.open(QIODevice::ReadWrite)) {
        return;
    }
    QString message = QString::number(devicesNum)+"\n";
    for(unsigned long long i=0; i<addresses.size(); i++){
        message+=addresses[i]+" "+QString::number(axx[i])+" "+QString::number(axy[i])+"\n";
    }
    QTextStream stream( &file );
    stream << message << endl;
}

void confwdg::setMacs(QString text)
{
    QTextStream out;
    addresses=text.split(",");
    out << "MACS:" << endl;
    for(QString add: addresses){
        out << add << endl;
    }
}

void confwdg::setCords(QString text)
{
    QStringList coords;
    QStringList depuratedCords;
    QTextStream out;
    coords=text.split(";");
    out << "COORDS:" << endl;
    for(QString c : coords){
        c.remove("(");
        c.remove(")");
        depuratedCords= c.split(",");
        if(depuratedCords.size()>1){
            axx.push_back(depuratedCords.front().toInt());
            depuratedCords.pop_front();
            axy.push_back(depuratedCords.front().toInt());
            depuratedCords.pop_front();
            out << axx.front() << endl;
            out << axy.front() << endl;
        }
    }
}

void confwdg::setDev(int val)
{
    QTextStream out(stdout);
    devicesNum= val;
    out << val << endl;
}
