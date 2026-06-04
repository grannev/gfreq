#include "digmath.h"

void add_bit(unsigned long *byte, char bit)
{
    *byte <<= 1;
    if (bit)
        *byte |= 1;
}

