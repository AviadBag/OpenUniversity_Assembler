#include "str_helper.h"
#include <ctype.h>

void skip_whitespaces(char** ptr)
{
    while (isspace(**ptr) && **ptr)
        (*ptr)++;
}