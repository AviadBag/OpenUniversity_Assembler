#ifndef _STR_HELPER_H
#define _STR_HELPER_H

/**
 * This module contains useful string functions.
 */

#include "boolean.h"

/**
 * Moves the given string to the first non-whitespace char
 * @param ptr A pointer to the string.
 */
void skip_whitespaces(char** ptr);

/**
 * Cheks if the given string has whitespaces inside.
 * @param str The string to check;
 * @return true if it has; else - false.
 */
boolean has_whitespaces(char* str);

#endif