#include "utils.h"

#include <math.h>

boolean is_in_range_2_complement(long num, int bytes)
{
    int bits = bytes * 8;
    long max = pow(2, bits-1) - 1;
    long min = -pow(2, bits-1);

    return (num <= max && num >= min);
}