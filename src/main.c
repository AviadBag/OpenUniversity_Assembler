#include <stdio.h>
#include "instructions_table.h"
#include "parser.h"
#include "validator.h"
#include "command.h"

int main()
{
    command cmd;
    parser_status p_status;
    validator_status v_status;
    
    instructions_table_init();

    p_status = parser_parse("label:   add  $4, 5 , $3", &cmd, 17);
    printf("%s\n", parser_status_to_string(p_status));
    if (command_has_label(cmd))
        printf("Label: %s\n", cmd.label);
    printf("Command: %s\n", cmd.command_name);

    v_status = validator_validate(cmd);
    printf("%s\n", validator_status_to_string(v_status));

    instructions_table_free();

    return 0;
}