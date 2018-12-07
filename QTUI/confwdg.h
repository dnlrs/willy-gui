#ifndef CONFWDG_H
#define CONFWDG_H
#include <QMainWindow>
#include <vector>
#include <QString>
#include <list>

class confwdg : public QMainWindow
{
    Q_OBJECT
public:
    confwdg(QWidget *parent = 0);
    ~confwdg();

signals:
    void teelMe(int, std::vector<std::string>, std::vector<std::string>);
    void confClicked();

public slots:
    void configurationclicked();
    void setMacs(QString);
    void setCords(QString);
    void setDev(int);

private:
    QStringList addresses;
    std::vector<int> axx;
    std::vector<int> axy;
    std::string stringaddresses;
    std::string stringcoords;
    int devicesNum;
};

#endif // CONFWDG_H
