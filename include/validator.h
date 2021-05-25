#ifndef _VALIDATOR_H
#define _VALIDATOR_H

/**
 * The validator module - validates that a given command is valid.
 */

#include "parser.h"

typedef enum e_validator_status
{
    VALIDATOR_INVALID,
    VALIDATOR_OK
} validator_status;

/**
 * Checks if the given command is valid.
 * @param cmd The command to check.
 * @return VALIDATOR_INVALID or VALIDATOR_OK.
 */
validator_status validator_validate(command cmd);

#endif