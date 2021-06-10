#include <stdio.h>
#include "instructions_table.h"
#include "parser.h"
#include "validator.h"
#include "command.h"
#include "directives_table.h"

void check_validator()
{
    command cmd;
    directive* d;
    parser_status p_status;
    validator_status v_status;
    
    printf("check_validator():\n");

    instructions_table_init();

    p_status = parser_parse("label:   add  $4, 5 ", &cmd, 17);
    printf("%s\n", parser_status_to_string(p_status));
    if (command_has_label(cmd))
        printf("Label: %s\n", cmd.label);
    printf("Command: %s\n", cmd.command_name);

    v_status = validator_validate(cmd);
    printf("%s\n", validator_status_to_string(v_status));

    instructions_table_free();

    printf("------------\n");
    directives_table_get_directive("A", &d);
    printf("%d\n", d->number_of_operands);
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

int main()
{
    check_directives_table();

    return 0;
}