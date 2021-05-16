#ifndef _PARSER_H
#define _PARSER_H

#define LABEL_MAX_LENGTH 31

typedef enum e_command_type
{
    DIRECTIVE,  /* For example: .entry LABEL */
    INSTRUCTION /* For example: addi $9, -45, $8 */
} command_type;

typedef struct s_command
{
    command_type type;                /* Tye type of the command */
    char **operands;                  /* All the operands of the command */
    int operands_length;              /* The length of the operands array */
    char *command_name;               /* The name of the command (For example - "addi") */
    char label[LABEL_MAX_LENGTH + 1]; /* The label of that line; Empty string if there is no label. */
} command;

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
 * @param str The string to parse.
 * @param cmd A pointer to a command struct to store inside the data.
 * @return PARSER_SYNTAX_ERROR or PARSER_EMPTY or PARSER_OVERFLOW or PARSER_OK.
 */
parser_status parser_parse(char *str, command *cmd);

/**
 * Converts a parser_status object to it's string representation. 
 * @param  status The status to convert
 * @return The string representation
 */
char* parser_status_to_string(parser_status status);

#endif