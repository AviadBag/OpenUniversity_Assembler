#include "command.h"

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