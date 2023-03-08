#ifndef MMN14_STRING_UTILS_H
#define MMN14_STRING_UTILS_H

#include "utils.h"

#define ERROR_ "\x1b[31mERROR:\x1b[0m "
#define WARNING "\x1b[35mWARNING:\x1b[0m "
#define LINE_ "\x1b[34mLINE\x1b[0m  "
#define BLUE "\033[1;34m"
#define WHITE "\033[0m"
#define DELIMITER "\n \t"
#define COMMA ","

/**
 * Checks if a line of text is empty or comment or contains only whitespaces.
 *
 * @param line - The line of text to check.
 * @return Returns 1 if the line is empty, otherwise returns 0.
 */
int empty_line(char line[]);

/**
 * Removes comments from a string, if there are.
 *
 * @param str - The string to remove the comment from.
 */
void remove_comment(char *str);

/**
 * Trim whitespace characters from the beginning and end of a string.
 *
 * @param str - The string to be trimmed.
 */
void trim_whitespace(char *str);

/**
 * Duplicates a string into dynamically allocated memory.
 *
 * @param filename - The name of the file where the function is called from.
 * @param str - The string to be duplicated.
 * @return Returns a pointer to the duplicated string on success, or NULL on failure with an error massage.
 *
 * @note The caller is responsible for freeing the returned memory.
 */
char *_strdup(char *filename, const char *str);

/**
 * Checks whether a given string is a reserved register name.
 *
 * @param str - The string to check.
 * @return Returns ERROR if the string is a reserved register name, NO_ERROR otherwise.
 */
int is_register(const char *str);

/**
 * Checks whether the given instruction belongs to the first group of instructions.
 * The first group includes "mov", "cmp", "add", "sub", and "lea".
 *
 * @param instruction - The instruction to check.
 * @return Returns true if the instruction belongs to the first group, 0 otherwise.
 */
int first_group_instructions(const char *instruction);

/**
 * Checks if the given instruction belongs to the second group of instructions.
 * The second group consists of the instructions: "not", "clr", "inc", "dec", "jmp", "bne", "red", "prn", "jsr".
 *
 * @param instruction - The instruction to check.
 * @return - Returns 0 if the instruction does not belong to the second group,
 *                       otherwise true if the instruction is a reserved instruction name.
 */
int second_group_instructions(const char *instruction);

/**
 * Checks if the given instruction is a third group instruction (rts or stop).
 *
 * @param instruction - The instruction to check.
 * @return - Returns 0 if the instruction is not a third group instruction, or true if it is.
 */
int third_group_instructions(const char *instruction);

/**
 * Determines if a given string is a valid assembly instruction.
 *
 * @param str - The string to check.
 * @return - Returns NO_ERROR if the string is a valid instruction, or ERROR otherwise.
 */
int is_instruction(const char *str);

int is_directive(const char *str);

/**
 * Checks whether the given string is a reserved name in the assembler.
 *
 * @param str - The string to check for reserved name status.
 * @return Returns - ERROR (1) if the string is a reserved name, NO_ERROR (0) otherwise.
 */
int is_reserved(const char *str);


/**
 * Prints an error message to standard output and updates the error state.
 * The error message consists of an optional filename, an optional line number,
 * and one or more format strings that are concatenated and printed to output.
 *
 * @param filename     - The name of the file where the error occurred, or NULL.
 * @param line_number  - The line number in the file where the error occurred, or 0.
 * @param error_state  - A pointer to an integer that tracks the error state.
 *                       The error state is set to 1 if an error message is printed.
 * @param count        - The number of variable arguments passed after the format string.
 * @param ...          - A variable number of format strings and corresponding arguments.
 *
 * Example usage:
 *   error_msg(<file_name>, <line_number>, &error_state, number_of_parameters,
 *              "Missing whitespace after \"", macro_name, "\".");
 * Output:
 *   Error: file_name: line line_number: Missing whitespace after "macro_name".
 */
void error_msg(char* filename, int line_number, int* error_state, int count, ...);

/**
 * Prints a warning message to standard output.
 * The warning message consists of an optional filename, an optional line number,
 * and one or more format strings that are concatenated and printed to output.
 *
 * @param filename - The name of the file where the warning occurred, or NULL.
 * @param line_number - The line number in the file where the warning occurred, or 0.
 * @param count - The number of variable arguments passed after the format string.
 * @param ... - A variable number of format strings and corresponding arguments.
 */
void warning_msg(char* filename, int line_number, int count, ...);

#endif /*MMN14_STRING_UTILS_H */
