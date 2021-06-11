#include <stdlib.h>
#include <string.h>
#include "instructions_table.h"
#include "dictionary.h"

#define LENGTH_OF_ARRAY(arr) (sizeof(arr) / sizeof((arr)[0]))

static operand_type two_registers_operands_types[] = {REGISTER, REGISTER};
static operand_type three_registers_operands_types[] = {REGISTER, REGISTER, REGISTER};
static operand_type arithmetics_logics_operands_types[] = {REGISTER, CONSTANT, REGISTER};
static operand_type conditional_jumps_operands_types[] = {REGISTER, REGISTER, LABEL};
static operand_type memory_instructions_operands_types[] = {REGISTER, CONSTANT, REGISTER};
static operand_type register_or_label_operand_type[] = {LABEL_OR_REGISTER};
static operand_type label_operand_type[] = {LABEL};

static instruction instructions_arr[] = {
    {"add", R, 1, 0, 3, three_registers_operands_types},
    {"sub", R, 2, 0, 3, three_registers_operands_types},
    {"and", R, 3, 0, 3, three_registers_operands_types},
    {"or", R, 4, 0, 3, three_registers_operands_types},
    {"nor", R, 5, 0, 3, three_registers_operands_types},
    {"move", R, 1, 1, 2, two_registers_operands_types},
    {"mvhi", R, 2, 1, 2, two_registers_operands_types},
    {"mvlo", R, 3, 1, 2, two_registers_operands_types},
    {"addi", I, 0, 10, 3, arithmetics_logics_operands_types},
    {"subi", I, 0, 11, 3, arithmetics_logics_operands_types},
    {"andi", I, 0, 12, 3, arithmetics_logics_operands_types},
    {"ori", I, 0, 13, 3, arithmetics_logics_operands_types},
    {"nori", I, 0, 14, 3, arithmetics_logics_operands_types},
    {"bne", I, 0, 15, 3, conditional_jumps_operands_types},
    {"beq", I, 0, 16, 3, conditional_jumps_operands_types},
    {"blt", I, 0, 17, 3, conditional_jumps_operands_types},
    {"bgt", I, 0, 18, 3, conditional_jumps_operands_types},
    {"lb", I, 0, 19, 3, memory_instructions_operands_types},
    {"sb", I, 0, 20, 3, memory_instructions_operands_types},
    {"lw", I, 0, 21, 3, memory_instructions_operands_types},
    {"sw", I, 0, 22, 3, memory_instructions_operands_types},
    {"lh", I, 0, 23, 3, memory_instructions_operands_types},
    {"sh", I, 0, 24, 3, memory_instructions_operands_types},
    {"jmp", J, 0, 30, 1, register_or_label_operand_type},
    {"la", J, 0, 31, 1, label_operand_type},
    {"call", J, 0, 32, 1, label_operand_type},
    {"stop", J, 0, 63, 0, NULL}};

instructions_table_status instructions_table_get_instruction(char *name, instruction **inst)
{
    int i;

    for (i = 0; i < LENGTH_OF_ARRAY(instructions_arr); i++)
    {
        if (strcmp(instructions_arr[i].name, name) == 0)
        {
            *inst = &instructions_arr[i];
            return IT_OK;
        }
    }

    return IT_INSTRUCTION_NOT_FOUND;
}
