#include <stdio.h>
#include "instructions_table.h"
#include "parser.h"
#include "validator.h"
#include "command.h"
#include "directives_table.h"

void check_validator()
{
    command cmd;
    parser_status p_status;
    validator_status v_status;
    
    printf("check_validator():\n");

    p_status = parser_parse("w:   sw 1, 2", &cmd, 17);
    printf("%s\n", parser_status_to_string(p_status));
    if (command_has_label(cmd))
        printf("Label: %s\n", cmd.label);
    printf("Command: %s\n", cmd.command_name);
    if (cmd.type == DIRECTIVE)
        printf("Command type: Directive\n");
    else
        printf("Command type: Instruction\n");

    v_status = validator_validate(cmd);
    printf("%s\n", validator_status_to_string(v_status));
}

void check_directives_table()
{
    directive* d;
    directives_table_status s;

    printf("check_directives_table():\n");

    s = directives_table_get_directive("db", &d);
    if (s == DT_OK)
        printf("Directive name = %s\n", d->name);
    else
        printf("ERROR\n");
}

void check_instructions_table()
{
    instruction* i;
    instructions_table_status s;

    printf("check_instructions_table():\n");

    s = instructions_table_get_instruction("sw", &i);
    if (s == IT_OK)
        printf("Instruction name = %s\n", i->name);
    else
        printf("ERROR\n");
}

int main()
{
    check_validator();

    return 0;
}