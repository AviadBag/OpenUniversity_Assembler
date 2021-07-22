#ifndef __FIRST_WALK_H__
#define __FIRST_WALK_H__

#include "linked_list.h"
#include "walk.h"

/**
 * @brief This method does the first walk - creates a symbols table from the given file.
 * 
 * @param file_name The file to compile.
 * @param symbols_table Will point to the symbol table. SHOULD BE FREED BY linked_list_free(), and it's symbols should be freed.
 * @return walk_status WALK_IO_ERROR or WALK_NOT_ENOUGH_MEMORY or WALK_PROBLEM_WITH_CODE or WALK_OK.
 */
walk_status first_walk(char* file_name, symbols_table* symbols_table_p);

#endif