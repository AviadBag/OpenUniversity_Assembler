#ifndef _INSTRUCTIONS_TABLE_H
#define _INSTRUCTIONS_TABLE_H

#include "status.h"

typedef enum e_operand_type
{
    REGISTER,
    CONSTANT,
    LABEL,
    LABEL_OR_REGISTER
} operand_type;

typedef enum e_instruction_type
{
    R,
    I,
    J
} instruction_type;

typedef struct s_instruction
{
    instruction_type type;
    int funct; /* If exists; 0 otherwise. */
    int opcode;

    int number_of_operands;       /* How many operands does this instruction get? */
    /* An array of operands types, of length number_of_operands. NULL if there are no operands. */
    operand_type *operands_types;
} instruction;

/**
 * Initializes the instructions table.
 * @return NOT_ENOUGH_MEMORY if it happens, else - SUCCESS.
 */
status instructions_table_init();

/**
 * Gives the instruction struct of the required instruction.
 * @param name The name of the instruction. (For example - "add").
 * @param inst A pointer to where to put the instruction struct.
 * @retrun INSTRUCTION_DOES_NOT_EXIST if it happens, else - SUCCESS.
 * MUST BE CALLED AFTER instructions_table_init(0)!
 */
status instructions_table_get_instruction(char *name, instruction *inst);

/**
 * Frees the memory taken by the instructions table.
 * MUST BE CALLED AFTER instructions_table_init(0)!
 */
void free_instructions_table();

#endif