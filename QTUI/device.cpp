#include "device.h"

long long device::getId() const
{
    return id;
}

void device::setId(long long value)
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


std::string device::mac2str(long long addr)
/*
 * aa:bb:cc:dd:ee:ff
 *
 * big endian (network byte order):
 *     |aa|bb|cc|dd|ee|ff|
 *      0  1  2  3  4  5
 *
 * note: since the string is built backwards also
 * the byte array is read bacwards.
 */
{
    static const char* digits = "0123456789ABCDEF";
    char rval[18]; // xx:xx:xx:xx:xx:xx'\0'

    int pos = 17;
    int mac_length = 6;
    for (int byte_index = (mac_length - 1); byte_index >= 0; byte_index--) {
        int real_byte_index = (mac_length - 1) - byte_index; // because machine is little endian
        uint8_t current_byte = ((char*) &addr)[real_byte_index];
        rval[pos--] = ':';
        rval[pos--] = digits[current_byte & 0x0f];
        current_byte >>= 4;
        rval[pos--] = digits[current_byte & 0x0f];
    }
    rval[17] = '\0';

    return std::string(rval);
}
