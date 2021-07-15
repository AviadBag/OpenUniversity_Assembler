#include "linked_list.h"

#include <stdio.h>
#include <string.h>

#include "first_walk.h"
#include "linked_list.h"
#include "symbol.h"
#include "boolean.h"

#define DESIRED_INPUT_FILE_EXT "as"

/**
 * @brief Checks if the given file name ends with DESIRED_INPUT_FILE_EXT
 * 
 * @param file_name The file name to check.
 * @return boolean True or False.
 */
boolean has_legal_extension(char* file_name)
{
    char* dot = strchr(file_name, '.');
    if (!dot)
        return false; /* There is no extenstion... */

    dot++; /* Skip the '.' */
    return strcmp(dot, DESIRED_INPUT_FILE_EXT) == 0;
}

void print_symbol(void* data)
{
    symbol* s = (symbol*) (data);
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
    printf(", symbol is %s\n", s->is_entry ? "entry" : "not entry");
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
void compile(char* file_name)
{
    symbols_table st;
    first_walk_status fw_status;
    FILE* file;
    
    if (!has_legal_extension(file_name))
    {
        printf("Error: File \"%s\" has no \".%s\" extension. Skipping.\n", file_name, DESIRED_INPUT_FILE_EXT);
        return;
    }

    file = fopen(file_name, "r");
    if (!file)
    {
        printf("Error: Cannot open file \"%s\". Skipping.\n", file_name);
        return;
    }

    fw_status = first_walk(file, &st);
    if (fw_status == FIRST_WALK_NOT_ENOUGH_MEMORY)
    {
        printf("Error: Not enough memory!\n");
        return;
    }
    if (fw_status != FIRST_WALK_OK)
        return;
    
    print_symbols_table(st);

    /* Clean up */
    linked_list_free(st);
}

int main(int argc, char* argv[])
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