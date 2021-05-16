#include "parser.h"
#include "boolean.h"
#include <stdlib.h>
#include <ctype.h>

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
 * Parses the label staff in the command.
 * @param str     A pointer to the pointer of the string to parse. MUST NOT BE EMPTY OR COMMENT
 * @param command The command object to parse into.
 * @return PARSER_SYNTAX_ERROR or PARSER_OVERFLOW or PARSER_OK.
 */
static parser_status parse_label(char **str, command *cmd)
{
    int label_length;

    if (**str == ':')
        /* A label cannot be empty */
        return PARSER_SYNTAX_ERROR;
    if (has_label(*str))
    {
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

parser_status parser_parse(char *str, command *cmd)
{
    parser_status parse_label_status;

    /* TODO: Check if empty */
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
        case PARSER_EMPTY_LINE:
            return "PARSER_EMPTY_LINE";
        case PARSER_OVERFLOW:
            return "PARSER_OVERFLOW";
        case PARSER_OK:
            return "PARSER_OK";
        default:
            return "INVALID_PARSER_OBJECT";
    }
}