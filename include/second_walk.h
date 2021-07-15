#ifndef __SECOND_WALK_H__
#define __SECOND_WALK_H__

#include "walk.h"
#include "linked_list.h"

/* This moulde implements the second walk, which gets as an input a symbols table, and returns:
    1. Data image + DCF
    2. Code image + ICF
    3. For every symbol, fills the is_entry field.
    4. For every symbol, fills the instructions_using_me field.
*/

/**
 * @brief Does the second walk.
 * 
 * @param file_name       The name of the input file.
 * @param symbols_table_p A pointer to the given symbols table.
 * @param data_image_p    A pointer to a linked list that will represent the data image.
 * @param dcf_p           A pointer to where to store the dcf after the second walk.
 * @param code_image_p    A pointer to a linked list that will represent the code image.
 * @param icf_p           A pointer to where to store the icf after the second walk.
 * @return walk_status WALK_NOT_ENOUGH_MEMORY or WALK_IO_ERROR or WALK_OK.
 */
walk_status second_walk(char* file_name, symbols_table *symbols_table_p, linked_list *data_image_p, int *dcf_p, linked_list *code_image_p, int *icf_p);

#endif