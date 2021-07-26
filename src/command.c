#include "command.h"

#include <stdlib.h>

char *command_type_to_string(command_type type)
{
    switch (type)
    {
    case DIRECTIVE:
        return "DIRECTIVE";
    case INSTRUCTION:
        return "INSTRUCTION";
    default:
        return "INVALID_COMMAND_TYPE_OBJECT";
    }
}

boolean command_has_label(command cmd)
{
    return cmd.label[0] != '\0';
}

void free_command(command cmd) 
{
    int i;

    free(cmd.command_name);
    for (i = 0; i < cmd.number_of_operands; i++)
        free(cmd.operands[i]);
    free(cmd.operands);
}
