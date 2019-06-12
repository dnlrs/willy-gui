#ifndef POSITION_H
#define POSITION_H

#include <QtCore/QString>

class position
{
private:
    QString hash;

    double x;
    double y;
    int rssi;

    QString ssid;
    long long mac;

    int channel;
    unsigned long long timestamp;
    int sequence_number;
public:
    position();

    QString getHash() const;
    void setHash(const QString &value);
    double getX() const;
    void setX(double value);
    double getY() const;
    void setY(double value);
    int getRssi() const;
    void setRssi(int value);
    QString getSsid() const;
    void setSsid(const QString &value);
    long long getMac() const;
    void setMac(long long);
    int getChannel() const;
    void setChannel(int value);
    unsigned long long getTimestamp() const;
    void setTimestamp(unsigned long long value);
    int getSequence_number() const;
    void setSequence_number(int value);
};

#endif // POSITION_H
