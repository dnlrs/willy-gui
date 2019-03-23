#ifndef MACSTATS_H
#define MACSTATS_H
#include <QMainWindow>
#include <vector>
#include <QString>
#include <QProcess>
#include <QTextEdit>
#include <QComboBox>

class macstats : public QComboBox
{
    Q_OBJECT
public:
    macstats(QWidget *parent = 0);
    ~macstats();

public slots:
    void update(time_t start, time_t end);
    void showItemStat(const QString & text);

private:
    QMainWindow *win = new QMainWindow(this);
    time_t interval;
    time_t beginning;
};
#endif // MACSTATS_H
