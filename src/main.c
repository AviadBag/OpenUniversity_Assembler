#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "first_walk.h"
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

    file_writer_status object_status, entries_status, externals_status;

    if (!has_legal_extension(file_name))
    {
        printf("Error: File \"%s\" has no \".%s\" extension. Skipping.\n", file_name, DESIRED_INPUT_FILE_EXT);
        return;
    }

    fw_status = first_walk(file_name, &st);
    if (fw_status == WALK_NOT_ENOUGH_MEMORY)
    {
        printf("Error: Not enough memory!\n");
        goto first_walk_free;
    }
    if (fw_status != WALK_OK) /* If it another error, I already logged it */
        goto first_walk_free;

    sw_status = second_walk(file_name, &st, &data_image, &dcf, &code_image, &icf);
    if (sw_status == WALK_NOT_ENOUGH_MEMORY)
    {
        printf("Error: Not enough memory!\n");
        goto second_walk_free;
    }
    if (sw_status != WALK_OK) /* If it another error, I already logged it */
        goto second_walk_free;

    object_status = write_object_file(file_name, data_image, dcf, code_image, icf);
    entries_status = write_entries_file(file_name, st);
    externals_status = write_externals_file(file_name, st);
    if (object_status == FILE_WRITER_NOT_ENOUGH_MEMORY || entries_status == FILE_WRITER_NOT_ENOUGH_MEMORY || externals_status == FILE_WRITER_NOT_ENOUGH_MEMORY)
        printf("Error: Not enough memory!\n");
    /* If there is another error, I already logged it, and we can continue to clean up. Else, we can continue to clean up... */

    /* Clean up */
    second_walk_free:
    if (data_image) /* If it is null, it was not allocated! */
        free(data_image);
    if (code_image) /* If it is null, it was not allocated! */
        free(code_image);

    first_walk_free:
    free_symbols_table(st); /* Nothing will happen if it was not allocated */
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