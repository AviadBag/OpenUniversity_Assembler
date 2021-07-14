#include "walk.h"
#include "parser.h"
#include "validator.h"
#include "logger.h"

#include <stdio.h>
#include <string.h>

#define INSTRUCTION_SIZE 4 /* = 32 bits */

#define BYTE 1
#define HALF 2
#define WORD 4

#define WALK "Walk"
#define PROBLEM_WITH_CODE "PromblemWithCode"

/**
 * @brief This method reads the next line from file. Every line must be at most LINE_MAX_LENGTH chars.
 *        If the file is over, the buffer will be empty.
 * @param f   The file to read from.
 * @param buf The buffer to write into. Must be size of at least LINE_MAX_LENGTH + 1.
 * @return WALK_PROBLEM_WITH_CODE or WALK_OK
 */
walk_status read_next_line(FILE *f, char *buf)
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

            return WALK_PROBLEM_WITH_CODE;
        }

        if (c == '\n')
            break;

        *buf++ = c;
    }

    *buf = '\0';
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

walk_status get_next_command(FILE *f, command *cmd, int line_number)
{
    char line[LINE_MAX_LENGTH + 1]; /* +1 for the last '\0'. */
    parser_status p_status;
    validator_status v_status;

read_line:
    if (read_next_line(f, line) == WALK_PROBLEM_WITH_CODE)
    {
        logger_log(WALK, PROBLEM_WITH_CODE, line_number, "A line must be at most %d chars, including whitespaces", LINE_MAX_LENGTH);
        return WALK_PROBLEM_WITH_CODE;
    }

    if (!*line) /* EOF */
        return WALK_EOF;

    p_status = parser_parse(line, cmd, line_number);
    switch (p_status)
    {
    case PARSER_OVERFLOW:
    case PARSER_SYNTAX_ERROR:
        return WALK_PROBLEM_WITH_CODE;

    case PARSER_EMPTY:
        goto read_line; /* Try the next line */

    default:
        break;
    }

    v_status = validator_validate(*cmd, line_number);
    if (v_status == VALIDATOR_INVALID)
        return WALK_PROBLEM_WITH_CODE;

    return WALK_OK;
}