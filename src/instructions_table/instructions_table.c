#include <stdlib.h>
#include "instructions_table.h"
#include "dictionary.h"

static dictionary dict;

/**
 * Allocatess an instruction struct. SHOULD BE FREED AFTER USAGE!
 * @param i                  A pointer to where to store the address of the allocated struct.
 * @param type               The type of the instruction.
 * @param funct              The funct of the instruction.
 * @param opcode             The opcode of the instruction.
 * @param number_of_operands How many operands the opcode has.
 * @param operand_type       An array of the operands types of the instruction.
 * @return NOT_ENOUGH_MEMORY if it happens, else - SUCCESS.
 */
static status allocate_instruction(instruction **i, instruction_type type, int funct, int opcode, int number_of_operands, operand_type *operands_types)
{
    *i = (instruction *)malloc(sizeof(instruction));
    if (*i == NULL)
        return NOT_ENOUGH_MEMORY;

    (*i)->type = type;
    (*i)->funct = funct;
    (*i)->opcode = opcode;
    (*i)->number_of_operands = number_of_operands;
    (*i)->operands_types = operands_types;

    return SUCCESS;
}

/**
 * Adds R instructions to the global dictionary. (Must be initialized!)
 * @return NOT_ENOUGH_MEMORY if it happens, else - SUCCESS.
 */
static status add_R_instructions()
{
    /* The operands types of instructions with two operands */
    static operand_type two_operands_types[] = {REGISTER, REGISTER};

    /* The operands types of instructions with three operands */
    static operand_type three_operands_types[] = {REGISTER, REGISTER, REGISTER};

    instruction *add, *sub, *and, *or, *nor;
    instruction *move, *mvhi, *mvlo;

    /* Fill all the instructions with data */
    if (allocate_instruction(&add,  R, 1, 0, 3, three_operands_types) == NOT_ENOUGH_MEMORY
     || allocate_instruction(&sub,  R, 2, 0, 3, three_operands_types) == NOT_ENOUGH_MEMORY
     || allocate_instruction(&and,  R, 3, 0, 3, three_operands_types) == NOT_ENOUGH_MEMORY
     || allocate_instruction(&or,   R, 4, 0, 3, three_operands_types) == NOT_ENOUGH_MEMORY
     || allocate_instruction(&nor,  R, 5, 0, 3, three_operands_types) == NOT_ENOUGH_MEMORY
     || allocate_instruction(&move, R, 1, 1, 2, two_operands_types)   == NOT_ENOUGH_MEMORY
     || allocate_instruction(&mvhi, R, 2, 1, 2, two_operands_types)   == NOT_ENOUGH_MEMORY
     || allocate_instruction(&mvlo, R, 3, 1, 2, two_operands_types)   == NOT_ENOUGH_MEMORY
    )
        return NOT_ENOUGH_MEMORY;

    /* Put the instructions in the dictionary_ */
    if (dictionary_put(&dict, "add",  (void*)add)    == NOT_ENOUGH_MEMORY
     || dictionary_put(&dict, "sub",  (void*)sub)    == NOT_ENOUGH_MEMORY
     || dictionary_put(&dict, "and",  (void*)and)    == NOT_ENOUGH_MEMORY
     || dictionary_put(&dict, "or",   (void*)or)     == NOT_ENOUGH_MEMORY
     || dictionary_put(&dict, "nor",  (void*)nor)    == NOT_ENOUGH_MEMORY
     || dictionary_put(&dict, "move", (void*)move)   == NOT_ENOUGH_MEMORY
     || dictionary_put(&dict, "mvhi", (void*)mvhi)   == NOT_ENOUGH_MEMORY
     || dictionary_put(&dict, "mvlo", (void*)mvlo)   == NOT_ENOUGH_MEMORY
    )
        return NOT_ENOUGH_MEMORY;

    return SUCCESS;
}

/**
 * Adds I instructions to the global dictionary. (Must be initialized!)
 * @return NOT_ENOUGH_MEMORY if it happens, else - SUCCESS.
 */
static status add_I_instructions()
{
    /* The operands types of arithmetics or logics */
    static operand_type arithmetics_logics_operands_types[] = {REGISTER, CONSTANT, REGISTER};

    /* The operands types of conditional jumps */
    static operand_type conditional_jumps_operands_types[] = {REGISTER, REGISTER, LABEL};

    /* The operands types of memory instructions */
    static operand_type memory_instructions_operands_types[] = {REGISTER, CONSTANT, REGISTER};

    instruction *addi, *subi, *andi, *ori, *nori;
    instruction *bne, *beq, *blt, *bgt;
    instruction *lb, *sb, *lw, *sw, *lh, *sh;

    /* Fill all the instructions with data */
    if (allocate_instruction(&addi, I, 0, 10, 3, arithmetics_logics_operands_types) == NOT_ENOUGH_MEMORY
     || allocate_instruction(&subi, I, 0, 11, 3, arithmetics_logics_operands_types) == NOT_ENOUGH_MEMORY
     || allocate_instruction(&andi, I, 0, 12, 3, arithmetics_logics_operands_types) == NOT_ENOUGH_MEMORY
     || allocate_instruction(&ori,  I, 0, 13, 3, arithmetics_logics_operands_types) == NOT_ENOUGH_MEMORY
     || allocate_instruction(&nori, I, 0, 14, 3, arithmetics_logics_operands_types) == NOT_ENOUGH_MEMORY
     || allocate_instruction(&bne,  I, 0, 15, 3, conditional_jumps_operands_types)  == NOT_ENOUGH_MEMORY
     || allocate_instruction(&beq,  I, 0, 16, 3, conditional_jumps_operands_types)  == NOT_ENOUGH_MEMORY
     || allocate_instruction(&blt,  I, 0, 17, 3, conditional_jumps_operands_types)  == NOT_ENOUGH_MEMORY
     || allocate_instruction(&bgt,  I, 0, 18, 3, conditional_jumps_operands_types)  == NOT_ENOUGH_MEMORY
     || allocate_instruction(&lb,   I, 0, 19, 3, memory_instructions_operands_types)   == NOT_ENOUGH_MEMORY
     || allocate_instruction(&sb,   I, 0, 20, 3, memory_instructions_operands_types)   == NOT_ENOUGH_MEMORY
     || allocate_instruction(&lw,   I, 0, 21, 3, memory_instructions_operands_types)   == NOT_ENOUGH_MEMORY
     || allocate_instruction(&sw,   I, 0, 22, 3, memory_instructions_operands_types)   == NOT_ENOUGH_MEMORY
     || allocate_instruction(&lh,   I, 0, 23, 3, memory_instructions_operands_types)   == NOT_ENOUGH_MEMORY
     || allocate_instruction(&sh,   I, 0, 24, 3, memory_instructions_operands_types)   == NOT_ENOUGH_MEMORY
    )
        return NOT_ENOUGH_MEMORY;

    /* Put the instructions in the dictionary_ */
    if (dictionary_put(&dict, "addi", (void*)addi) == NOT_ENOUGH_MEMORY
     || dictionary_put(&dict, "subi", (void*)subi) == NOT_ENOUGH_MEMORY
     || dictionary_put(&dict, "andi", (void*)andi) == NOT_ENOUGH_MEMORY
     || dictionary_put(&dict, "ori",  (void*)ori)  == NOT_ENOUGH_MEMORY
     || dictionary_put(&dict, "nori", (void*)nori) == NOT_ENOUGH_MEMORY
     || dictionary_put(&dict, "bne",  (void*)bne)  == NOT_ENOUGH_MEMORY
     || dictionary_put(&dict, "beq",  (void*)beq)  == NOT_ENOUGH_MEMORY
     || dictionary_put(&dict, "blt",  (void*)blt)  == NOT_ENOUGH_MEMORY
     || dictionary_put(&dict, "bgt",  (void*)bgt)  == NOT_ENOUGH_MEMORY
     || dictionary_put(&dict, "lb",   (void*)lb)   == NOT_ENOUGH_MEMORY
     || dictionary_put(&dict, "sb",   (void*)sb)   == NOT_ENOUGH_MEMORY
     || dictionary_put(&dict, "lw",   (void*)lw)   == NOT_ENOUGH_MEMORY
     || dictionary_put(&dict, "sw",   (void*)sw)   == NOT_ENOUGH_MEMORY
     || dictionary_put(&dict, "lh",   (void*)lh)   == NOT_ENOUGH_MEMORY
     || dictionary_put(&dict, "sh",   (void*)sh)   == NOT_ENOUGH_MEMORY
    )
        return NOT_ENOUGH_MEMORY;

    return SUCCESS;
}

/**
 * Adds J instructions to the global dictionary. (Must be initialized!)
 * @return NOT_ENOUGH_MEMORY if it happens, else - SUCCESS.
 */
static status add_J_instructions()
{
    /* Operands types array for only one operand - register or labal. */
    static operand_type register_or_label_operand_type[] = {LABEL_OR_REGISTER};

    /* Operands types array for only one operand - labal. */
    static operand_type label_operand_type[] = {LABEL};

    instruction *jmp, *la, *call, *stop;

    /* Fill all the instructions with data */
    if (allocate_instruction(&jmp,  J, 30, 0, 2, register_or_label_operand_type) == NOT_ENOUGH_MEMORY
     || allocate_instruction(&la,   J, 31, 0, 1, label_operand_type) == NOT_ENOUGH_MEMORY
     || allocate_instruction(&call, J, 32, 0, 1, label_operand_type) == NOT_ENOUGH_MEMORY
     || allocate_instruction(&stop, J, 63, 0, 0, NULL) == NOT_ENOUGH_MEMORY
    )
        return NOT_ENOUGH_MEMORY;

    /* Put the instructions in the dictionary_ */
    if (dictionary_put(&dict, "jmp",  (void*)jmp)  == NOT_ENOUGH_MEMORY
     || dictionary_put(&dict, "la",   (void*)la)   == NOT_ENOUGH_MEMORY
     || dictionary_put(&dict, "call", (void*)call) == NOT_ENOUGH_MEMORY
     || dictionary_put(&dict, "stop", (void*)stop) == NOT_ENOUGH_MEMORY
    )
        return NOT_ENOUGH_MEMORY;

    return SUCCESS;
}

status instructions_table_init()
{
    dict = dictionary_create();

    if (add_R_instructions() == NOT_ENOUGH_MEMORY || add_I_instructions() == NOT_ENOUGH_MEMORY || add_J_instructions() == NOT_ENOUGH_MEMORY)
        return NOT_ENOUGH_MEMORY;

    return SUCCESS;
}

status instructions_table_get_instruction(char *name, instruction *inst)
{
    if (dictionary_get(dict, name, (void*)inst) == KEY_DOES_NOT_EXIST) 
        return INSTRUCTION_DOES_NOT_EXIST;

    return SUCCESS;
}
