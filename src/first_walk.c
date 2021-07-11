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

/**
 * @brief This method reads the next line from file. Every line must be at most LINE_MAX_LENGTH chars.
 *        If the file is over, the buffer will be empty.
 * @param f   The file to read from.
 * @param buf The buffer to write into. Must be size of at least LINE_MAX_LENGTH + 1.
 * @return FIRST_WALK_PROBLEM_WITH_CODE or FIRST_WALK_OK
 */
first_walk_status read_next_line(FILE* f, char* buf)
{
    int c;
    int counter = 0;
    while ((c = fgetc(f)) != EOF)
    {
        counter++;
        if (counter > LINE_MAX_LENGTH)
            return FIRST_WALK_PROBLEM_WITH_CODE;

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
    return strcmp(cmd.command_name, ".extern") == 0;
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

    if (strcmp(cmd.command_name, ".extern") == 0)
        return false;
    
    if (strcmp(cmd.command_name, ".entry") == 0)
        return false;

    return true;
}

/**
 * @brief If the given command is an ".extern" defintion, put the symbol declared extern in the given symbols table.
 * 
 * @param cmd The command. MUST BE VALIDATED.
 * @param st  The symbols table to insert into.
 * @return FIRST_WALK_NOT_ENOUGH_MEMORY or FIRST_WALK_OK.
 */
first_walk_status put_extern_symbol(command cmd, symbols_table st)
{
    symbol* symbol_t;

    if (!should_put_extern_symbol(cmd))
        return FIRST_WALK_OK; /* Just skip it */

    symbol_t = malloc(sizeof(symbol));
    if (!symbol_t)
        return FIRST_WALK_NOT_ENOUGH_MEMORY;

    symbol_t->is_entry = false;
    symbol_t->name = cmd.operands[0];
    symbol_t->type = EXTERNAL;

    return FIRST_WALK_OK;
}

/**
 * @brief Puts the symbol of the given command's label (if exists) in the symbols table.
 * 
 * @param cmd The command. MUST BE VALIDATED.
 * @param st  The symbols table to insert into.
 * @return FIRST_WALK_NOT_ENOUGH_MEMORY or FIRST_WALK_OK.
 */
first_walk_status put_label_symbol(command cmd, symbols_table st)
{
    symbol* symbol_t;

    if (!should_put_label_symbol(cmd))
        return FIRST_WALK_OK; /* Just skip it */

    symbol_t = malloc(sizeof(symbol));
    if (!symbol_t)
        return FIRST_WALK_NOT_ENOUGH_MEMORY;

    symbol_t->is_entry = false; /* Will be filled during the second walk */
    symbol_t->name = cmd.label;
    symbol_t->type = (cmd.type == INSTRUCTION) ? CODE : DATA;

    return FIRST_WALK_OK;
}

/**
 * @brief Puts the symbol of the given command (If exist) in the symbols table.
 * 
 * @param cmd The command. MUST BE VALIDATED.
 * @param st  The symbols table to insert into.
 * @return FIRST_WALK_NOT_ENOUGH_MEMORY or FIRST_WALK_OK.
 */
first_walk_status put_symbol(command cmd, symbols_table st)
{
    first_walk_status status;

    if ((status = put_label_symbol(cmd, st)) != FIRST_WALK_OK)
        return status;
    if ((status = put_extern_symbol(cmd, st)) != FIRST_WALK_OK)
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
first_walk_status fill_symbols_table(FILE* f, symbols_table st)
{
    int line_number;
    char line[LINE_MAX_LENGTH+1]; /* +1 for the last '\0'. */
    first_walk_status status = FIRST_WALK_OK;
    parser_status p_status;
    validator_status v_status;
    command cmd;

    line_number = 0;
    while (1)
    {
        line_number++;
        if (read_next_line(f, line) == FIRST_WALK_PROBLEM_WITH_CODE)
        {
            status = FIRST_WALK_PROBLEM_WITH_CODE;
            logger_log("FirstWalk", "CodeError", line_number, "A line must be at most %d chars, including whitespaces", LINE_MAX_LENGTH);
            continue;
        }

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
    }

    return status;
}

first_walk_status first_walk(char* file_name, symbols_table* symbols_table_p)
{
    FILE* file;
    symbols_table st;

    file = fopen(file_name, "r");
    if (!file)
    {
        printf("Error: Cannot open file \"%s\". Skipping.\n", file_name);
        return FIRST_WALK_IO_ERROR;
    }

    st = dictionary_create();
    *symbols_table_p = st; /* It is ok; st is dynamically allocated. */

    return fill_symbols_table(file, st);
}
