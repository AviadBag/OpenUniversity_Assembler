#include "translator.h"
#include "instructions_table.h"
#include "bitmap.h"

#include <stdlib.h>

#define R_COPY_INSTRUCTIONS_NUMBER_OF_OPERANDS 2

/**
 * @brief Converts a register string (line "$3") to it's int representation. (Like 3).
 * 
 * @param register_str The register string. MUST be validated.
 * @return int         The register number.
 */
static int register_string_to_int(char* register_str)
{
	return atoi(++register_str); /* Increment to skip the '$' */
}

/**
 * @brief Translates an R instruction into it's machine language representation.
 *
 * @param cmd  The command to translate
 * @param inst The instruction struct that represents the insturction.
 *
 * @return 
 */
static machine_instruction translate_R_instruction(command cmd, instruction inst)
{
	machine_instruction m = 0;
	int rs, rt, rd;
	
	bitmap_put_data(&m, &inst.opcode, 26, 31); /* Put opcode */
	bitmap_put_data(&m, &inst.funct, 6, 10);   /* Put funct */

	if (inst.number_of_operands == R_COPY_INSTRUCTIONS_NUMBER_OF_OPERANDS)
	{
		/* This is a copy instruction. */
		rd = register_string_to_int(cmd.operands[0]);
		rs = register_string_to_int(cmd.operands[1]);
		rt = 0;
	}
	else /* This is an arthimetic-login instruction. */
	{
		rs = register_string_to_int(cmd.operands[0]);
		rt = register_string_to_int(cmd.operands[1]);
		rd = register_string_to_int(cmd.operands[2]);
	}

	bitmap_put_data(&m, &rs, 21, 25); /* Put rs */
	bitmap_put_data(&m, &rt, 16, 20); /* Put rt */
	bitmap_put_data(&m, &rd, 11, 15); /* Put rd */

	return m;
}

/**
 * @brief Translates an I instruction into it's machine language representation.
 *
 * @param cmd  The command to translate
 * @param inst The instruction struct that represents the insturction.
 *
 * @return 
 */
static machine_instruction translate_I_instruction(command cmd, instruction inst)
{
	return 0;
}

/**
 * @brief Translates an J instruction into it's machine language representation.
 *
 * @param cmd  The command to translate
 * @param inst The instruction struct that represents the insturction.
 *
 * @return 
 */
static machine_instruction translate_J_instruction(command cmd, instruction inst)
{
	return 0;
}

machine_instruction translator_translate(command cmd)
{
	instruction* inst;
	instructions_table_get_instruction(cmd.command_name, &inst);
	if (inst->type == R)	
		return translate_R_instruction(cmd, *inst);
	else if (inst->type == I)
		return translate_I_instruction(cmd, *inst);
	else /* J */
		return translate_J_instruction(cmd, *inst);

	return 0;		
}
