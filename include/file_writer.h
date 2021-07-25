#ifndef __FILE_WRITER_H__
#define __FILE_WRITER_H__

/* The final module - Writes all of the generated data to a file! */

#include "walk.h"

typedef enum e_file_writer_status
{
    FILE_WRITER_IO_ERROR,
    FILE_WRITER_OK
} file_writer_status;

/**
 * @brief Creates and object file.
 * 
 * @param file_name           The name of the original input file.
 * @param data_image          A pointer to the data image.
 * @param dcf_p               The DCF
 * @param code_image          A pointer to the code image.
 * @param icf_p               The ICF
 * @return file_writer_status FILE_WRITER_IO_ERROR or FILE_WRITER_OK.
 */
file_writer_status write_object_file(char* original_file_name, unsigned char *data_image, unsigned long *dcf_p, unsigned char code_image, unsigned long icf_p);

/**
 * @brief Creates an entries files.
 * 
 * @param st                  The symbols table.
 * @return file_writer_status FILE_WRITER_IO_ERROR or FILE_WRITER_OK.
 */
file_writer_status write_entries_file(char* original_file_name, symbols_table st);

/**
 * @brief Creates an externals files.
 * 
 * @param st                  The symbols table.
 * @return file_writer_status FILE_WRITER_IO_ERROR or FILE_WRITER_OK.
 */
file_writer_status write_externals_file(char* original_file_name, symbols_table st);

#endif