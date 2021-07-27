#include "walk.h"
#include "parser.h"
#include "validator.h"
#include "logger.h"
#include "linked_list.h"
#include "symbol.h"

#include <stdio.h>
#include <string.h>

#define WALK "Walk"
#define PROBLEM_WITH_CODE "ProblemWithCode"

/**
 * @brief This method reads the next line from file. Every line must be at most LINE_MAX_LENGTH chars.
 *        If the file is over, the buffer will be empty.
 * @param f   The file to read from.
 * @param buf The buffer to write into. Must be size of at least LINE_MAX_LENGTH + 1.
 * @return WALK_PROBLEM_WITH_CODE or WALK_EOF or WALK_OK
 */
walk_status read_next_line(FILE *f, char *buf)
{
    int c;
    int counter = 0;
    while ((c = fgetc(f)) != EOF)
    {
        counter++;
        if (c == '\n')
            break;

        if (counter > LINE_MAX_LENGTH)
        {
            /* Read until the end of the line, and then return the error */
            do
            {
                c = fgetc(f);
            } while (c != EOF && c != '\n');

            return WALK_PROBLEM_WITH_CODE;
        }

        *buf++ = c;
    }
    *buf = '\0';
    
    if (counter == 0)
        return WALK_EOF;

    return WALK_OK;
}

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

walk_status get_next_command(FILE *f, command *cmd, int* line_number, boolean validate)
{
    char line[LINE_MAX_LENGTH + 1]; /* +1 for the last '\0'. */
    parser_status p_status;
    validator_status v_status;
    walk_status status;

read_line:
    (*line_number)++;
    status = read_next_line(f, line);
    if (status == WALK_PROBLEM_WITH_CODE)
    {
        logger_log(WALK, PROBLEM_WITH_CODE, *line_number, "A line must be at most %d chars, including whitespaces", LINE_MAX_LENGTH);
        return WALK_PROBLEM_WITH_CODE;
    }
    else if (status == WALK_EOF)
        return status;

    p_status = parser_parse(line, cmd, *line_number);
    switch (p_status)
    {
    case PARSER_OVERFLOW:
    case PARSER_SYNTAX_ERROR:
        free_command(*cmd);
        return WALK_PROBLEM_WITH_CODE;
    case PARSER_NOT_ENOUGH_MEMORY:
        return WALK_NOT_ENOUGH_MEMORY;

    case PARSER_EMPTY:
        free_command(*cmd);
        goto read_line; /* Try the next line */

    default:
        break;
    }

    if (validate)
    {
        v_status = validator_validate(*cmd, *line_number);
        if (v_status == VALIDATOR_INVALID)
        {
            free_command(*cmd);
            return WALK_PROBLEM_WITH_CODE;
        }
    }

    return WALK_OK;
}

void free_symbols_table(symbols_table st) 
{
    int i;
    for (i = 0; i < linked_list_length(st); i++)
    {
        symbol* symbol_t = linked_list_get(st, i);
        linked_list_free_elements(symbol_t->instructions_using_me);
        linked_list_free(symbol_t->instructions_using_me);
    }

    linked_list_free_elements(st);
    linked_list_free(st);
}
