#include "operand_type.h"

char* operand_type_to_string(operand_type o)
{
    switch (o)
    {
        case REGISTER:
            return "REGISTER";
        case CONSTANT_BYTE:
            return "CONSTANT_BYTE";
        case CONSTANT_HALF:
            return "CONSTANT_HALF";
        case CONSTANT_WORD:
            return "CONSTANT_WORD";
        case LABEL:
            return "LABEL";
        case LABEL_OR_REGISTER:
            return "LABEL_OR_REGISTER";
        case STRING:
            return "STRING";
        default:
            return "OPERAND_TYPE_INVALID";
    }
}