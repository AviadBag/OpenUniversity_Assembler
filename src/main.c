#include <stdio.h>
#include "instructions_table.h"
#include "parser.h"
#include "validator.h"
#include "command.h"
#include "directives_table.h"
#include "str_helper.h"
#include "translator.h"

void printBits(size_t const size, void const * const ptr)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;
    
    for (i = size-1; i >= 0; i--) {
        for (j = 7; j >= 0; j--) {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
    }
    puts("");
}

int main()
{
    machine_instruction m;
	char* command_str;
	command cmd;
	int line;

	command_str = "label: blt $5, $24, loop";
	line = 17;
	printf("Parser Output: %s\n", parser_status_to_string(parser_parse(command_str, &cmd, line)));
	printf("Validator Output: %s\n", validator_status_to_string(validator_validate(cmd, line)));
	m = translator_translate(cmd);
	printf("Machine Instruction: ");
	printBits(sizeof(machine_instruction), &m);

    return 0;
}