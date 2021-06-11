#include "validator.h"
#include "command.h"
#include "instructions_table.h"
#include "directives_table.h"
#include "str_helper.h"
#include <ctype.h>

/**
 * Checks if the given label is valid.
 * @param label The label to check. Must be not empty!
 * @return VALIDATOR_INVALID or VALIDATOR_OK.
 */
validator_status validate_label(char *label)
{
    instruction* inst;
    directive* dir;

    /* A label must start with an higher or lower char. */
    if (!isalpha(label[0]))
        return VALIDATOR_INVALID;

    /* A label cannot be a reserved word. */
    if (instructions_table_get_instruction(label, &inst) == IT_OK || directives_table_get_directive(label, &dir) == DT_OK)
        return VALIDATOR_INVALID;

    /* There cannot be a whitespace in the label */
    if (has_whitespaces(label))
        return VALIDATOR_INVALID;

    return VALIDATOR_OK;
}

/**
 * Checks if the given command name exists - as a directive (TODO) or as a command.
 * @param command_name The command name to check. Must be not empty!
 * @param type         The type of the command - DIRECTIVE or INSTRUCTION.
 * @return VALIDATOR_INVALID or VALIDATOR_OK.
 */
validator_status validate_command_name(char *command_name, command_type type)
{
    instruction* inst;

    if (type == INSTRUCTION)
    {
        if (instructions_table_get_instruction(command_name, &inst) == IT_INSTRUCTION_NOT_FOUND)
            return VALIDATOR_INVALID;
    }
    else if (type == DIRECTIVE)
    {
        /* TODO */
    }
    else
        return VALIDATOR_INVALID;

    return VALIDATOR_OK;
}

/**
 * Checks if the operands length of the given command is valid.
 * @param cmd The commnad to check. The command name must exist.
 * @return VALIDATOR_INVALID or VALIDATOR_OK.
 */
validator_status validate_operands_length(command cmd)
{
    instruction* inst;

    if (cmd.type == INSTRUCTION)
    {
        instructions_table_get_instruction(cmd.command_name, &inst);
        if (inst->number_of_operands != cmd.number_of_operands)
            return VALIDATOR_INVALID;
    }
    else /* Directive */
    {
        /* TODO */
    }

    return VALIDATOR_OK;
}

/**
 * Checks if the operands of the given command are valid.
 * @param cmd The commnad to check. The command name must exist.
 * @return VALIDATOR_INVALID or VALIDATOR_OK.
 */
validator_status validate_operands(command cmd)
{
    validator_status status;

    if ((status = validate_operands_length(cmd)) != VALIDATOR_OK)
        return status;

    return VALIDATOR_OK;
}

validator_status validator_validate(command cmd)
{
    validator_status status;

    if (command_has_label(cmd))
        if ((status = validate_label(cmd.label)) != VALIDATOR_OK)
            return status;

    if ((status = validate_command_name(cmd.command_name, cmd.type)) != VALIDATOR_OK)
        return status;

    if ((status = validate_operands(cmd)) != VALIDATOR_OK)
        return status;

    return VALIDATOR_OK;
}

char *validator_status_to_string(validator_status status)
{
    switch (status)
    {
    case VALIDATOR_OK:
        return "VALIDATOR_OK";
    case VALIDATOR_INVALID:
        return "VALIDATOR_INVALID";
    default:
        return "INVALID_VALIDATOR_STATUS_OBJECT";
    }
}