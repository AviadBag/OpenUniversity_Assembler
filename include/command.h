#ifndef _COMMAND_H
#define _COMMAND_H

/**
 * This module used for representing a parsed command.
 */

#include "boolean.h"

#define LABEL_MAX_LENGTH 31

typedef enum e_command_type
{
    DIRECTIVE,  /* For example: .entry LABEL */
    INSTRUCTION /* For example: addi $9, -45, $8 */
} command_type;


typedef struct s_command
{
    command_type type;                /**< Tye type of the command */
    char **operands;                  /**< All the operands of the command. NULL if there are no operands. */
    int number_of_operands;           /**< The length of the operands array */
    char *command_name;               /**< The name of the command (For example - "addi") */
    char label[LABEL_MAX_LENGTH + 1]; /**< The label of that line; Empty string if there is no label. */

    /**< The parser might exit early, with errors. These variables indicate what memory was allready allocated and needs
     *   to be freed, and what should not be freed - because it was never allocated */
    boolean operands_array_allocated, command_name_allocated;
    int number_of_operands_allocated;
} command;

/**
 * Converts a command_type object to it's string representation. 
 * @param  type The type to convert
 * @return The string representation
 */
char* command_type_to_string(command_type type);

/**
 * Checks if the given command has a label.
 * @param cmd The command to check.
 * @return true if there is a label. Else - false.
 */
boolean command_has_label(command cmd);

/**
 * @brief Frees the command's allocated memory. (Some of the command's fields are dynamically allocated..)
 * 
 * @param cmd The command to free.
 */
void free_command(command cmd);

#endif