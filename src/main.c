#include "linked_list.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "first_walk.h"
#include "linked_list.h"
#include "symbol.h"
#include "boolean.h"
#include "walk.h"
#include "second_walk.h"

#define DESIRED_INPUT_FILE_EXT "as"

/**
 * @brief Checks if the given file name ends with DESIRED_INPUT_FILE_EXT
 * 
 * @param file_name The file name to check.
 * @return boolean True or False.
 */
boolean has_legal_extension(char *file_name)
{
    char *dot = strrchr(file_name, '.');
    if (!dot)
        return false; /* There is no extenstion... */

    dot++; /* Skip the '.' */
    return strcmp(dot, DESIRED_INPUT_FILE_EXT) == 0;
}

void print_symbol(void *data)
{
    int i;
    symbol *s = (symbol *)(data);
    printf("Symbol name: %s", s->name);
    printf(", Symbol type: ");
    switch (s->type)
    {
    case CODE:
        printf("CODE");
        break;
    case DATA:
        printf("DATA");
        break;
    case EXTERNAL:
        printf("EXTERNAL");
        break;
    }
    printf(", symbol value: %lu", s->value);
    printf(", symbol is %s", s->is_entry ? "entry" : "not entry");

    for (i = 0; i < linked_list_length(s->instructions_using_me); i++)
    {
        if (i == 0)
            printf(", Instructions Using Me: %lu", *((unsigned long *)linked_list_get(s->instructions_using_me, i)));
        else
            printf(", %lu", *((unsigned long *)linked_list_get(s->instructions_using_me, i)));
    }

    printf("\n");
}

void print_symbols_table(symbols_table st)
{
    int i;
    for (i = 0; i < linked_list_length(st); i++)
    {
        print_symbol(linked_list_get(st, i));
    }
}

/**
 * @brief Compiles the given assembly file.
 * 
 * @param file_name The file to compile.
 */
void compile(char *file_name)
{
    symbols_table st;
    unsigned char *code_image, *data_image;
    unsigned long dcf, icf;
    int i;
    walk_status fw_status;
    walk_status sw_status;

    if (!has_legal_extension(file_name))
    {
        printf("Error: File \"%s\" has no \".%s\" extension. Skipping.\n", file_name, DESIRED_INPUT_FILE_EXT);
        return;
    }

    fw_status = first_walk(file_name, &st);
    if (fw_status == WALK_NOT_ENOUGH_MEMORY)
    {
        printf("Error: Not enough memory!\n");
        return;
    }
    if (fw_status != WALK_OK) /* If it another error, I already logged it */
        return;

    sw_status = second_walk(file_name, &st, &data_image, &dcf, &code_image, &icf);
    if (sw_status == WALK_NOT_ENOUGH_MEMORY)
    {
        printf("Error: Not enough memory!\n");
        return;
    }
    if (sw_status != WALK_OK) /* If it another error, I already logged it */
        return;

    print_symbols_table(st);

    for (i = IC_DEFAULT_VALUE; i < icf; i++)
    {
        printf("%-2.2X ", code_image[i]);
        if ((i + 1) % 4 == 0)
            printf("\n");
    }
    printf("\n");

    for (i = DC_DEFAULT_VALUE; i < dcf; i++)
    {
        printf("%-2.2X ", data_image[i]);
        if ((i + 1) % 4 == 0)
            printf("\n");
    }
    printf("\n");

    /* Clean up */
    linked_list_free(st);
    free(data_image);
    free(code_image);
}

int main(int argc, char *argv[])
{
    int i;

    if (argc == 1)
    {
        printf("Usage: \"%s file1.asm file2.asm ...\"\n", argv[0]);
        return 1;
    }

    for (i = 1; i < argc; i++)
        compile(argv[i]);

    return 0;
}