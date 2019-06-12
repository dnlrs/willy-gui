#include "position.h"

QString position::getHash() const
{
    return hash;
}

void position::setHash(const QString &value)
{
    hash = value;
}

double position::getX() const
{
    return x;
}

void position::setX(double value)
{
    x = value;
}

double position::getY() const
{
    return y;
}

void position::setY(double value)
{
    y = value;
}

int position::getRssi() const
{
    return rssi;
}

void position::setRssi(int value)
{
    rssi = value;
}

QString position::getSsid() const
{
    return ssid;
}

void position::setSsid(const QString &value)
{
    ssid = value;
}

long long position::getMac() const
{
    return mac;
}

void position::setMac(long long value)
{
    mac = value;
}

int position::getChannel() const
{
    return channel;
}

void position::setChannel(int value)
{
    channel = value;
}

unsigned long long position::getTimestamp() const
{
    return timestamp;
}

void position::setTimestamp(unsigned long long value)
{
    timestamp = value;
}

int position::getSequence_number() const
{
    return sequence_number;
}

void position::setSequence_number(int value)
{
    sequence_number = value;
}

position::position()
{

}

