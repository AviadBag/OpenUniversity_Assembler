#include "parser.h"
#include "boolean.h"
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_LINE 80

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
 * @return PARSER_SYNTAX_ERROR or PARSER_OVERFLOW or PARSER_OK.
 */
static parser_status parse_label(char **str, command *cmd)
{
    int label_length;

    if (has_label(*str))
    {
        /* Jump to the first non-empty char */
        while (isspace(**str))
            (*str)++;

        if (**str == ':')
            /* A label cannot be empty! */
            return PARSER_SYNTAX_ERROR;

        label_length = 0;
        while (**str != ':')
        {
            label_length++;
            if (label_length > 31)
                return PARSER_OVERFLOW;

            cmd->label[label_length - 1] = **str;
            (*str)++;
        }

        if (isspace(*(*str - 1)))
            /* The colon must be right after the label */
            return PARSER_SYNTAX_ERROR;

        cmd->label[label_length] = '\0';

        (*str)++; /* Point to after the label */
        if (!isspace(**str))
            /* There must be a space after the label */
            return PARSER_SYNTAX_ERROR;
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
static boolean is_empty(char* str)
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
 * Checks if the command is too long.
 * @param str The command.
 * @return if too long.
 */
static boolean is_too_long(char* str)
{
    int length = 0;
    while (*str)
    {
        if (!isspace(*str))
            length++;
        
        if (length > MAX_LINE)
            return true;

        str++;
    }

    return false;
}

parser_status parser_parse(char *str, command *cmd)
{
    /* TODO: Print the errors!! */

    parser_status parse_label_status;

    if (is_empty(str))
        return PARSER_EMPTY;
    if (is_too_long(str))
        return PARSER_OVERFLOW;
    if ((parse_label_status = parse_label(&str, cmd)) != PARSER_OK)
        return parse_label_status;

    return PARSER_OK;
}

char* parser_status_to_string(parser_status status)
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
            return "INVALID_PARSER_OBJECT";
    }
}