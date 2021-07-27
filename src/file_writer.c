#include "file_writer.h"
#include "linked_list.h"
#include "symbol.h"
#include "walk.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SOURCE_EXT    "as"
#define OBJECT_EXT    "ob"
#define EXTERNALS_EXT "ext"
#define ENTRIES_EXT   "ent"

/**
 * @brief Changes the extension of <file_name> to <new_ext>.
 * 
 * @param original_file_name The file to change. IT'S EXTENSION MUST BE <SOURCE_EXT>!
 * @param new_ext            The new extension to put. MUST NOT BE "".
 * @return char*             A pointer to the new file name, or NULL if malloc() faild. THE MEMORY MUST BE FREED AFTER USAGE!
 */
char* change_extension(char* original_file_name, char* new_ext)
{
    /* Create the result buffer */
    char* dot; /* Will point to the dot of the extension */
    int original_file_name_len = strlen(original_file_name);
    int original_ext_len = strlen(SOURCE_EXT);
    int new_ext_len = strlen(new_ext);
    int new_file_name_length = original_file_name_len - original_ext_len + new_ext_len;
    char* new_file_name = (char*) malloc(new_file_name_length + 1);
    if (!new_file_name)
        return NULL;
    memset(new_file_name, 0, new_file_name_length + 1);

    strncpy(new_file_name, original_file_name, new_file_name_length);
    dot = strrchr(new_file_name, '.');
    *(dot+1) = '\0';
    strcat(new_file_name, new_ext);

    return new_file_name;
}

file_writer_status write_externals_file(char* original_file_name, symbols_table st)
{
    int i, j;
    char* new_file_name = change_extension(original_file_name, EXTERNALS_EXT);
    FILE* file = fopen(new_file_name, "w");
    if (!file)
    {
        printf("Error: Cannot open file \"%s\". Skipping.\n", new_file_name);
        return FILE_WRITER_IO_ERROR;
    }

    for (i = 0; i < linked_list_length(st); i++)
    {
        symbol* symbol_p = linked_list_get(st, i);
        if (symbol_p->type == EXTERNAL)
        {
            for (j = 0; j < linked_list_length(symbol_p->instructions_using_me); j++)
            {
                fprintf(file, "%s %04lu\n", symbol_p->name, *((unsigned long*) linked_list_get(symbol_p->instructions_using_me, j)));
            }
        }
    }

    free(new_file_name);
    fclose(file);
    
    return FILE_WRITER_OK;
}

file_writer_status write_entries_file(char* original_file_name, symbols_table st)
{
    int i;
    char* new_file_name = change_extension(original_file_name, ENTRIES_EXT);
    FILE* file = fopen(new_file_name, "w");
    if (!file)
    {
        printf("Error: Cannot open file \"%s\". Skipping.\n", new_file_name);
        return FILE_WRITER_IO_ERROR;
    }

    for (i = 0; i < linked_list_length(st); i++)
    {
        symbol* symbol_p = linked_list_get(st, i);
        if (symbol_p->is_entry)
        {
            fprintf(file, "%s %04lu\n", symbol_p->name, symbol_p->value);
        }
    }

    free(new_file_name);
    fclose(file);

    return FILE_WRITER_OK;
}

file_writer_status write_object_file(char* original_file_name, unsigned char *data_image, unsigned long dcf, unsigned char *code_image, unsigned long icf)
{
    unsigned long i;
    char* new_file_name = change_extension(original_file_name, OBJECT_EXT);
    FILE* file = fopen(new_file_name, "w");
    if (!file)
    {
        printf("Error: Cannot open file \"%s\". Skipping.\n", new_file_name);
        return FILE_WRITER_IO_ERROR;
    }

    /* Write ICF and DCF */
    fprintf(file, "%lu %lu\n", icf - IC_DEFAULT_VALUE, dcf);

    /* Write code image */
    for (i = 0; i < icf - IC_DEFAULT_VALUE; i++)
    {
        if ((i + 1) % 4 == 1)
            fprintf(file, "%04lu ", i + IC_DEFAULT_VALUE); /* Print the adderss only with the first in the row */
        fprintf(file, "%-2.2X ", code_image[i]);
        if ((i + 1) % 4 == 0)
            fprintf(file, "\n");
    }

    /* Write data image */
    for (i = 0; i < dcf; i++)
    {
        if ((i + 1) % 4 == 1)
            fprintf(file, "%04lu ", icf + i); /* Print the adderss only with the first in the row */
        fprintf(file, "%-2.2X ", data_image[i]);
        if ((i + 1) % 4 == 0)
            fprintf(file, "\n");
    }

    free(new_file_name);
    fclose(file);

    return FILE_WRITER_OK;
}