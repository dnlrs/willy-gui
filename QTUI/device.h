#ifndef DEVICE_H
#define DEVICE_H


class device
{
private:
    int id;
    double x;
    double y;
public:
    device();
    int getId() const;
    void setId(int value);
    double getX() const;
    void setX(double value);
    double getY() const;
    void setY(double value);
};

#endif // DEVICE_H
