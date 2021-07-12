#include "first_walk.h"
#include "logger.h"
#include "parser.h"
#include "validator.h"
#include "symbol.h"
#include "boolean.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_MAX_LENGTH 80

#define FIRST_WALK "FirstWalk"
#define PROBLEM_WITH_CODE "PromblemWithCode"

#define PC_DEFAULT_VALUE 100
#define DC_DEFAULT_VALUE 0

#define INSTRUCTION_SIZE 4 /* = 32 bits */

#define BYTE 1
#define HALF 2
#define WORD 4

/**
 * @brief Updates pc and dc according to the given command.
 * 
 * @param pc  The program counter
 * @param dc  The data counetr
 * @param cmd The next command. MUST BE VALIDATED.
 */
void next_counter(unsigned long *pc, unsigned long *dc, command cmd)
{
    if (cmd.type == INSTRUCTION)
        *pc += INSTRUCTION_SIZE;
    else
    {
        int unit_size; /* In bytes */
        int n;         /* How many data? */

        if (strcmp(cmd.command_name, "entry") == 0 || strcmp(cmd.command_name, "extern") == 0)
            return; /* There is nothing to do */

        if (strcmp(cmd.command_name, "db") == 0 || strcmp(cmd.command_name, "asciz") == 0)
            unit_size = BYTE;
        else if (strcmp(cmd.command_name, "dh") == 0)
            unit_size = HALF;
        else if (strcmp(cmd.command_name, "dw") == 0)
            unit_size = WORD;

        if (strcmp(cmd.command_name, "asciz") == 0)
            n = strlen(cmd.operands[0]) - 2 + 1; /* -2 - so it will not count the quotes. +1 - for the null terminator. */
        else
            n = cmd.number_of_operands;

        *dc += n * unit_size;
    }
}

/**
 * @brief This method reads the next line from file. Every line must be at most LINE_MAX_LENGTH chars.
 *        If the file is over, the buffer will be empty.
 * @param f   The file to read from.
 * @param buf The buffer to write into. Must be size of at least LINE_MAX_LENGTH + 1.
 * @return FIRST_WALK_PROBLEM_WITH_CODE or FIRST_WALK_OK
 */
first_walk_status read_next_line(FILE *f, char *buf)
{
    int c;
    int counter = 0;
    while ((c = fgetc(f)) != EOF)
    {
        counter++;
        if (counter > LINE_MAX_LENGTH)
        {
            /* Read until the end of the line, and then return the error */
            do
            {
                c = fgetc(f);
            } while (c != EOF && c != '\n');

            return FIRST_WALK_PROBLEM_WITH_CODE;
        }

        if (c == '\n')
            break;

        *buf++ = c;
    }

    *buf = '\0';
    return FIRST_WALK_OK;
}

/**
 * @brief Checks if the label declared by ".extern" definition in the given table should be put in the given symbols table.
 *        That symbol should not be put if:
 *        1. This command contains no ".extern" definition...
 * @param cmd The command. MUST BE VALIDATED.
 * @return boolean 
 */
boolean should_put_extern_symbol(command cmd)
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
boolean should_put_label_symbol(command cmd)
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
boolean symbols_table_symbol_exist(char *symbol_name, symbols_table st)
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
 * @return FIRST_WALK_NOT_ENOUGH_MEMORY or FIRST_WALK_OK.
 */
first_walk_status put_extern_symbol(command cmd, symbols_table *symbols_table_p)
{
    symbol *symbol_t;

    if (!should_put_extern_symbol(cmd))
        return FIRST_WALK_OK; /* Just skip it */

    symbol_t = malloc(sizeof(symbol));
    if (!symbol_t)
        return FIRST_WALK_NOT_ENOUGH_MEMORY;

    symbol_t->is_entry = false;
    symbol_t->type = EXTERNAL;
    symbol_t->value = 0; /* Will be filled by the linker */
    strcpy(symbol_t->name, cmd.operands[0]);

    if (symbols_table_symbol_exist(symbol_t->name, *symbols_table_p))
        return FIRST_WALK_OK; /* I just skip */

    if (linked_list_append(symbols_table_p, symbol_t) == LINKED_LIST_NOT_ENOUGH_MEMORY)
        return FIRST_WALK_NOT_ENOUGH_MEMORY;

    return FIRST_WALK_OK;
}

/**
 * @brief Puts the symbol of the given command's label (if exists) in the symbols table.
 * 
 * @param cmd  The command. MUST BE VALIDATED.
 * @param st   The symbols table to insert into.
 * @param pc  Current program counter
 * @param dc  Current data counter
 * @param line On what line is this command is?
 * @return FIRST_WALK_NOT_ENOUGH_MEMORY or FIRST_WALK_PROBLEM_WITH_CODE or FIRST_WALK_OK.
 */
first_walk_status put_label_symbol(command cmd, symbols_table *symbols_table_p, unsigned long pc, unsigned long dc, int line)
{
    symbol *symbol_t;

    if (!should_put_label_symbol(cmd))
        return FIRST_WALK_OK; /* Just skip it */

    symbol_t = malloc(sizeof(symbol));
    if (!symbol_t)
        return FIRST_WALK_NOT_ENOUGH_MEMORY;

    symbol_t->is_entry = false; /* Will be filled during the second walk */
    symbol_t->type = (cmd.type == INSTRUCTION) ? CODE : DATA;
    symbol_t->value = (cmd.type == INSTRUCTION) ? pc : dc;
    strcpy(symbol_t->name, cmd.label);

    if (symbols_table_symbol_exist(symbol_t->name, *symbols_table_p))
    {
        logger_log(FIRST_WALK, PROBLEM_WITH_CODE, line, "Label %s was already defined", symbol_t->name);
        return FIRST_WALK_PROBLEM_WITH_CODE;
    }

    if (linked_list_append(symbols_table_p, symbol_t) == LINKED_LIST_NOT_ENOUGH_MEMORY)
        return FIRST_WALK_NOT_ENOUGH_MEMORY;

    return FIRST_WALK_OK;
}

/**
 * @brief Puts the symbol of the given command (If exist) in the symbols table.
 * 
 * @param cmd The command. MUST BE VALIDATED.
 * @param st  The symbols table to insert into.
 * @param pc  Current program counter
 * @param dc  Current data counter
 * @param line On what line is this command is?
 * @return FIRST_WALK_NOT_ENOUGH_MEMORY or FIRST_WALK_OK.
 */
first_walk_status put_symbol(command cmd, symbols_table *symbols_table_p, unsigned long pc, unsigned long dc, int line)
{
    first_walk_status status;

    if ((status = put_label_symbol(cmd, symbols_table_p, pc, dc, line)) != FIRST_WALK_OK)
        return status;
    if ((status = put_extern_symbol(cmd, symbols_table_p)) != FIRST_WALK_OK)
        return status;

    return FIRST_WALK_OK;
}

/**
 * @brief Fills the symbols table with the symbols
 * 
 * @param f  The file to read from.
 * @param st The symbols table to write into.
 * @return first_walk_status - FIRST_WALK_NOT_ENOUGH_MEMORY or FIRST_WALK_PROBLEM_WITH_CODE or FIRST_WALK_OK.
 */
first_walk_status fill_symbols_table(FILE *f, symbols_table *symbols_table_p)
{
    int line_number;
    unsigned long pc, dc;
    char line[LINE_MAX_LENGTH + 1]; /* +1 for the last '\0'. */
    first_walk_status status = FIRST_WALK_OK;
    parser_status p_status;
    validator_status v_status;
    command cmd;

    line_number = 0;
    pc = 100, dc = 0;

    while (1)
    {
        line_number++;
        if (read_next_line(f, line) == FIRST_WALK_PROBLEM_WITH_CODE)
        {
            status = FIRST_WALK_PROBLEM_WITH_CODE;
            logger_log(FIRST_WALK, PROBLEM_WITH_CODE, line_number, "A line must be at most %d chars, including whitespaces", LINE_MAX_LENGTH);
            continue;
        }

        if (!*line) /* EOF */
            break;

        p_status = parser_parse(line, &cmd, line_number);
        switch (p_status)
        {
        case PARSER_OVERFLOW:
        case PARSER_SYNTAX_ERROR:
            status = FIRST_WALK_PROBLEM_WITH_CODE;
            continue;

        case PARSER_EMPTY:
            continue;

        default:
            break;
        }

        v_status = validator_validate(cmd, line_number);
        if (v_status == VALIDATOR_INVALID)
        {
            status = FIRST_WALK_PROBLEM_WITH_CODE;
            continue;
        }

        put_symbol(cmd, symbols_table_p, pc, dc, line_number);
        printf("Command: %s, dc: %lu, pc: %lu\n", cmd.command_name, dc, pc);

        next_counter(&pc, &dc, cmd);
    }

    return status;
}

first_walk_status first_walk(char *file_name, symbols_table *symbols_table_p)
{
    FILE *file;

    file = fopen(file_name, "r");
    if (!file)
    {
        printf("Error: Cannot open file \"%s\". Skipping.\n", file_name);
        return FIRST_WALK_IO_ERROR;
    }

    *symbols_table_p = linked_list_create();

    return fill_symbols_table(file, symbols_table_p);
}
