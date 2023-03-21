#ifndef MMN14_ERROR_HANDLING_H
#define MMN14_ERROR_HANDLING_H

#include "utils.h"

/**
 * Validates a label in a line of assembly code.
 * @param file - The name of the file being assembled.
 * @param error_state - A pointer to the error state integer variable.
 * @param line_number - The line number of the line being assembled.
 * @param line - The line of assembly code being assembled.
 */
void label_check(char *file, int *error_state, int line_number, char *line);

/**
 * Checks if the .data directive statement is used correctly.
 * @param file The current file being checked.
 * @param error_state A pointer to an integer that holds the current error state.
 * @param line_number The current line number being checked.
 * @param line The current line being checked.
 */
void data_check(char *file, int *error_state, int line_number ,char *line);

/**
 * Checks if the .string directive statement is used correctly.
 * @param file The current file being checked.
 * @param line_number The current line number being checked.
 * @param error_state A pointer to an integer that holds the current error state.
 * @param line The current line being checked.
*/
void string_check(char *file, int line_number, int *error_state, char *line);

/**
 * Checks if the .extern directive statement is used correctly.
 * @param file The current file being checked.
 * @param line_number The current line number being checked.
 * @param error_state A pointer to an integer that holds the current error state.
 * @param line The current line being checked.
 */
void extern_check(char *file, int line_number, int *error_state, char *line);

/**
 * Checks if the .entry directive statement is used correctly.
 * @param file The current file being checked.
 * @param line_number The current line number being checked.
 * @param error_state A pointer to an integer that holds the current error state.
 * @param st A pointer to the symbol table.
 * @param line The current line being checked.
*/
void entry_check(char *file, int line_number, int *error_state, symbol_table *st, char *line);

/**
 * Checks if the first group of instructions ("mov", "cmp", "add", "sub", "lea") statement is used correctly.
 * @param file The current file being checked.
 * @param line_number The current line number being checked.
 * @param error_state A pointer to an integer that holds the current error state.
 * @param st A pointer to the symbol table.
 * @param line The current line being checked.
 */
void first_group_check(char *file, int line_number, int *error_state, symbol_table *st, char *line);

/**
 * Checks if the second group of instructions ("not","clr","inc", "dec","jmp","bne", "red", "prn","jsr") statement is used correctly.
 * @param file - The current file being checked.
 * @param line_number - The current line number being checked.
 * @param error_state - A pointer to an integer that holds the current error state.
 * @param st - A pointer to the symbol table.
 * @param line - The current line being checked.
 */
void second_group_check(char *file, int line_number, int *error_state, symbol_table *st, char *line);

/**
 * Checks the syntax of a line of assembly code and reports any errors found.
 * @param file - the name of the source file being checked
 * @param line - the line of code being checked
 * @param error_state - a pointer to an integer that stores the current error state
 * @param line_number - the line number of the code being checked
 * @param st - a pointer to the symbol table
 */
void syntax_errors(char *file, char *line, int *error_state, int line_number, symbol_table *st);


#endif /* MMN14_ERROR_HANDLING_H */
