#ifndef _TRANSLATOR_H
#define _TRANSLATOR_H

#include "command.h"

/* This moudle translates a single assembly instruction to a 32 bit machine language instruction */

typedef unsigned int machine_instruction; /* Represents an instruction in machine language */ 

/**
 * This method translates the given command into a machine code.
 * @param cmd The command to translate. cmd.command_type MUST be INSTRUCTION, and must be validated by the validator.
 * @return machine_instruction The translated instruction.
 */
machine_instruction translator_translate(command cmd);

#endif
