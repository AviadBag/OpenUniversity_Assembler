#include <stdio.h>
#include <string.h>

#include "first_walk.h"
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

/**
 * @brief Compiles the given assembly file.
 * 
 * @param file_name The file to compile.
 */
void compile(char* file_name)
{
    symbols_table st;
    first_walk_status fw_status;
    
    if (!has_legal_extension(file_name))
    {
        printf("Error: File \"%s\" has no \".%s\" extension. Skipping.\n", file_name, DESIRED_INPUT_FILE_EXT);
        return;
    }

    fw_status = first_walk(file_name, &st);
    if (fw_status != FIRST_WALK_OK)
        return;
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