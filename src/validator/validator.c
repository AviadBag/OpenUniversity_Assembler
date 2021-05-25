#include "validator.h"
#include "command.h"
#include "instructions_table.h"
#include <ctype.h>

/**
 * Checks if the given label is legal.
 * @param label The label to check. Must be not empty!
 * @return VALIDATOR_INVALID or VALIDATOR_OK.
 */
validator_status validate_label(char label[LABEL_MAX_LENGTH])
{
    instruction inst;
    
    /* A label must start with an higher or lower char. */
    if (!isalpha(label[0]))
        return VALIDATOR_INVALID;

    /* A label cannot be a reserved word */
    if (instructions_table_get_instruction(label, &inst) == IT_OK)
        return VALIDATOR_INVALID;

    return VALIDATOR_OK;
}

validator_status validator_validate(command cmd)
{
    validator_status status;
    
    if (command_has_label(cmd))
        if ((status = validate_label(cmd.label)) != VALIDATOR_OK)
            return status;

    return VALIDATOR_OK;
}