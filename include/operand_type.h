#ifndef _OPERAND_TYPE_H
#define _OPERAND_TYPE_H

typedef enum e_operand_type
{
    REGISTER,
    CONSTANT,
    LABEL,
    LABEL_OR_REGISTER
} operand_type;

#endif