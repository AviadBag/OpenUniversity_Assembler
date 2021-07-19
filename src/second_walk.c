#include "second_walk.h"
#include "linked_list.h"
#include "walk.h"
#include "command.h"
#include "logger.h"
#include "symbol.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SECOND_WALK "SecondWalk"
#define PROBLEM_WITH_CODE "ProblemWithCode"

#define BUFFER_MIN_SIZE 2048

#define REALLOC(buffer, max_size) \
{ \
    (buffer) = realloc((buffer), BUFFER_MIN_SIZE);  \
    if (!buffer) \
        return WALK_NOT_ENOUGH_MEMORY; \
    (max_size) += BUFFER_MIN_SIZE; \
}

static int code_image_max_size;
static int data_image_max_size;

/**
 * @brief Finds a symbol in the symbols table, according to it's name.
 * 
 * @param name     The name of the symbol.
 * @param st       The symbols table to search in.
 * @return symbol* A pointer to the found symbol; NULL if not found.
 */
static symbol* find_symbol(char* name, symbols_table st)
{
    int i;
    for (i = 0; i < linked_list_length(st); i++)
    {
        symbol* symbol_p = linked_list_get(st, i);
        if (strcmp(name, symbol_p->name) == 0)
            return symbol_p;
    }

    return NULL;
}

/**
 * @brief Handles the given "entry" directive.
 * 
 * @param cmd             The "entry" directive. MUST BE VALIDATED.
 * @param symbols_table_p A pointer to the symbols table.
 * @param line            On what line is this label?
 * @return walk_status WALK_PROBLEM_WITH_CODE or WALK_NOT_ENOUGH_MEMORY or WALK_OK
 */
static walk_status handle_entry_directive(command cmd, symbols_table *symbols_table_p, int line)
{
    symbol* symbol_p = find_symbol(cmd.operands[0], *symbols_table_p);
    if (!symbol_p)
    {
        logger_log(SECOND_WALK, PROBLEM_WITH_CODE, line, "Cannot mark label \"%s\" as entry, because it does not exist");
        return WALK_PROBLEM_WITH_CODE;
    }
    symbol_p->is_entry = true;

    return WALK_OK;
}

/**
 * @brief Handles the given "define" directive. ('db' or 'dh' or 'dw').
 *  
 * @param cmd        The "define" directive. MUST BE VALIDATED.
 * @param data_image A pointer to where to put the address of the data image.
 * @param line       On what line is this label?
 * @return walk_status WALK_NOT_ENOUGH_MEMORY or WALK_OK
 */
walk_status handle_define_directive(command cmd, char** data_image, int* dcf_p)
{
    int size, i;
    switch (cmd.command_name[1]) /* 'b' for byte, 'h' for half, 'w' for word */
    {
        case 'b':
            size = BYTE;
            break;
        case 'h':
            size = HALF;
            break;
        case 'w':
            size = WORD;
            break;
        default: /* Will never happen */
            break;
    }

    while (*dcf_p + size * cmd.number_of_operands > data_image_max_size)
        REALLOC(*data_image, data_image_max_size);

    for (i = 0; i < cmd.number_of_operands; i++)
    {
        char* operand = cmd.operands[i];
        if (size == BYTE)
        {
            (*data_image)[*dcf_p] = atoi(operand);
        }

        *dcf_p += size;
    }

    return WALK_OK;
}

/**
 * @brief Handles the given directive.
 * 
 * @param cmd             The directive. MUST BE VALIDATED.
 * @param data_image      A pointer to where to put the address of the data image.
 * @param dcf_p           A pointer to the DCF.
 * @param symbols_table_p A pointer to the symbols table.
 * @param line            On what line is this label?
 * @return walk_status    WALK_PROBLEM_WITH_CODE or WALK_NOT_ENOUGH_MEMORY or WALK_OK
 */
static walk_status handle_directive(command cmd, char **data_image, int* dcf_p, symbols_table *symbols_table_p, int line)
{
    if (strcmp(cmd.command_name, "entry") == 0) return handle_entry_directive(cmd, symbols_table_p, line);
    if (*cmd.command_name == 'd') /* 'db' or 'dh' or 'dw'. */
        return handle_define_directive(cmd, data_image, dcf_p);

    return WALK_OK;
}

/**
 * @brief Handles the given instruction.
 * 
 * @param cmd             The directive. MUST BE VALIDATED.
 * @param code_image      A pointer to where to put the address of the code image.
 * @param dcf_p           A pointer to the DCF.
 * @param symbols_table_p A pointer to the symbols table.
 * @param line            On what line is this label?
 * @return walk_status    WALK_NOT_ENOUGH_MEMORY or WALK_OK
 */
walk_status handle_instruction(command cmd, char **code_image, int *icf_p)
{
    return WALK_OK;
}

walk_status second_walk(char* file_name, symbols_table *symbols_table_p, char **data_image, int *dcf_p, char **code_image, int *icf_p)
{
    FILE *file;
    command cmd;
    int line_number = 0;
    walk_status status;

    /* Initialize data image and code image */
    *data_image = malloc(BUFFER_MIN_SIZE);
    *code_image = malloc(BUFFER_MIN_SIZE);
    if (!data_image || !code_image)
        return WALK_NOT_ENOUGH_MEMORY;
    code_image_max_size = data_image_max_size = BUFFER_MIN_SIZE;

    /* Open the input file */
    file = fopen(file_name, "r");
    if (!file)
    {
        printf("Error: Cannot open file \"%s\". Skipping.\n", file_name);
        return WALK_IO_ERROR;
    }

    /* Start! */
    while ((status = get_next_command(file, &cmd, ++line_number, false)) != WALK_EOF)
    {
        if (status == WALK_NOT_ENOUGH_MEMORY)
            return status;

        if (cmd.type == DIRECTIVE)
        {
            if ((status = handle_directive(cmd, data_image, dcf_p, symbols_table_p, line_number)) != WALK_OK)
                return status;
        }
        else /* Instruction */
        {
            if ((status = handle_instruction(cmd, code_image, icf_p)) != WALK_OK)
                return status;
        }
    }

    return WALK_OK;
}