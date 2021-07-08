#ifndef __BITMAP_H__
#define __BITMAP_H__

/* This module holds utility methods for bit manipulations */

typedef unsigned char bit;

/**
 * @brief Puts <data> in <bitmap>, from bit <from> to bit <to>.
 * 
 * @param bitmap The bitmap
 * @param data   The data
 * @param from   From where to start?
 * @param to     Where to stop? (including) (Must be bigger than <from>)
 */
void bitmap_put_data(void* bitmap, void* data, int from, int to);

#endif