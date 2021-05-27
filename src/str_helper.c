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