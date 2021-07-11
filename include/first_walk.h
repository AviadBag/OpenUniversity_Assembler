#ifndef __FIRST_WALK_H__
#define __FIRST_WALK_H__

#include "dictionary.h"

typedef dictionary symbols_table;

typedef enum first_walk_status_e
{
    FIRST_WALK_IO_ERROR,
    FIRST_WALK_NOT_ENOUGH_MEMORY,
    FIRST_WALK_PROBLEM_WITH_CODE,
    FIRST_WALK_OK
} first_walk_status;

/**
 * @brief This method does the first walk - creates a symbols table from the given file.
 * 
 * @param file_name The file to compile.
 * @param symbols_table Will point to the symbol table. SHOULD BE FREED BY dictionary_free().
 * @return first_walk_status FIRST_WALK_IO_ERROR or FIRST_WALK_NOT_ENOUGH_MEMORY or FIRST_WALK_PROBLEM_WITH_CODE or FIRST_WALK_OK.
 */
first_walk_status first_walk(char* file_name, symbols_table* symbols_table_p);

#endif