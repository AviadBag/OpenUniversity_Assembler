#include "str_helper.h"
#include <ctype.h>

void skip_whitespaces(char **ptr)
{
    while (isspace(**ptr) && **ptr)
        (*ptr)++;
}

boolean has_whitespaces(char *str)
{
    while (*str)
    {
        if (isspace(*str))
            return true;

        str++;
    }

    return false;
}

boolean is_number(char* str)
{
    /* A number cannot be empty */
    if (!*str)
        return false;

    /* There can be a minus sign at the beginning */
    if (*str == '-')
        str++;

    while (*str)
    {
        if (!isdigit(*str++))
            return false;
    }

    return true;
}