#include <stdio.h>
#include "instructions_table.h"
#include "parser.h"
#include "validator.h"
#include "command.h"

int main()
{
    command cmd;
    parser_status p_status;
    
    instructions_table_init();

    p_status = parser_parse("labe l: mov ax, b", &cmd, 17);
    printf("%s\n", parser_status_to_string(p_status));
    if (command_has_label(cmd))
        printf("Label: %s\n", cmd.label);

    instructions_table_free();

    return 0;
}