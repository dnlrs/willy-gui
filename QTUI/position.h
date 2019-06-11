#ifndef POSITION_H
#define POSITION_H

#include <QtCore/QString>

class position
{
private:
    QString hash;
    float x;
    float y;
    int rssi;
    QString ssid;
    long long mac;
    int channel;
    int timestamp;
    int sequence_number;
public:
    position();

    QString getHash() const;
    void setHash(const QString &value);
    float getX() const;
    void setX(float value);
    float getY() const;
    void setY(float value);
    int getRssi() const;
    void setRssi(int value);
    QString getSsid() const;
    void setSsid(const QString &value);
    long long getMac() const;
    void setMac(long long);
    int getChannel() const;
    void setChannel(int value);
    int getTimestamp() const;
    void setTimestamp(int value);
    int getSequence_number() const;
    void setSequence_number(int value);
};

#endif // POSITION_H
