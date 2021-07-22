#ifndef _UTILS_H
#define _UTILS_H

#include "boolean.h"
#include "walk.h"
#include "symbol.h"

/* This module is just a collection of useful functions */

/**
 * @brief Checks if the give number is in the range of the given bits, with the 2's complement method
 * @param num The number to check.
 * @param byte The desired width.
 * @return True if it is in range; Else - false.
 */
boolean is_in_range_2_complement(long num, int bits);

/**
 * @brief Puts the given number in the given char array, in the given index.
 * 
 * @param arr   The char array
 * @param num   The number.
 * @param size  How many bytes of the number to put? (Little endian!)
 * @param index From what index to start?
 */
void put_in_char_array(unsigned char *arr, long num, int size, int index);

/**
 * @brief Finds a symbol in the symbols table, according to it's name.
 * 
 * @param name     The name of the symbol.
 * @param st       The symbols table to search in.
 * @return symbol* A pointer to the found symbol; NULL if not found.
 */
symbol *find_symbol(char *name, symbols_table st);

#endif