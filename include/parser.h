#ifndef _PARSER_H
#define _PARSER_H

typedef enum e_command_type
{
    DIRECTIVE,  /* For example: .entry LABEL */
    INSTRUCTION /* For example: addi $9, -45, $8 */
} command_type;

typedef struct s_command
{
    command_type type;   /* Tye type of the command */
    char **operands;     /* All the operands of the command */
    int operands_length; /* The length of the operands array */
    char *command_name;  /* The name of the command (For example - "addi") */
    char *label;         /* The label of that line; NULL if there is no label. */
} command;

#endif