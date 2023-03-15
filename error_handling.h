#ifndef _MMN14_ERROR_HANDLING_H
#define _MMN14_ERROR_HANDLING_H

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
 * Checks if the .data directive is used correctly.
 * @param file The current file being checked.
 * @param error_state A pointer to an integer that holds the current error state.
 * @param line_number The current line number being checked.
 * @param line The current line being checked.
 */
void data_check(char *file, int *error_state, int line_number ,char *line);

/**
 * Checks if the .string directive is used correctly.
 * @param file The current file being checked.
 * @param line_number The current line number being checked.
 * @param error_state A pointer to an integer that holds the current error state.
 * @param line The current line being checked.
*/
void string_check(char *file, int line_number, int *error_state, char *line);


#endif /*_MMN14_ERROR_HANDLING_H */
