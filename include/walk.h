#ifndef __WALK_H__
#define __WALK_H__

#include "command.h"
#include "linked_list.h"

#include <stdio.h>

/* This module holds of the code that both walks use */

#define PC_DEFAULT_VALUE 100
#define DC_DEFAULT_VALUE 0

#define LINE_MAX_LENGTH 80

typedef linked_list symbols_table;

typedef enum walk_status_e
{
    WALK_IO_ERROR,
    WALK_NOT_ENOUGH_MEMORY,
    WALK_PROBLEM_WITH_CODE,
    WALK_EOF,
    WALK_OK
} walk_status;

/**
 * @brief Updates pc and dc according to the given command.
 * 
 * @param pc  The program counter
 * @param dc  The data counetr
 * @param cmd The next command. MUST BE VALIDATED.
 */
void next_counter(unsigned long *pc, unsigned long *dc, command cmd);

/**
 * @brief Returns the next command from the given file, parsed and validated.
 * 
 * @param f              The file to read from. Returns WALK_EOF when reaching EOF.
 * @param cmd            A pointer to where to insert the command into.
 * @param line           What line is it?
 * @return walk_status - WALK_NOT_ENOUGH_MEMORY or WALK_PROBLEM_WITH_CODE or WALK_EOF or WALK_OK.
 */
walk_status get_next_command(FILE* f, command* cmd, int line_number);

#endif 