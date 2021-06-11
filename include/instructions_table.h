#ifndef _INSTRUCTIONS_TABLE_H
#define _INSTRUCTIONS_TABLE_H

/**
 * This module implements the instructions table - a table which gives you the information of every command.
 */

#include "operand_type.h"

typedef enum e_instruction_type
{
    R,
    I,
    J
} instruction_type;

typedef struct s_instruction
{
    char* name;                   /**< The name of this instruction. For example = "add". */

    instruction_type type;        /**< The type of this instruction - R or I or J. */
    int funct;                    /**< The funct of this instruction - if exists; 0 otherwise. */
    int opcode;                   /**< The opcode of this instruction. */

    int number_of_operands;       /**< How many operands does this instruction get? */
    operand_type *operands_types; /**< An array of operands types, of length number_of_operands. NULL if there are no operands. */
} instruction;

typedef enum e_instructions_table_status
{
    IT_NOT_ENOUGH_MEMORY, /* IT stands for Instructions Table. */
    IT_INSTRUCTION_NOT_FOUND,
    IT_OK
} instructions_table_status;

/**
 * Gives the instruction struct of the required instruction.
 * @param name The name of the instruction. (For example - "add").
 * @param inst A pointer to a pointer of instruction struct. Will point to the returned struct.
 * @return  IT_INSTRUCTION_NOT_FOUND if it happens, else - IT_OK.
 * MUST BE CALLED AFTER instructions_table_init(0)!
 */
instructions_table_status instructions_table_get_instruction(char *name, instruction **inst);

#endif