#ifndef DEVICE_H
#define DEVICE_H

#include <string>

class device
{
private:
    long long id;
    double x;
    double y;
public:
    device();
    long long getId() const;
    void setId(long long value);
    double getX() const;
    void setX(double value);
    double getY() const;
    void setY(double value);

    static std::string mac2str(long long addr);
};

#endif // DEVICE_H
