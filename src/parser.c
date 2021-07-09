#include "parser.h"
#include "boolean.h"
#include "str_helper.h"
#include "logger.h"

#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_LABEL 31

#define PARSER "Parser"
#define SYNTAX_ERROR "SyntaxError"
#define OVERFLOW_ERROR "OverflowError"

/**
 * Checks if there is a label inside of the given string.
 * @param str The string to check.
 * @return if there is a label.
 */
static boolean has_label(char *str)
{
    boolean inside_quotes = false;

    while (*str)
    {
        if (*str == '"')
            inside_quotes = !inside_quotes;
        else if (*str == ':')
            return true;

        str++;
    }

    return false;
}

/**
 * Parses the label in the command. Increments the given string to after the label.
 * @param str     A pointer to the pointer of the string to parse. MUST NOT BE EMPTY OR COMMENT
 * @param command The command object to parse into.
 * @param line    On what line is this string?
 * @return PARSER_SYNTAX_ERROR or PARSER_OVERFLOW or PARSER_OK.
 */
static parser_status parse_label(char **str, command *cmd, int line)
{
    int label_length;

    if (has_label(*str))
    {
        skip_whitespaces(str);

        if (**str == ':')
        {
            logger_log(PARSER, SYNTAX_ERROR, line, "A label cannot be empty!");
            return PARSER_SYNTAX_ERROR;
        }

        label_length = 0;
        while (**str != ':')
        {
            label_length++;
            if (label_length > MAX_LABEL)
            {
                logger_log(PARSER, OVERFLOW_ERROR, line, "A line cannot be longer than 31 characters");
                return PARSER_OVERFLOW;
            }

            cmd->label[label_length - 1] = **str;
            (*str)++;
        }

        if (isspace(*(*str - 1)))
        {
            logger_log(PARSER, SYNTAX_ERROR, line, "The colon of the label must be right after the label");
            return PARSER_SYNTAX_ERROR;
        }

        cmd->label[label_length] = '\0';

        (*str)++; /* Point to after the label */
        if (!isspace(**str))
        {
            logger_log(PARSER, SYNTAX_ERROR, line, "There must be a space after the label");
            return PARSER_SYNTAX_ERROR;
        }
    }
    else
        cmd->label[0] = '\0';

    return PARSER_OK;
}

/**
 * Checks if the command is empty or is a comment.
 * @param str The command.
 * @return if empty.
 */
static boolean is_empty(char *str)
{
    while (*str)
    {
        if (*str == ';')
            return true;
        else if (!isspace(*str))
            return false;

        str++;
    }

    return true;
}

/**
 * Parses the command name in the command. Increments the given string to after the command name.
 * @param str     A pointer to the pointer of the string to parse, STARTING AFTER THE LABEL!
 * @param command The command object to parse into.
 * @param line    On what line is this string?
 * @return PARSER_SYNTAX_ERROR or PARSER_NOT_ENOUGH_MEMORY or PARSER_OK.
 */
static parser_status parse_command_name(char **str, command *cmd, int line)
{
    int command_name_length = 0, i;
    char *ptr;

    skip_whitespaces(str);

    if (**str == '.')
        cmd->type = DIRECTIVE;
    else
        cmd->type = INSTRUCTION;

    /* Find the length of the command name */
    ptr = *str;
    while (!isspace(*ptr) && *ptr)
    {
        command_name_length++;
        ptr++;
    }

    if (cmd->type == DIRECTIVE)
        command_name_length--; /* I don't want to put the '.' in the command name. */

    if (command_name_length == 0)
    {
        logger_log(PARSER, SYNTAX_ERROR, line, "A line must contain a command (For example - addi)");
        return PARSER_SYNTAX_ERROR;
    }

    /* Allocate buffer for the command name */
    cmd->command_name = malloc((command_name_length + 1) * sizeof(char));
    if (cmd->command_name == NULL)
        return PARSER_NOT_ENOUGH_MEMORY;

    /* Fill! */
    if (cmd->type == DIRECTIVE)
        (*str)++; /* Skip the dot if this is a directive. */
    for (i = 0; i < command_name_length; i++)
    {
        cmd->command_name[i] = **str;
        (*str)++;
    }

    /* Put terminating zero */
    cmd->command_name[command_name_length] = '\0';

    return PARSER_OK;
}

/**
 * Calculates the number of operands in the given string. Reports if there
 * was any syntax error with the operands.
 * @param str  The string to check. MUST BEGIN RIGHT FROM THE OPERANDS.
 *             There must be at least one operand!
 * @param int  A pointer to integer to fill in the number of operands.
 * @param line The line in which the error occurred.
 * @return PARSER_SYNTAX_ERROR or PARSER_OK.
 */
static parser_status get_number_of_operands(char *str, int *number_of_operands, int line)
{
    boolean inside_quotes, inside_operand, after_comma, after_quotes_end;

    *number_of_operands = 0;
    inside_quotes = false, inside_operand = false, after_comma = true, after_quotes_end = false;
    while (*str)
    {
        if (*str == '"')
        {
            inside_quotes = !inside_quotes;

            if (inside_operand && inside_quotes)
            {
                logger_log(PARSER, SYNTAX_ERROR, line, "You cannot open quotes inside of an operand");
                return PARSER_SYNTAX_ERROR;
            }

            if (!after_comma || after_quotes_end)
            {
                logger_log(PARSER, SYNTAX_ERROR, line, "There must be a comma before an operand, except the first operand");
                return PARSER_SYNTAX_ERROR;
            }

            inside_operand = true;

            if (!inside_quotes)
            {
                after_quotes_end = true;
                goto next_iteration; /* This current char doesn't need more parse */
            }
        }

        if (inside_quotes)
            goto next_iteration; /* There is nothing to parse inside of quotes. */

        if (*str == ',')
        {
            if (inside_operand)
            {
                inside_operand = false;
                after_comma = false;
            }

            if (after_comma && !inside_operand)
            {
                logger_log(PARSER, SYNTAX_ERROR, line, "There cannot be a comma after a comma");
                return PARSER_SYNTAX_ERROR;
            }

            (*number_of_operands)++;
            after_comma = true;
            inside_operand = false;
        }
        else if (!isspace(*str)) /* This is a regular operand */
        {
            if (!after_comma || after_quotes_end)
            {
                logger_log(PARSER, SYNTAX_ERROR, line, "There must be a comma before an operand, except the first operand");
                return PARSER_SYNTAX_ERROR;
            }

            inside_operand = true;
        }
        else /* This is a whitespace */
        {
            if (inside_operand)
                after_comma = false;

            inside_operand = false;
        }

        /* If it wasn't just now defined. Actually, it's not needed, because after
        after_quotes_end is setted to true - it jumps to next_iteration, but anyway
        it is a good practis to put it - for future code change. */
        if (!inside_quotes)
            after_quotes_end = false;

    next_iteration:
        str++;
    }

    /* If inside_operand is true, it means that I am RIGHT AFTER an operand, so
    then it's ok if after_comma is true, because It just means that there
    was a comma before the operand. But if I am not after a operand, and
    after_comma is true - It is a syntax error, because there shouldn't be 
    any comma after the last operand. */
    if (after_comma && !inside_operand)
    {
        logger_log(PARSER, SYNTAX_ERROR, line, "There souldn't be any comma after the last operand");
        return PARSER_SYNTAX_ERROR;
    }

    if (inside_quotes)
    {
        logger_log(PARSER, SYNTAX_ERROR, line, "You must close your quotes");
        return PARSER_SYNTAX_ERROR;
    }
    (*number_of_operands)++; /* To count the first operand */

    return PARSER_OK;
}

/**
 * Fills the operand of the given string in the given array.
 * @param str                The string to parse. MUST BEGIN RIGHT FROM THE 
 *                           OPERANDS. IT'S SYNTAX MUST BE OK!
 * @param number_of_operands How many operands there are?
 * @param operands           The array to fill in.
 * @return PARSER_NOT_ENOUGH_MEMORY or PARSER_OK;
 */
static parser_status fill_operands(char *str, int number_of_operands, char **operands_array)
{
    int i, j, operand_length;
    char *ptr, *operand; /* An helping pointer */
    boolean inside_quotes;

    for (i = 0; i < number_of_operands; i++)
    {
        ptr = str; /* Back up the string pointer */

        /* First, calculate the operand size */
        operand_length = 0;
        inside_quotes = false;
        while ((*str != ',' || inside_quotes) && *str)
        {
            if (*str == '"')
                inside_quotes = !inside_quotes;

            if (!isspace(*str) || inside_quotes)
                operand_length++;

            str++;
        }

        /* Now, insert the pointer to the array! */
        /* First, allocate memory for the operand */
        operand = malloc(operand_length * sizeof(char));
        if (operand == NULL)
            return PARSER_NOT_ENOUGH_MEMORY;

        str = ptr; /* Go back to the beginning of the operand */
        j = 0;
        inside_quotes = false;
        while ((*str != ',' || inside_quotes) && *str)
        {
            if (*str == '"')
                inside_quotes = !inside_quotes;

            if (!isspace(*str) || inside_quotes)
            {
                operand[j] = *str;
                j++;
            }

            str++;
        }

        operands_array[i] = operand;

        if (*str)
            str++; /* To skip the comma */
    }

    return PARSER_OK;
}

/**
 * Parses the operands in the command.
 * @param str     A pointer to the string to parse.
 * @param command The command object to parse into, STARTING FROM THE OPERANDS!
 * @param line    On what line is this string?
 * @return PARSER_SYNTAX_ERROR or PARSER_NOT_ENOUGH_MEMORY or PARSER_OK.
 */
static parser_status parse_operands(char *str, command *cmd, int line)
{
    parser_status status;

    skip_whitespaces(&str);
    if (!*str) /* There are no operands.. */
    {
        cmd->number_of_operands = 0;
        cmd->operands = NULL;
        return PARSER_OK;
    }

    /* Get the number of operands */
    if ((status = get_number_of_operands(str, &cmd->number_of_operands, line)) != PARSER_OK)
        return status;

    /* Allocate the array of the operands */
    cmd->operands = malloc(cmd->number_of_operands * sizeof(char *));
    if (cmd->operands == NULL)
        return PARSER_NOT_ENOUGH_MEMORY;

    /* Fill the operands in the array */
    if ((status = fill_operands(str, cmd->number_of_operands, cmd->operands)) != PARSER_OK)
        return status;

    return PARSER_OK;
}

parser_status parser_parse(char *str, command *cmd, int line)
{
    parser_status status;

    if (is_empty(str))
        return PARSER_EMPTY;
    if ((status = parse_label(&str, cmd, line)) != PARSER_OK)
        return status;
    if ((status = parse_command_name(&str, cmd, line)) != PARSER_OK)
        return status;
    if ((status = parse_operands(str, cmd, line)) != PARSER_OK)
        return status;

    return PARSER_OK;
}

char *parser_status_to_string(parser_status status)
{
    switch (status)
    {
    case PARSER_SYNTAX_ERROR:
        return "PARSER_SYNTAX_ERROR";
    case PARSER_EMPTY:
        return "PARSER_EMPTY";
    case PARSER_OVERFLOW:
        return "PARSER_OVERFLOW";
    case PARSER_OK:
        return "PARSER_OK";
    default:
        return "INVALID_PARSER_STATUS_OBJECT";
    }
}
