

#ifndef MMN14_UTILS_H
#define MMN14_UTILS_H

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
#define LABEL_SIZE 31 /* Maximum label name length with null terminator */
#define HASH_MAP_SIZE 256  /* Maximum 256 words in the system */

enum err{ERROR = -1, NO_ERROR};
typedef enum boolean { false = -1, true = 1 } bool;
enum flag{off, on};

/* symbol entry struct*/
typedef struct symbol_entry {
    char* label;
    int address; /* decimal address */
    bool is_code;
    bool is_data;
    bool is_extern;
    bool is_entry;
    struct symbol_entry* next;
} symbol_entry;

/* symbol table struct*/
typedef struct symbolTable {
    symbol_entry* head;
} symbol_table;

typedef unsigned short word;

typedef struct hash_node {
    int decimal_address;
    char code[LABEL_SIZE];
    unsigned short int binary_address;
} hash_node;

typedef struct hash_map {
    struct hash_node nodes[HASH_MAP_SIZE];
} hash_map;

void assembler(int number_of_arg, char *file[]);

FILE* open_file(char* filename, const char* mode);

void generate_filename(char *filename, char *suffix, char **new_filename);

int search_string_in_file(FILE *fp, const char *string);

void close_files(int count, ...);

void delete_file(char *file_name, char *suffix);

void free_pointers(int count, ...);



#endif /*MMN14_UTILS_H */
