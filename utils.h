

#ifndef _MMN14_UTILS_H
#define _MMN14_UTILS_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>
#include "string_utils.h"

#define LINE_SIZE 81 /* Maximum line length in source file is 80 characters (excluding \n). */
#define MAX_ERROR_LENGTH 160 /* Error message could include variable of 80 chars excluding the massage */
#define FILE_NAME 256 /* In linux the maximum length for a filename is 255 (excluding \0) */
#define START_MEMORY 100 /* Assuming code starts at address 100 */
#define MAX_MEMORY 256 /* This machine can only have 256 words (each word is 14 bits) */
#define NO_ARGUMENT 0

enum err{ERROR = -1, NO_ERROR};
enum flag{off, on};

void assembler(int number_of_arg, char *file[]);

FILE* open_file(const char* filename, const char* mode);

void generate_filename(const char *filename, const char *suffix, char **new_filename);

int search_string_in_file(FILE *fp, const char *string);

void close_files(int count, ...);

void delete_file(const char *file_name, const char *suffix);

void free_pointers(int count, ...);



#endif /*_MMN14_UTILS_H */
