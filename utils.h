

#ifndef MMN14_UTILS_H
#define MMN14_UTILS_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>

#define LINE_SIZE 81 /* Maximum line length in source file is 80 characters (excluding \n). */
#define START_MEMORY 100 /* Assuming code starts at address 100 */
#define LABEL_SIZE 31 /* Maximum label name length with null terminator */
#define HASH_MAP_SIZE 256  /* Maximum 256 words in the system */

#define ERROR_MSG "\x1b[31mERROR:\x1b[0m "
#define WARNING "\x1b[35mWARNING:\x1b[0m "
#define LINE "\x1b[34mLINE\x1b[0m  "
#define BLUE "\033[1;34m"
#define WHITE "\033[0m"
#define DELIMITER "\n \t"
#define COMMA ","
#define NEW_LINE "\n"
#define NO_ARGUMENT 0


enum err{ERROR = -1, NO_ERROR = 0};
typedef enum boolean {TRUE=0, false = 0, true = 1, FALSE=1} bool;

typedef struct mcr_node {
    char *name;
    char *data;
    struct mcr_node *next;
} macro;

typedef unsigned short word;

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
typedef struct symbol_table {
    symbol_entry* head;
} symbol_table;

typedef struct hash_node {
    int decimal_address;
    char code[LABEL_SIZE];
    word binary_address;
} hash_node;

typedef struct hash_map {
    hash_node nodes[HASH_MAP_SIZE];
} hash_map;

FILE* open_file(char* filename, const char* mode);

void generate_filename(char *filename, char *suffix, char **new_filename);

int search_string_in_file(FILE *fp, const char *string);

void close_files(int count, ...);

void delete_file(char *file_name, char *suffix);

void free_pointers(int count, ...);


/**
 * Checks if a line of text is empty or comment or contains only whitespaces.
 * @param line - The line of text to check.
 * @return Returns true if the line is empty, otherwise returns false.
 */
bool empty_line(char line[]);

/**
 * Removes comments from a string, if there are.
 * @param str - The string to remove the comment from.
 */
void remove_comment(char *str);

/**
 * Trim whitespace characters from the beginning and end of a string.
 * @param str - The string to be trimmed.
 */
void trim_whitespace(char *str);

/**
 * Trims the trailing whitespace of the given string by replacing it with a null terminator.
 * @param str - The string to trim.
 */
char * strtok_trimmed(char *str, const char *delim);

/**
 * Duplicates a string into dynamically allocated memory.
 * @param filename - The name of the file where the function is called from.
 * @param str - The string to be duplicated.
 * @return Returns a pointer to the duplicated string on success, or NULL on failure with an error massage.
 * @note The caller is responsible for freeing the returned memory.
 */
char *_strdup(char *filename, const char *str);

/**
 * Checks whether a given string is a reserved register name.
 * @param str - The string to check.
 * @return Returns true if the string is a reserved register name, false otherwise.
 */
bool is_register(const char *str);

/**
 * Checks whether the given instruction belongs to the first group of instructions.
 * The first group includes "mov", "cmp", "add", "sub", and "lea".
 * @param instruction - The instruction to check.
 * @return Returns true if the instruction belongs to the first group, false otherwise.
 */
bool first_group_instructions(const char *instruction);

/**
 * Checks if the given instruction belongs to the second group of instructions.
 * The second group consists of the instructions: "not", "clr", "inc", "dec", "jmp", "bne", "red", "prn", "jsr".
 * @param instruction - The instruction to check.
 * @return - Returns false if the instruction does not belong to the second group instructions, otherwise true.
 */
bool second_group_instructions(const char *instruction);

/**
 * Checks if the given instruction is a third group instruction (rts or stop).
 * @param instruction - The instruction to check.
 * @return - Returns false if the instruction is not a third group instruction, otherwise true.
 */
bool third_group_instructions(const char *instruction);

/**
 * Determines if a given string is a valid assembly instruction.
 * @param str - The string to check.
 * @return - Returns false if the string is a valid instruction, otherwise true.
 */
bool is_instruction(const char *str);

/**
 * Determines if a given string is a valid assembly directive.
 * @param str - The string to check.
 * @return - Returns true if the string is a valid directive, otherwise false.
 */
bool is_directive(const char *str);

/**
 * Checks whether the given string is a reserved name in the assembler.
 * @param str - The string to check for reserved name status.
 * @return Returns - true if the string is a reserved name, false otherwise.
 */
bool is_reserved(const char *str);

/**
 * Checks whether a given string is a label in the given symbol table.
 * @param st - A pointer to the symbol table to search for the label name.
 * @param str - The string to check.
 * @return Returns true if the label is found in the symbol table, false otherwise.
 */
bool is_label(symbol_table *st, char *str);

/**
 * Checks whether a given string represents a valid number".
 * @param str_num - The string to check.
 * @return Returns true if the string represents a valid number, false otherwise.
 */
bool is_num(char *str_num);

/**
 * Searches for a label with the given name in the symbol table and returns a pointer to its symbol_entry.
 * @param st - A pointer to the symbol table to search in.
 * @param str - The label name to search for.
 * @return Returns a pointer to the symbol_entry for the matching label if found, otherwise returns NULL.
 */
symbol_entry *get_label(symbol_table *st, char *str);

/**
 * Prints an error message to standard output and updates the error state.
 * @param filename     - The name of the file where the error occurred, or NULL for no filename.
 * @param line_number  - The line number in the file where the error occurred, or 0 for no line number.
 * @param error_state  - A pointer to an integer that tracks the error state.
 *                       The error state is set to 1 if an error message is printed.
 * @param count        - The number of variable arguments passed after the format string.
 * @param ...          - A variable number of format strings and corresponding arguments.
 *
 * Example usage:
 *   error_msg(<file_name>, <line_number>, &error_state, number_of_arguments,
 *              "Missing whitespace after \'", macro_name, "\'.");
 * Output:
 *   Error: file_name: Line line_number: Missing whitespace after 'macro_name'.
 */
void error_msg(char* filename, int line_number, int* error_state, int count, ...);

/**
 * Prints a warning message to standard output.
 * @param filename - The name of the file where the warning occurred, or NULL.
 * @param line_number - The line number in the file where the warning occurred, or 0.
 * @param count - The number of variable arguments passed after the format string.
 * @param ... - A variable number of format strings and corresponding arguments.
 */
void warning_msg(char* filename, int line_number, int count, ...);


#endif /* MMN14_UTILS_H */
