#include "bitmap.h"

#define BITS_PER_BYTE 8

#define OFFSET_IN_BITMAP(i) (((index) / (sizeof(char) * BITS_PER_BYTE)))
#define OFFSET_IN_BYTE(i) (((index) % (sizeof(char) * BITS_PER_BYTE)))

/**
 * @brief Get <bitmap>[<index>]
 * 
 * @param _bitmap The bitmap
 * @param index   What bit?
 * @return char   The bit at <bitmap>[<index>]
 */
static char get_bit(void* _bitmap, int index)
{
    char* bitmap = (char*) _bitmap;
    bitmap += OFFSET_IN_BITMAP(index); /* Go the the right byte */
    return (*bitmap & (1 <<  OFFSET_IN_BYTE(index))) > 0;
}

/**
 * @brief Sets <bitmap>[<index>] to <bit>.
 * 
 * @param _bitmap The bitmap
 * @param index   What bit?
 * @param b       The bit to put at <bitmap>[<index>]
 */
static void set_bit(void* _bitmap, int index, bit b)
{
    char* bitmap = (char*) _bitmap;
    bitmap += OFFSET_IN_BITMAP(index); /* Go the the right byte */
    *bitmap |= b << OFFSET_IN_BYTE(i);
}

void bitmap_put_data(void* bitmap, void* data, int from, int to) 
{
    int i;

    if (from > to) /* Invalid */
        return;

    for (i = 0; from <= to; from++, i++)
    {
        bit b;

        b = get_bit(data, i);
        set_bit(bitmap, from, b);
    }
}
