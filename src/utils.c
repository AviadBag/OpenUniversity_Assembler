#include "utils.h"
#include "string.h"
#include "symbol.h"
#include "linked_list.h"

#include <math.h>

#define FIRST_BYTE_MASK 0xFF /* Helps to extract the first byte from a number, by doing (num & FIRST_BYTE_MASK). */
#define BITS_IN_BYTE 8       /* How many bits does each byte contain? */

boolean is_in_range_2_complement(long num, int bytes)
{
    int bits = bytes * 8;
    long max = pow(2, bits-1) - 1;
    long min = -pow(2, bits-1);

    return (num <= max && num >= min);
}

void put_in_char_array(unsigned char *arr, long num, int size, int index)
{
    int byte;
    for (byte = 0; byte < size; byte++) /* Iterate on every required byte */
    {
        arr[index] = (char)(num & FIRST_BYTE_MASK);
        index++;
        num >>= BITS_IN_BYTE;
    }
}

symbol *find_symbol(char *name, symbols_table st)
{
    int i;
    for (i = 0; i < linked_list_length(st); i++)
    {
        symbol *symbol_p = linked_list_get(st, i);
        if (strcmp(name, symbol_p->name) == 0)
            return symbol_p;
    }

    return NULL;
}