#include "second_walk.h"
#include "linked_list.h"
#include "walk.h"
#include "command.h"
#include "logger.h"
#include "symbol.h"

#include <stdio.h>
#include <string.h>

#define SECOND_WALK "SecondWalk"
#define PROBLEM_WITH_CODE "ProblemWithCode"

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
 * @brief Handles the given entry directive.
 * 
 * @param cmd             The entry directive.
 * @param symbols_table_p A pointer to the symbols table.
 * @param line            On what line is this label?
 * @return walk_status WALK_PROBLEM_WITH_CODE or WALK_OK
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
 * @brief Handles the given directive.
 * 
 * @param cmd             The directive.
 * @param data_image_p    A pointer to the data image.
 * @param dcf_p           A pointer to the DCF.
 * @param symbols_table_p A pointer to the symbols table.
 * @param line            On what line is this label?
 * @return walk_status    WALK_PROBLEM_WITH_CODE or WALK_NOT_ENOUGH_MEMORY or WALK_OK
 */
static walk_status handle_directive(command cmd, linked_list* data_image_p, int* dcf_p, symbols_table *symbols_table_p, int line)
{
    if (strcmp(cmd.command_name, "entry") == 0) return handle_entry_directive(cmd, symbols_table_p, line);

    return WALK_OK;
}

/**
 * @brief Handles the given instruction.
 * 
 * @param cmd             The directive.
 * @param data_image_p    A pointer to the data image.
 * @param dcf_p           A pointer to the DCF.
 * @param symbols_table_p A pointer to the symbols table.
 * @param line            On what line is this label?
 * @return walk_status    WALK_NOT_ENOUGH_MEMORY or WALK_OK
 */
walk_status handle_instruction(command cmd, linked_list *code_image_p, int *icf_p)
{
    return WALK_OK;
}

walk_status second_walk(char* file_name, symbols_table *symbols_table_p, linked_list *data_image_p, int *dcf_p, linked_list *code_image_p, int *icf_p)
{
    FILE *file;
    command cmd;
    int line_number = 0;
    walk_status status;

    /* Initialize data image and code image */
    *data_image_p = linked_list_create();
    *code_image_p = linked_list_create();

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
            if ((status = handle_directive(cmd, data_image_p, dcf_p, symbols_table_p, line_number)) != WALK_OK)
                return status;
        }
        else /* Instruction */
        {
            if ((status = handle_instruction(cmd, code_image_p, icf_p)) != WALK_OK)
                return status;
        }
    }

    return WALK_OK;
}