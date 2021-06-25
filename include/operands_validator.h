#ifndef _OPERANDS_VALIDATOR_H
#define _OPERANDS_VALIDATOR_H

#include "validator.h"
#include "command.h"

/* This module holds of the operands validation functions */

/**
 * Checks if the operands of the given command are valid.
 * @param cmd The commnad to check. The command name must exist.
 * @return VALIDATOR_INVALID or VALIDATOR_OK.
 */
validator_status validate_operands(command cmd);

#endif