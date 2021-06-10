#ifndef _DIRECTIVES_TABLE_H
#define _DIRECTIVES_TABLE_H

#include "operand_type.h"

typedef enum e_directives_table_status
{
    /* DT stands for "Directives Table". */
    DT_DIRECTIVE_DOES_NOT_EXIST,
    DT_OK
} directives_table_status;

typedef struct s_directive
{
    char *name;                   /**< The name of the directive */
    int number_of_operands;       /**< How many operands does this directive get? -1 means one operand in the array, but the directive gets infinity number of operands of this types. */
    operand_type *operands_types; /**< An array of operands types, of length number_of_operands. */
} directive;

/**
 * Gives the directive struct of the required directive.
 * @param name The name of the directive. (For example - "db").
 * @param dir  A pointer to a pointer of instruction struct. Will point to the returned struct.
 * @return DT_DIRECTIVE_DOES_NOT_EXIST or DT_OK.
 */
directives_table_status directives_table_get_directive(char *name, directive **dir);

#endif