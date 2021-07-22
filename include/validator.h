#ifndef _VALIDATOR_H
#define _VALIDATOR_H

/**
 * The validator module - validates that a given command is valid. DOES NOT MAKE SURE THAT USED LABEL EXIST!
 */

#include "parser.h"

typedef enum e_validator_status
{
    VALIDATOR_INVALID,
    VALIDATOR_OK
} validator_status;

/**
 * Checks if the given command is valid.
 * @param cmd  The command to check.
 * @param line On what line this command is?
 * @return VALIDATOR_INVALID or VALIDATOR_OK.
 */
validator_status validator_validate(command cmd, int line);

/**
 * Converts a status object to it's string representation.
 * @param status The status to convert.
 * @return It's string representation.
 */
char* validator_status_to_string(validator_status status);

#endif