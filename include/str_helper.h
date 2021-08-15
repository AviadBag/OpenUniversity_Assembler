#ifndef _STR_HELPER_H
#define _STR_HELPER_H

/**
 * This module contains useful string functions.
 */

#include "boolean.h"

/**
 * @brief Moves the given string to the first non-whitespace char
 * @param ptr A pointer to the string.
 */
void skip_whitespaces(char** ptr);

/**
 * @brief Checks if the given string is really a number.
 * @param str The string to check.
 * @return true if it is a number; else - false.
 */
boolean is_number(char* str);

#endif
