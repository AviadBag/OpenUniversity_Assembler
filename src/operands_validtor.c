#include "operands_validator.h"
#include "instructions_table.h"
#include "directives_table.h"
#include "logger.h"
#include "str_helper.h"
#include "utils.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>

#define OPERANDS_VALIDATOR "OperandsValidator"
#define INVALID_OPERANDS "InvalidOperands"

#define FIRST_REGISTER 0
#define LAST_REGISTER 31

/**
 * Checks if the operands length of the given command is valid.
 * @param cmd  The commnad to check. The command name must exist.
 * @param line On what line these operands are?
 * @return VALIDATOR_INVALID or VALIDATOR_OK.
 */
validator_status validate_operands_length(command cmd, int line)
{
    instruction* inst;
    directive* dir;
    int required_number_of_operands;

    if (cmd.type == INSTRUCTION)
    {
        instructions_table_get_instruction(cmd.command_name, &inst);
        required_number_of_operands = inst->number_of_operands;
    }
    else /* Directive */
    {
        directives_table_get_directive(cmd.command_name, &dir);
        required_number_of_operands = dir->number_of_operands;
    }

    if (required_number_of_operands == DT_INFINITY)
    {
        if (cmd.number_of_operands == 0)
        {
            if (cmd.type == INSTRUCTION)
                logger_log(OPERANDS_VALIDATOR, INVALID_OPERANDS, line, "Instruction \"%s\" expects a least 1 operand, given 0", cmd.command_name);
            else /* Directive */
                logger_log(OPERANDS_VALIDATOR, INVALID_OPERANDS, line, "Directive \".%s\" expects a least 1 operand, given 0", cmd.command_name);
            return VALIDATOR_INVALID;
        }
        
        return VALIDATOR_OK;
    }

    if (required_number_of_operands != cmd.number_of_operands)
    {
        if (cmd.type == INSTRUCTION)
            logger_log(OPERANDS_VALIDATOR, INVALID_OPERANDS, line, "Instruction \"%s\" expects %d operands, given %d", cmd.command_name, required_number_of_operands, cmd.number_of_operands);
        else /* Directive */
            logger_log(OPERANDS_VALIDATOR, INVALID_OPERANDS, line, "Directive \".%s\" expects %d operands, given %d", cmd.command_name, required_number_of_operands, cmd.number_of_operands);          
        
        return VALIDATOR_INVALID;
    }

    return VALIDATOR_OK;
}

/**
 * @brief Returnes the operands types array of the given command.
 * 
 * @param command         The command. MUST EXIST.
 * @return operand_type* The operands types array.
 */
operand_type* get_operands_types(command cmd)
{
    instruction* inst;
    directive* dir;

    if (cmd.type == INSTRUCTION)
    {
        instructions_table_get_instruction(cmd.command_name, &inst);
        return inst->operands_types;
    }
    else /* A directive */
    {
        directives_table_get_directive(cmd.command_name, &dir);
        return dir->operands_types;
    }
}

/**
 * @brief This method checks if the given operand is a valid register.
 * 
 * @param operand The operand to check.
 * @param line    On what line this operand is?
 * @return validator_status VALIDATOR_INVALID or VALIDATOR_OK
 */
validator_status validate_register_operand(char* operand, int line)
{
    int length;
    int register_value;

    /* Make sure that the register is written well */
    if (operand[0] != '$')
    {
        logger_log(OPERANDS_VALIDATOR, INVALID_OPERANDS, line, "A register name must start with '$'");
        return VALIDATOR_INVALID;
    }

    length = strlen(operand);
    if (length < 2 || length > 3)
    {
        logger_log(OPERANDS_VALIDATOR, INVALID_OPERANDS, line, "The register length must be between 2 and 3");
        return VALIDATOR_INVALID;
    }

    /* Make sure that the register name is valid */
    operand++; /* Point to the register number itself */
    if (sscanf(operand, "%d", &register_value) != 1)
    {
        logger_log(OPERANDS_VALIDATOR, INVALID_OPERANDS, line, "The register value must be a number");
        return VALIDATOR_INVALID;
    }

    if (register_value < FIRST_REGISTER || register_value > LAST_REGISTER)
    {
        logger_log(OPERANDS_VALIDATOR, INVALID_OPERANDS, line, "Register number %d is out of range. Must be between %d and %d", register_value, FIRST_REGISTER, LAST_REGISTER);
        return VALIDATOR_INVALID;
    }

    return VALIDATOR_OK;
}

/**
 * @brief Checks if the given operand is a valid constant.
 * 
 * @param operand The operand to check.
 * @param width   The width that the operand should be. (In bytes). Must be less than long size.
 * @param line    On what line this operand is?
 */
validator_status validate_constant(char* operand, int width, int line)
{
    long num;
    
    if (!is_number(operand))
    {
        logger_log(OPERANDS_VALIDATOR, INVALID_OPERANDS, line, "Constant \"%s\" is not a number", operand);
        return VALIDATOR_INVALID;
    }

    num = strtol(operand, 0, 10);

    /* Check the range */
    if (!is_in_range_2_complement(num, width) || (num == LONG_MIN && errno == ERANGE) || (num == LONG_MAX && errno == ERANGE))
    {
        logger_log(OPERANDS_VALIDATOR, INVALID_OPERANDS, line, "The number %ld must fit to %d bytes in 2's complement", num, width);
        return VALIDATOR_INVALID;
    }

    return VALIDATOR_OK;
}

/**
 * @brief Checks if the given operand matchs to the given operand type.
 * 
 * @param operand The operand to check.
 * @param type    The desired operand type.
 * @param line    On what line this operand is?
 * @return validator_status VALIDATOR_OK or VALIDATOR_INVALID
 */
validator_status validate_operand_type(char* operand, operand_type type, int line)
{
    switch (type)
    {
        case REGISTER:
            return validate_register_operand(operand, line);
        case CONSTANT_BYTE:
            return validate_constant(operand, 1, line);
        case CONSTANT_HALF:
            return validate_constant(operand, 2, line);
        case CONSTANT_WORD:
            return validate_constant(operand, 4, line);
        default:
            return VALIDATOR_OK;
    }
}

/**
 * Checks if the operands types of the given command is valid.
 * @param cmd The commnad to check. The command name must exist, the number of operands must be correct.
 * @param line On what line these operands are?
 * @return VALIDATOR_INVALID or VALIDATOR_OK.
*/
validator_status validate_operands_type(command cmd, int line)
{
    operand_type* operands_types;
    int i;
    validator_status status;

    /* First, We will have to get the operands types */
    operands_types = get_operands_types(cmd);

    /* Now, check every operand */
    for (i = 0; i < cmd.number_of_operands; i++)
    {
        if ((status = validate_operand_type(cmd.operands[i], operands_types[i], line)) != VALIDATOR_OK)
            return status;
    }

    return VALIDATOR_OK;
}

validator_status validate_operands(command cmd, int line)
{
    validator_status status;

    if ((status = validate_operands_length(cmd, line)) != VALIDATOR_OK)
        return status;

    if ((status = validate_operands_type(cmd, line)) != VALIDATOR_OK)
        return status;

    return VALIDATOR_OK;
}