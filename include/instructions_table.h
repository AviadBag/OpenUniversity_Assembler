#ifndef _INSTRUCTIONS_TABLE_H
#define _INSTRUCTIONS_TABLE_H

#include "operand_type.h"
#include "status.h"

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
    operand_type *operands_types; /* An array of operands types, of length number_of_operands */
} instruction;

/**
 * Initializes the instructions table.
 * @return NOT_ENOUGH_MEMORY if it happens, else - SUCCESS
 */
status instructions_table_init();

/**
 * Gives the instruction struct of the required instruction.
 * @param name The name of the instruction. (For example - "add").
 * @param inst A pointer to where to put the instruction struct.
 * @retrun INSTRUCTION_DOES_NOT_EXIST if it happens, else - SUCCESS.
 */
status instructions_table_get_instruction(char *name, instruction *inst);

#endif