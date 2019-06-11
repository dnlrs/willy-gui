#include "device.h"

int device::getId() const
{
    return id;
}

void device::setId(int value)
{
    id = value;
}

double device::getX() const
{
    return x;
}

void device::setX(double value)
{
    x = value;
}

double device::getY() const
{
    return y;
}

void device::setY(double value)
{
    y = value;
}

device::device()
{

}
