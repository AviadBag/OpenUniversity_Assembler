#include "boolean.h"
#include "second_walk.h"
#include "linked_list.h"
#include "walk.h"
#include "command.h"
#include "logger.h"
#include "symbol.h"
#include "translator.h"
#include "instructions_table.h"
#include "utils.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SECOND_WALK "SecondWalk"
#define PROBLEM_WITH_CODE "ProblemWithCode"

#define BUFFER_MIN_SIZE 2048

#define J_INSTRUCTIONS_LABEL_OPERAND_INDEX 0

/* Extends the buffer and it's max_size var, returns WALK_NOT_ENOUGH_MEMORY if it happens. */
#define REALLOC(buffer, current_size, max_size)                       \
    {                                                                 \
        (buffer) = realloc((buffer), current_size + BUFFER_MIN_SIZE); \
        if (!buffer)                                                  \
            return WALK_NOT_ENOUGH_MEMORY;                            \
        (max_size) += BUFFER_MIN_SIZE;                                \
    }

static int code_image_max_size;
static int data_image_max_size;

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
    symbol *symbol_p = find_symbol(cmd.operands[0], *symbols_table_p);
    if (!symbol_p)
    {
        logger_log(SECOND_WALK, PROBLEM_WITH_CODE, line, "Cannot mark label \"%s\" as entry, because it does not exist", cmd.operands[0]);
        return WALK_PROBLEM_WITH_CODE;
    }
    if (symbol_p->type == EXTERNAL)
    {
        logger_log(SECOND_WALK, PROBLEM_WITH_CODE, line, "Cannot mark label \"%s\" as entry, because it defined external", cmd.operands[0]);
        return WALK_PROBLEM_WITH_CODE;
    }
    symbol_p->is_entry = true;

    return WALK_OK;
}

/**
 * @brief Handles the given "define" directive. ('db' or 'dh' or 'dw').
 *  
 * @param cmd          The "define" directive. MUST BE VALIDATED.
 * @param data_image   A pointer to where to put the address of the data image, already containing a data image.
 * @param dc_p         A pointer to the DCF.
 * @return walk_status WALK_NOT_ENOUGH_MEMORY or WALK_OK
 */
walk_status handle_define_directive(command cmd, unsigned char **data_image, unsigned long *dc_p)
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

    /* Make sure that the buffer is big enough */
    while (*dc_p + size * cmd.number_of_operands > data_image_max_size)
        REALLOC(*data_image, *dc_p, data_image_max_size)

    /* Do each operand */
    for (i = 0; i < cmd.number_of_operands; i++)
    {
        char *operand = cmd.operands[i];
        long num = strtol(operand, 0, 10);
        put_in_char_array(*data_image, num, size, *dc_p);

        *dc_p += size;
    }

    return WALK_OK;
}

/**
 * @brief Handles the given "asciz" directive.
 *  
 * @param cmd          The "asciz" directive. MUST BE VALIDATED.
 * @param data_image   A pointer to where to put the address of the data image, already containing a data image.
 * @param dc_p         A pointer to the DCF.
 * @return walk_status WALK_NOT_ENOUGH_MEMORY or WALK_OK
 */
walk_status handle_asciz_directive(command cmd, unsigned char **data_image, unsigned long *dc_p)
{
    size_t count = strlen(cmd.operands[0]) - 2; /* Dont include the quotes */
    int i;

    /* Is the buffer big enough? */
    while (*dc_p + count > data_image_max_size)
        REALLOC(*data_image, *dc_p, data_image_max_size)

    for (i = 0; i < count; i++)
        (*data_image)[(*dc_p)++] = (unsigned char)cmd.operands[0][i + 1]; /* +1 Because [0] contains the first quote. */

    (*data_image)[(*dc_p)++] = '\0';

    return WALK_OK;
}

/**
 * @brief Handles the given directive.
 * 
 * @param cmd             The directive. MUST BE VALIDATED.
 * @param data_image      A pointer to where to put the address of the data image.
 * @param dc_p            A pointer to the DC.
 * @param symbols_table_p A pointer to the symbols table.
 * @param line            On what line is this label?
 * @return walk_status    WALK_PROBLEM_WITH_CODE or WALK_NOT_ENOUGH_MEMORY or WALK_OK
 */
static walk_status handle_directive(command cmd, unsigned char **data_image, unsigned long *dc_p, symbols_table *symbols_table_p, int line)
{
    if (strcmp(cmd.command_name, "entry") == 0)
        return handle_entry_directive(cmd, symbols_table_p, line);
    else if (*cmd.command_name == 'd') /* 'db' or 'dh' or 'dw'. */
        return handle_define_directive(cmd, data_image, dc_p);
    else if (strcmp(cmd.command_name, "asciz") == 0)
        return handle_asciz_directive(cmd, data_image, dc_p);
    else if (strcmp(cmd.command_name, "extern") == 0)
        return WALK_OK; /* There is nothing to do; The first walk already treated this case */

    return WALK_OK;
}

/**
 * @brief Adds the given command to the externs table in the symbols table,
 *        if it uses an extern label, and if the instruction type is J.
 * 
 * @param cmd          The command. MUST BE VALIDATED, and must pass a translation.
 * @param ic           Current IC.
 * @param st           The symbol table.
 * @return walk_status WALK_NOT_ENOUGH_MEMORY or WALK_PROBLEM_WITH_CODE or WALK_OK.
 */
walk_status add_instruction_to_externs_table(command cmd, unsigned long ic, symbols_table st)
{
    instruction *inst;
    char *label_name;
    symbol *symbol_p;
    unsigned long *value_p;

    instructions_table_get_instruction(cmd.command_name, &inst);

    /* Is it J? */
    if (inst->type != J)
        return WALK_OK;

    /* Ok, it is J. The only J instruction that does not use a label is "stop". */
    if (strcmp(cmd.command_name, "stop") == 0)
        return WALK_OK;

    label_name = cmd.operands[J_INSTRUCTIONS_LABEL_OPERAND_INDEX];

    /* JMP instruction can take a register instead. Make sure that this is not a register. */
    if (label_name[0] == '$')
        return WALK_OK;

    /* Make sure that the label is extern. */
    symbol_p = find_symbol(label_name, st); /* The label surely exist, because it passed translation */
    if (symbol_p->type != EXTERNAL)
        return WALK_OK;

    /* Ok, we have to add it! */
    value_p = malloc(sizeof(unsigned long));
    *value_p = ic;
    if (linked_list_append(&symbol_p->instructions_using_me, value_p) == LINKED_LIST_NOT_ENOUGH_MEMORY)
    {
        free(value_p);
        return WALK_NOT_ENOUGH_MEMORY;
    }

    return WALK_OK;
}

/**
 * @brief Handles the given instruction.
 * 
 * @param cmd          The directive. MUST BE VALIDATED.
 * @param st           The symbols table.
 * @param code_image   A pointer to where to put the address of the code image.
 * @param dc_p         A pointer to the DC.
 * @param line         On what line this instruction is?
 * @return walk_status WALK_NOT_ENOUGH_MEMORY or WALK_PROBLEM_WITH_CODE or WALK_OK.
 */
walk_status handle_instruction(command cmd, symbols_table st, unsigned char **code_image, unsigned long *ic_p, int line)
{
    machine_instruction m;
    walk_status status;
    translator_status t_status;
    unsigned long index; /* The index in the code image, IN BYTES */

    index = *ic_p - IC_DEFAULT_VALUE;

    t_status = translator_translate(cmd, st, *ic_p, line, &m);
    if (t_status != TRANSLATOR_OK)
        return WALK_PROBLEM_WITH_CODE; /* Do not need to log; The translator already logged. */

    /* Is the buffer big enough? */
    while (index + sizeof(machine_instruction) > code_image_max_size)
        REALLOC(*code_image, index, code_image_max_size)

    ((machine_instruction *)(*code_image))[index / sizeof(machine_instruction)] = m;
    status = add_instruction_to_externs_table(cmd, *ic_p, st);
    *ic_p += INSTRUCTION_SIZE;

    return status;
}

walk_status second_walk(char *file_name, symbols_table *symbols_table_p, unsigned char **data_image, unsigned long *dcf_p, unsigned char **code_image, unsigned long *icf_p)
{
    FILE *file;
    command cmd;
    int line_number = 0;
    walk_status status;
    walk_status final_status = WALK_OK;

    /* Initialize data image and code image */
    *data_image = malloc(BUFFER_MIN_SIZE);
    *code_image = malloc(BUFFER_MIN_SIZE);
    if (!*data_image || !*code_image)
        return WALK_NOT_ENOUGH_MEMORY;
    code_image_max_size = data_image_max_size = BUFFER_MIN_SIZE;

    *dcf_p = DC_DEFAULT_VALUE;
    *icf_p = IC_DEFAULT_VALUE;

    /* Open the input file */
    file = fopen(file_name, "r");
    if (!file)
    {
        printf("Error: Cannot open file \"%s\". Skipping.\n", file_name);
        return WALK_IO_ERROR;
    }

    /* Start! */
    while ((status = get_next_command(file, &cmd, &line_number, false)) != WALK_EOF)
    {
        if (status == WALK_NOT_ENOUGH_MEMORY)
            return status;

        if (cmd.type == DIRECTIVE)
        {
            if ((status = handle_directive(cmd, data_image, dcf_p, symbols_table_p, line_number)) != WALK_OK)
            {
                free_command(cmd);
                if (status == WALK_PROBLEM_WITH_CODE)
                    final_status = status;
                else return status;
            } else free_command(cmd);
        }
        else /* Instruction */
        {
            if ((status = handle_instruction(cmd, *symbols_table_p, code_image, icf_p, line_number)) != WALK_OK)
            {
                free_command(cmd);
                if (status == WALK_PROBLEM_WITH_CODE)
                    final_status = status;
                else return status;
            } else free_command(cmd);
        }
    }

    fclose(file);
    return final_status;
}
