#ifndef _PARSER_H
#define _PARSER_H

/**
 * This module is a parser - parses a given string into a command object.
 */

#include "command.h"

typedef enum e_parser_status
{
    PARSER_SYNTAX_ERROR,
    PARSER_EMPTY,
    PARSER_NOT_ENOUGH_MEMORY,
    PARSER_OVERFLOW, /* When the label/line is too long */
    PARSER_OK
} parser_status;

/**
 * Parses the given string.
 * @param str  The string to parse.
 * @param cmd  A pointer to a command struct to store inside the data. After usage, should be freed using command.h:free_command()
 * @param line On what line is this string?
 * @return PARSER_SYNTAX_ERROR or PARSER_EMPTY or PARSER_OVERFLOW or PARSER_NOT_ENOUGH_MEMORY or PARSER_OK.
 */
parser_status parser_parse(char *str, command *cmd, int line);

#endif
