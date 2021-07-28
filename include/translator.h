#ifndef _TRANSLATOR_H
#define _TRANSLATOR_H

#include "command.h"
#include "walk.h"

/* This moudle translates a single assembly instruction to a 32 bit machine language instruction */

typedef unsigned int machine_instruction; /* Represents an instruction in machine language */ 

typedef enum e_translator_status
{
    TRANSLATOR_OK,
    TRANSLATOR_LABEL_DOES_NOT_EXIST,
    TRANSLATOR_OVERFLOW
} translator_status;

/**
 * This method translates the given command into a machine code.
 * @param cmd                  The command to translate. cmd.command_type MUST be INSTRUCTION, and must be validated by the validator.
 * @param st                   The symbols table.
 * @param ic                   The current instruction counter.
 * @param line                 On what line this instruction is?
 * @param m                    A pointer to a machine_instruction; Will be filled with the instruction.
 * @return translator_status   TRANSLATOR_OK or TRANSLATOR_LABEL_DOES_NOT_EXIST or TRANSLATOR_OVERFLOW
 */
translator_status translator_translate(command cmd, symbols_table st, unsigned long ic, int line, machine_instruction* m);

#endif
