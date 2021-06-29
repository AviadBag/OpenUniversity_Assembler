#ifndef _OPERAND_TYPE_H
#define _OPERAND_TYPE_H

typedef enum e_operand_type
{
    REGISTER,
    CONSTANT_BYTE,
    CONSTANT_HALF,
    CONSTANT_WORD,
    LABEL,
    LABEL_OR_REGISTER,
    STRING
} operand_type;

/**
 * @brief Converts an operand_type object to it's string representation.
 * @param o The operand_type object.
 * @return char* The string representation.
 */
char* operand_type_to_string(operand_type o);

#endif