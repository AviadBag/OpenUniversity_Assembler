#ifndef __SECOND_WALK_H__
#define __SECOND_WALK_H__

#include "walk.h"
#include "linked_list.h"

/* This module implements the second walk, which gets as an input a symbols table, and returns:
    1. Data image + DCF
    2. Code image + ICF
    3. For every symbol, fills the is_entry field.
    4. For every symbol, fills the instructions_using_me field.
*/

/**
 * @brief Does the second walk.
 * 
 * @param file_name       The name of the input file.
 * @param symbols_table_p A pointer to the given symbols table. For every extern symbol, st->instructions_using_me shuold be freed using linked_list_free, and it's items should be freed.
 * @param data_image      A pointer to where to put the address of the data image. SHOULD BE FREED AFTER USAGE, unless it is NULL.
 * @param dcf_p           A pointer to where to store the dcf after the second walk.
 * @param code_image      A pointer to where to put the address of the code image. SHOULD BE FREED AFTER USAGE, unless it is NULL.
 * @param icf_p           A pointer to where to store the icf after the second walk.
 * @return walk_status WALK_NOT_ENOUGH_MEMORY or WALK_IO_ERROR or WALK_PROBLEM_WITH_CODE or WALK_OK.
 */
walk_status second_walk(char* file_name, symbols_table *symbols_table_p, unsigned char **data_image, unsigned long *dcf_p, unsigned char **code_image, unsigned long *icf_p);

#endif
