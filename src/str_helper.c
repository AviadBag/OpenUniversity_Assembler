#include "str_helper.h"
#include <ctype.h>

void skip_whitespaces(char **ptr)
{
    while (isspace(**ptr) && **ptr)
        (*ptr)++;
}

boolean is_number(char* str)
{
    /* A number cannot be empty */
    if (!*str)
        return false;

    /* There can be a minus or plus sign at the beginning */
    if (*str == '-' || *str == '+')
        str++;

    while (*str)
    {
        if (!isdigit(*str++))
            return false;
    }

    return true;
}