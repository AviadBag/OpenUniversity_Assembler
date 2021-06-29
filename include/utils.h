#ifndef _UTILS_H
#define _UTILS_H

#include "boolean.h"

/* This module is just a collection of useful functions */

/**
 * @brief Checks if the give number is in the range of the given bytes, with the 2's complement method
 * @param num The number to check.
 * @param byte The desired width.
 * @return True if it is in range; Else - false.
 */
boolean is_in_range_2_complement(long num, int bytes);

#endif