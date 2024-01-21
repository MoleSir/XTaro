#include "math.hh"

namespace xtaro::util
{
    int power(int base, unsigned int exponent)
    {
        int value = 1;
        for (unsigned int i = 0; i < exponent; ++i)
            value *= base;
        return value;
    }

    unsigned int fullBitsNumber(unsigned int size)
    {
        unsigned int value {0x00000000};
        for (unsigned int i = 0; i < size; ++i)
            value = (value << 1) + 1;
        return value;
    }

    double deviation(double v1, double v2)
    {
        if (v1 > v2) return v1 - v2;
        else return v2 - v1;
    }
}