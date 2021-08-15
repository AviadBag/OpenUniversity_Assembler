#include "validator.h"
#include "command.h"
#include "instructions_table.h"
#include "directives_table.h"
#include "str_helper.h"
#include "operands_validator.h"
#include "logger.h"
#include <ctype.h>

#define VALIDATOR "Validator"
#define INVALID_COMMAND "InvalidCommand"

/**
 * Checks if the given label is valid.
 * @param label The label to check. Must be not empty!
 * @param line On what line is this label?
 * @return VALIDATOR_INVALID or VALIDATOR_OK.
 */
validator_status validate_label(char *label, int line)
{
    instruction* inst;
    directive* dir;

    /* A label must start with a upper or lower char. */
    if (!isalpha(label[0]))
    {
        logger_log(VALIDATOR, INVALID_COMMAND, line, "A label must begin with a upper or lower char!");
        return VALIDATOR_INVALID;
    }

    /* A label cannot be a reserved word. */
    if (instructions_table_get_instruction(label, &inst) == IT_OK || directives_table_get_directive(label, &dir) == DT_OK)
    {
        logger_log(VALIDATOR, INVALID_COMMAND, line, "A label cannot be named \"%s\", because it is already a reserved word", label);
        return VALIDATOR_INVALID;
    }

    /* The label must have only chars and digits */
    while (*label)
    {
        if (!isalnum(*label) && !isdigit(*label))
        {
            logger_log(VALIDATOR, INVALID_COMMAND, line, "A label cannot contain other than chars and digits");
            return VALIDATOR_INVALID;
        }
        
        label++;
    }

    return VALIDATOR_OK;
}

/**
 * Checks if the given command name exists - as a directive (TODO) or as a command.
 * @param command_name The command name to check. Must be not empty!
 * @param type         The type of the command - DIRECTIVE or INSTRUCTION.
 * @param line         On what line is this command name?
 * @return VALIDATOR_INVALID or VALIDATOR_OK.
 */
validator_status validate_command_name(char *command_name, command_type type, int line)
{
    instruction* inst;
    directive* dir;

    if (type == INSTRUCTION)
    {
        if (instructions_table_get_instruction(command_name, &inst) == IT_INSTRUCTION_NOT_FOUND)
        {
            logger_log(VALIDATOR, INVALID_COMMAND, line, "Instruction \"%s\" does not exist", command_name);
            return VALIDATOR_INVALID;
        }
    }
    else /* Directive */
    {
        if (directives_table_get_directive(command_name, &dir) == DT_DIRECTIVE_DOES_NOT_EXIST)  
        {
            logger_log(VALIDATOR, INVALID_COMMAND, line, "Directive \"%s\" does not exist", command_name);
            return VALIDATOR_INVALID;
        }
    }

    return VALIDATOR_OK;
}

validator_status validator_validate(command cmd, int line)
{
    validator_status status;

    if (command_has_label(cmd))
        if ((status = validate_label(cmd.label, line)) != VALIDATOR_OK)
            return status;

    if ((status = validate_command_name(cmd.command_name, cmd.type, line)) != VALIDATOR_OK)
        return status;

    if ((status = validate_operands(cmd, line)) != VALIDATOR_OK)
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