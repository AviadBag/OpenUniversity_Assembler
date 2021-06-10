#include "directives_table.h"
#include <string.h>

#define INFINITY -1
#define LENGTH_OF_ARRAY(arr) (sizeof(arr) / sizeof((arr)[0]))

static operand_type constant_byte_arr[] = {CONSTANT_BYTE};
static operand_type constant_half_arr[] = {CONSTANT_HALF};
static operand_type constant_word_arr[] = {CONSTANT_WORD};
static operand_type string_arr[] = {STRING};
static operand_type label_arr[] = {LABEL};

static directive directives_arr[] = {
    {"db", INFINITY, constant_byte_arr},
    {"dh", INFINITY, constant_half_arr},
    {"dw", INFINITY, constant_word_arr},
    {"asciz", 1, string_arr},
    {"entry", 1, label_arr},
    {"extern", 1, label_arr}};

directives_table_status directives_table_get_directive(char *name, directive **dir)
{
    int i;

    for (i = 0; i < LENGTH_OF_ARRAY(directives_arr); i++)
    {
        if (strcmp(directives_arr[i].name, name) == 0)
        {
            *dir = &directives_arr[i];
            return DT_OK;
        }
    }

    return DT_DIRECTIVE_DOES_NOT_EXIST;
}