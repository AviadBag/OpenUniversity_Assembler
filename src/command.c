#include "command.h"

#include <stdlib.h>

boolean command_has_label(command cmd)
{
    return cmd.label[0] != '\0';
}

void free_command(command cmd) 
{
    int i;

    if (cmd.command_name_allocated) free(cmd.command_name);
    for (i = 0; i < cmd.number_of_operands_allocated; i++)
        free(cmd.operands[i]);
    if (cmd.operands_array_allocated) free(cmd.operands);
}
