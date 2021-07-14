#include "first_walk.h"
#include "logger.h"
#include "parser.h"
#include "validator.h"
#include "symbol.h"
#include "boolean.h"
#include "walk.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FIRST_WALK "FirstWalk"
#define PROBLEM_WITH_CODE "PromblemWithCode"

/**
 * @brief Checks if the label declared by ".extern" definition in the given table should be put in the given symbols table.
 *        That symbol should not be put if:
 *        1. This command contains no ".extern" definition...
 * @param cmd The command. MUST BE VALIDATED.
 * @return boolean 
 */
static boolean should_put_extern_symbol(command cmd)
{
    return strcmp(cmd.command_name, "extern") == 0;
}

/**
 * @brief Checks if the given command's label should be put in the symbols table.
 *        A command's label should not be put if:
 *        1. There is no label...
 *        2. The command is .entry or .extern
 * @param cmd      The command to check.
 * @return boolean True or False.
 */
static boolean should_put_label_symbol(command cmd)
{
    /* I could have made this function much shorter, but I prefer it like that */

    if (!command_has_label(cmd))
        return false;

    if (strcmp(cmd.command_name, "extern") == 0)
        return false;

    if (strcmp(cmd.command_name, "entry") == 0)
        return false;

    return true;
}

/**
 * @brief Checks if the given symbol exist in the given symbols table
 * 
 * @param symbol      The symbol
 * @param symbol_name The symbols table
 * @return boolean    True or False
 */
static boolean symbols_table_symbol_exist(char *symbol_name, symbols_table st)
{
    int i;
    symbol *symbol_t;
    for (i = 0; i < linked_list_length(st); i++)
    {
        symbol_t = (symbol *)linked_list_get(st, i);
        if (strcmp(symbol_t->name, symbol_name) == 0)
            return true;
    }

    return false;
}

/**
 * @brief If the given command is an ".extern" defintion, put the symbol declared extern in the given symbols table.
 * 
 * @param cmd The command. MUST BE VALIDATED.
 * @param st  The symbols table to insert into.
 * @param line On what line is this command is?
 * @return WALK_NOT_ENOUGH_MEMORY or WALK_PROBLEM_WITH_CODE or WALK_OK.
 */
static walk_status put_extern_symbol(command cmd, symbols_table *symbols_table_p, int line)
{
    symbol *symbol_t;
    int i;

    if (!should_put_extern_symbol(cmd))
        return WALK_OK; /* Just skip it */

    symbol_t = malloc(sizeof(symbol));
    if (!symbol_t)
        return WALK_NOT_ENOUGH_MEMORY;

    symbol_t->is_entry = false;
    symbol_t->type = EXTERNAL;
    symbol_t->value = 0; /* Will be filled by the linker */
    strcpy(symbol_t->name, cmd.operands[0]);

    for (i = 0; i < linked_list_length(*symbols_table_p); i++)
    {
        symbol* s = (symbol*) linked_list_get(*symbols_table_p, i);
        if (strcmp(s->name, symbol_t->name) == 0) /* This symbol already exist */
        {
            if (s->type == EXTERNAL)
                return WALK_OK;
            else
            {
                logger_log(FIRST_WALK, PROBLEM_WITH_CODE, line, "Label \"%s\" was already defined", symbol_t->name);
                return WALK_PROBLEM_WITH_CODE; /* You cannot define it extern if it has already been defined.. */
            }
        }
    }

    if (linked_list_append(symbols_table_p, symbol_t) == LINKED_LIST_NOT_ENOUGH_MEMORY)
        return WALK_NOT_ENOUGH_MEMORY;

    return WALK_OK;
}

/**
 * @brief Puts the symbol of the given command's label (if exists) in the symbols table.
 * 
 * @param cmd  The command. MUST BE VALIDATED.
 * @param st   The symbols table to insert into.
 * @param pc  Current program counter
 * @param dc  Current data counter
 * @param line On what line is this command is?
 * @return WALK_NOT_ENOUGH_MEMORY or WALK_PROBLEM_WITH_CODE or WALK_OK.
 */
static walk_status put_label_symbol(command cmd, symbols_table *symbols_table_p, unsigned long pc, unsigned long dc, int line)
{
    symbol *symbol_t;

    if (!should_put_label_symbol(cmd))
        return WALK_OK; /* Just skip it */

    symbol_t = malloc(sizeof(symbol));
    if (!symbol_t)
        return WALK_NOT_ENOUGH_MEMORY;

    symbol_t->is_entry = false; /* Will be filled during the second walk */
    symbol_t->type = (cmd.type == INSTRUCTION) ? CODE : DATA;
    symbol_t->value = (cmd.type == INSTRUCTION) ? pc : dc;
    strcpy(symbol_t->name, cmd.label);

    if (symbols_table_symbol_exist(symbol_t->name, *symbols_table_p))
    {
        logger_log(FIRST_WALK, PROBLEM_WITH_CODE, line, "Label \"%s\" was already defined", symbol_t->name);
        return WALK_PROBLEM_WITH_CODE;
    }

    if (linked_list_append(symbols_table_p, symbol_t) == LINKED_LIST_NOT_ENOUGH_MEMORY)
        return WALK_NOT_ENOUGH_MEMORY;

    return WALK_OK;
}

/**
 * @brief Puts the symbol of the given command (If exist) in the symbols table.
 * 
 * @param cmd The command. MUST BE VALIDATED.
 * @param st  The symbols table to insert into.
 * @param pc  Current program counter
 * @param dc  Current data counter
 * @param line On what line is this command is?
 * @return WALK_NOT_ENOUGH_MEMORY or WALK_PROBLEM_WITH_CODE or WALK_OK.
 */
static walk_status put_symbol(command cmd, symbols_table *symbols_table_p, unsigned long pc, unsigned long dc, int line)
{
    walk_status status;

    if ((status = put_label_symbol(cmd, symbols_table_p, pc, dc, line)) != WALK_OK)
        return status;
    if ((status = put_extern_symbol(cmd, symbols_table_p, line)) != WALK_OK)
        return status;

    return WALK_OK;
}

/**
 * @brief Fills the symbols table with the symbols
 * 
 * @param f  The file to read from.
 * @param st The symbols table to write into.
 * @return walk_status - WALK_NOT_ENOUGH_MEMORY or WALK_PROBLEM_WITH_CODE or WALK_OK.
 */
static walk_status fill_symbols_table(FILE *f, symbols_table *symbols_table_p)
{
    int line_number, i;
    unsigned long pc, dc;
    walk_status status = WALK_OK, put_symbol_status;
    command cmd;

    line_number = 0;
    pc = PC_DEFAULT_VALUE, dc = DC_DEFAULT_VALUE;

    while (1)
    {   
        line_number++;
        status = get_next_command(f, &cmd, line_number);
        if (status == WALK_EOF)
            break;
        else if (status == WALK_PROBLEM_WITH_CODE)
            continue;
        else if (status == WALK_NOT_ENOUGH_MEMORY)
            return status;

        if ((put_symbol_status = put_symbol(cmd, symbols_table_p, pc, dc, line_number)) != WALK_OK)
            status = put_symbol_status;

        next_counter(&pc, &dc, cmd);
    }

    /* Update the data symbols' values to be AFTER the code */
    for (i = 0; i < linked_list_length(*symbols_table_p); i++)
    {
        symbol* symbol_t = (symbol*) linked_list_get(*symbols_table_p, i);
        if (symbol_t->type == DATA)
            symbol_t->value += pc;
    }

    return WALK_OK;
}

walk_status first_walk(char *file_name, symbols_table *symbols_table_p)
{
    FILE *file;
    walk_status status;

    file = fopen(file_name, "r");
    if (!file)
    {
        printf("Error: Cannot open file \"%s\". Skipping.\n", file_name);
        return WALK_IO_ERROR;
    }

    *symbols_table_p = linked_list_create();
    status = fill_symbols_table(file, symbols_table_p);

    fclose(file);
    return status;
}
