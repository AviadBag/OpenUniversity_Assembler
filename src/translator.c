#include "translator.h"
#include "instructions_table.h"
#include "bitmap.h"
#include "walk.h"
#include "symbol.h"
#include "utils.h"
#include "logger.h"

#include <stdlib.h>
#include <string.h>

#define R_COPY_INSTRUCTIONS_NUMBER_OF_OPERANDS 2

#define RS_START 21
#define RS_END 25

#define RT_START 16
#define RT_END 20

#define RD_START 11
#define RD_END 15

#define OPCODE_START 26
#define OPCODE_END 31

#define FUNCT_START 6
#define FUNCT_END 10

#define IMMED_START 0
#define IMMED_END 15

#define REG_START 25
#define REG_END 25

#define ADDRESS_START 0
#define ADDRESS_END 24

#define I_INSTRUCTION_IMMED_SIZE_BITS (IMMED_END - IMMED_START + 1)

#define TRANSLATOR "Translator"
#define PROBLEM_WITH_CODE "ProblemWithCode"
#define OVERFLOW "Overflow"

/**
 * @brief Converts a register string (line "$3") to it's int representation. (Like 3).
 * 
 * @param register_str The register string. MUST be validated.
 * @return int         The register number.
 */
static int register_string_to_int(char *register_str)
{
	return atoi(++register_str); /* Increment to skip the '$' */
}

/**
 * @brief Translates an R instruction into it's machine language representation.

 * @param m                  A pointer to a machine_instruction; Will be filled with the instruction.
 * @param cmd  The command to translate. MUST BE VALIDATED!
 * @param inst The instruction struct that represents the insturction.
 */
void translate_R_instruction(machine_instruction *m, command cmd, instruction inst)
{
	int rs, rt, rd;

	bitmap_put_data(m, &inst.opcode, OPCODE_START, OPCODE_END); /* Put opcode */
	bitmap_put_data(m, &inst.funct, FUNCT_START, FUNCT_END);	/* Put funct */

	if (inst.number_of_operands == R_COPY_INSTRUCTIONS_NUMBER_OF_OPERANDS)
	{
		/* This is a copy instruction. */
		rs = register_string_to_int(cmd.operands[0]);
		rd = register_string_to_int(cmd.operands[1]);
		rt = 0;
	}
	else /* This is an arthimetic-login instruction. */
	{
		rs = register_string_to_int(cmd.operands[0]);
		rt = register_string_to_int(cmd.operands[1]);
		rd = register_string_to_int(cmd.operands[2]);
	}

	bitmap_put_data(m, &rs, RS_START, RS_END); /* Put rs */
	bitmap_put_data(m, &rt, RT_START, RT_END); /* Put rt */
	bitmap_put_data(m, &rd, RD_START, RD_END); /* Put rd */
}

/**
 * @brief Translates an I instruction into it's machine language representation.
 *
 * @param m                  A pointer to a machine_instruction; Will be filled with the instruction.
 * @param cmd                The command to translate. MUST BE VALIDATED!
 * @param inst               The instruction struct that represents the insturction.
 * @param st                 The symbols table.
 * @param ic                 The current instruction counter.
 * @param line                 On what line this instruction is?
 * @return translator_status TRANSLATOR_OK or TRANSLATOR_LABEL_DOES_NOT_EXIST or TRANSLATOR_OVERFLOW.
 */
static translator_status translate_I_instruction(machine_instruction *m, command cmd, instruction inst, symbols_table st, unsigned long ic, int line)
{
	int rs, rt;
	int immed;
	symbol *symbol_p;
	int offset;

	/* Put the opcode */
	bitmap_put_data(m, &inst.opcode, OPCODE_START, OPCODE_END);

	/* There are two operands arrangement. A way to distinguish between them is to know that "conditional jump" gets
	   a label as the third operand.
	*/
	if (inst.operands_types[2] == LABEL)
	{
		/* Conditional jump */
		rs = register_string_to_int(cmd.operands[0]);
		rt = register_string_to_int(cmd.operands[1]);
		symbol_p = find_symbol(cmd.operands[2], st);
		if (!symbol_p)
		{
			logger_log(TRANSLATOR, PROBLEM_WITH_CODE, line, "Label \"%s\" does not exist", cmd.operands[2]);
			return TRANSLATOR_LABEL_DOES_NOT_EXIST;
		}
		if (symbol_p->type == EXTERNAL)
		{
			logger_log(TRANSLATOR, PROBLEM_WITH_CODE, line, "Conditional jumps cannot get external labels as an argument");
			return TRANSLATOR_LABEL_DOES_NOT_EXIST;
		}

		offset = (int) (symbol_p->value - ic);
		if (!is_in_range_2_complement(offset, I_INSTRUCTION_IMMED_SIZE_BITS))
		{
			logger_log(TRANSLATOR, OVERFLOW, line, "Label \"%s\" is too far!", cmd.operands[2]);
			return TRANSLATOR_OVERFLOW;
		}
		immed = offset;
	}
	else
	{
		/* Arthimetic logic or memory instructions */
		rs = register_string_to_int(cmd.operands[0]);
		rt = register_string_to_int(cmd.operands[2]);
		immed = atoi(cmd.operands[1]);
	}

	bitmap_put_data(m, &rs, RS_START, RS_END);			/* Put rs */
	bitmap_put_data(m, &rt, RT_START, RT_END);			/* Put rt */
	bitmap_put_data(m, &immed, IMMED_START, IMMED_END); /* Put immed */

	return TRANSLATOR_OK;
}

/**
 * @brief Translates an J instruction into it's machine language representation.
 *
 * @param m                  A pointer to a machine_instruction; Will be filled with the instruction.
 * @param cmd                The command to translate. MUST BE VALIDATED!
 * @param inst               The instruction struct that represents the insturction.
 * @param st                 The symbols table.
 * @param line               On what line this instruction is?
 * @return translator_status TRANSLATOR_OK or TRANSLATOR_LABEL_DOES_NOT_EXIST or TRANSLATOR_OVERFLOW.
 */
static translator_status translate_J_instruction(machine_instruction *m, command cmd, instruction inst, symbols_table st, int line)
{
	int reg = 0;
	unsigned long address;
	symbol *symbol_p;
	char* label;

	/* Put the opcode */
	bitmap_put_data(m, &inst.opcode, OPCODE_START, OPCODE_END);

	if (strcmp(cmd.command_name, "stop") == 0) /* The only J instruction that does not accept a label */
		address = 0;
	else
	{
		if (*cmd.operands[0] == '$') /* This is a jmp instruction with a register */
		{
			reg = 1;
			address = register_string_to_int(cmd.operands[0]);
		}
		else
		{
		    label = cmd.operands[0];
			symbol_p = find_symbol(label, st);
			if (!symbol_p)
			{
				logger_log(TRANSLATOR, PROBLEM_WITH_CODE, line, "Label \"%s\" does not exist", label);
				return TRANSLATOR_LABEL_DOES_NOT_EXIST;
			}

			address = symbol_p->value;
			if (!is_in_range_2_complement(address, I_INSTRUCTION_IMMED_SIZE_BITS))
			{
				logger_log(TRANSLATOR, OVERFLOW, line, "Label \"%s\" is too far!", label);
				return TRANSLATOR_OVERFLOW;
			}
		}
	}

	bitmap_put_data(m, &address, ADDRESS_START, ADDRESS_END);
	bitmap_put_data(m, &reg, REG_START, REG_END);

	return TRANSLATOR_OK;
}

translator_status translator_translate(command cmd, symbols_table st, unsigned long ic, int line, machine_instruction *m)
{
	instruction *inst;
	instructions_table_get_instruction(cmd.command_name, &inst);
	*m = 0;
	if (inst->type == R)
		translate_R_instruction(m, cmd, *inst);
	else if (inst->type == I)
		return translate_I_instruction(m, cmd, *inst, st, ic, line);
	else /* J */
		return translate_J_instruction(m, cmd, *inst, st, line);

	return TRANSLATOR_OK;
}
