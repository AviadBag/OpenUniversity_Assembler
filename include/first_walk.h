#ifndef __FIRST_WALK_H__
#define __FIRST_WALK_H__

#include "dictionary.h"

typedef dictionary symbols_table;

/**
 * @brief This method does the first walk - creates a symbols table from the given file.
 * 
 * @param file_name The file to compile.
 * @return symbols_table The symbols table of the given file. WITHOUT ".entry" labels!
 */
symbols_table first_walk(char* file_name);

#endif