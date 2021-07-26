#include "linked_list.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "first_walk.h"
#include "linked_list.h"
#include "symbol.h"
#include "boolean.h"
#include "second_walk.h"
#include "file_writer.h"

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

    if (write_entries_file(file_name, st) != FILE_WRITER_OK || write_externals_file(file_name, st) != FILE_WRITER_OK || write_object_file(file_name, data_image, dcf, code_image, icf) != FILE_WRITER_OK)
        return;

    /* Clean up */
    free_symbols_table(st);
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