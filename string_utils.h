#ifndef _MMN14_STRING_UTILS_H
#define _MMN14_STRING_UTILS_H

#include "utils.h"

#define DELIMITER "\n "
#define ERROR_ "\x1b[31mERROR:\x1b[0m "
#define LINE_ "\x1b[34mLINE\x1b[0m  "
#define EMPTY_STRING ""
#define FILE_DETECT_ERROR "No file detected"

#define ALLOCATION_ERROR(file_name, line_number, where, error_state) \
    error_msg(file_name, line_number, "Failed to allocate memory " where, error_state)


#define ALLOCATION_ERROR_(file_name, line_number, arg, where, error_state) \
    char msg[MAX_ERROR_LENGTH];\
    snprintf(msg, MAX_ERROR_LENGTH, "Failed to allocate memory for %s%s", arg, where);\
    error_msg(file_name, line_number, msg, error_state)

#define MACRO_ERROR "Error: 2\n"
#define MACRO_NAME_ERROR "Macro name already in use"
#define INVALID_MACRO_NAME_ERROR "Invalid macro name. The macro name must contain at least one character"

/** @format: "<file_name>| LINE <line_number>: ERROR: Missing space <where>.\n"*/
#define MISSING_SPACE(file_name, line, where, error_state) \
    error_msg(file_name, line, "Missing space " where, error_state);


#define ILLEGAL_COMMA_ERROR(file_name, line_number, where ,error_state) \
    error_msg(file_name, line_number, "illegal comma " where , error_state);


#define MISSING_INSTRUCTION(file_name, line_number, error_state) \
    error_msg(file_name, line_number, "Missing instruction", error_state);

#define ILLEGAL_INSTRUCTION(file_name, line_number, not_instruction, error_state) \
        char msg[MAX_ERROR_LENGTH];\
        sprintf(msg, "Illegal instruction '%s'", not_instruction);\
        error_msg(file_name, line_number, msg, error_state);\

#define ILLEGAL_ARGUMENT(filename, line, arg, err) \
    error_msg(filename, line, arg, err);           \

#define MISSING_ARGUMENT(file_name, line_number, error_state) \
    error_msg(file_name, line_number, "Missing argument", error_state)


#define RESERVED_NAME_ERROR(file_name, line_number, type, name, error_state)\
    char msg[MAX_ERROR_LENGTH], modified_name[LINE_SIZE];\
    strcpy(modified_name, name);\
    if (modified_name[strlen(modified_name) - 1] == '\n') {\
        modified_name[strlen(modified_name) - 1] = '\0';\
    }\
    sprintf(msg, "%s '%s' is a reserved word", type, modified_name);\
    error_msg(file_name, line_number, msg, error_state);




#define MISSING_COMMA "Missing comma"
#define DELETE_FILE_ERROR "deleting file"

#define LABEL_NAME_ERROR(file_name, line_number, error_state) \
    char msg[LINE_SIZE]; \
    sprintf(msg, "Invalid label name: must contain only letters and numbers"); \
    error_msg(file_name, line_number, msg, error_state);

#define TOO_MANY_ARGUMENTS_ERROR(file_name, line, extra_arg, err_state) \
    char msg[MAX_ERROR_LENGTH]; \
    sprintf(msg, "Too many arguments: '%s' is not needed and should be removed", extra_arg); \
    error_msg(file_name, line, msg, error_state);

#define MACHINE_ERROR(file_name, line_number, error_state) \
    error_msg(file_name, line_number, "Machine has run out of memory", error_state)


void remove_comment(char *str);
void remove_whitespace(char *str);
int empty_line(char line[]);
int check_reserved_words(const char* str);
char *_strdup(const char *str);
void error_msg(const char *file_name, int line_number, const char *msg, int *error_state);
void check_reserved_word(const char *str, int line_number);

int is_register(const char *str);
int is_instruction(const char *str);

/**
 * Checks whether the given string is a reserved name in the assembly language.
 * A name is considered reserved if it matches one of the predefined register names
 * or directive names, such as ".data" or ".extern".
 *
 * @param str The string to check for reserved name status.
 *
 * @return Returns ERROR if the string is a reserved name, NO_ERROR otherwise.
 */
int is_reserved(const char *str);

int first_group_instructions(const char *instruction);
int second_group_instructions(const char *instruction);
int third_group_instructions(const char *instruction);



#endif /*_MMN14_STRING_UTILS_H */
