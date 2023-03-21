#ifndef MMN14_FIRST_PASS_H
#define MMN14_FIRST_PASS_H

#include "error_handling.h"
#include "second_pass.h"

/**
 * Performs the first pass of a two-pass assembly process on the specified file.
 * @param file_name - The name of the file being processed.
 * @param fp_am - A pointer to the expand macro file.
 */
void first_pass(char *filename, FILE *fp_am);

/**
 * Defines entry labels in the symbol table and checks for invalid labels defined as both entry and extern.
 * @param file_name - The name of the file being processed.
 * @param error_state - The current error state of the program.
 * @param fp_am - A pointer to the expand macro file.
 * @param st - A pointer to the symbol table being used.
 */
void define_entry_labels(char *filename, int error_state, FILE *fp_am, symbol_table *st);

/**
 * Inserts a new label into the symbol table.
 * @param file_name - the name of the source file.
 * @param error_state - pointer to the error state.
 * @param line_number - the number of the line in the source file.
 * @param st - pointer to the symbol table.
 * @param line - the current line from the source file.
 * @param instruction_counter - pointer to the instruction counter.
 * @param data_counter - pointer to the data counter.
 */
void insert_label(char *file_name, int *error_state, int line_number, symbol_table *st, char *line, int *instruction_counter, int *data_counter);

/**
 * Prints the contents of the symbol table.
 * @param st - pointer to the symbol table.
 */
void print_symbol_table(symbol_table* st);

/**
 * Calculates the number of elements in a .data or .string directive.
 * @param line - the line containing the .data or .string directive.
 * @return the number of elements in the .data or .string directive.
 */
int data_counter(char *line);

/**
 * Calculates the number of memory words required to store an instruction line.
 * @param line - The instruction line.
 * @return The number of memory words required to store the instruction line.
 */
int instruction_counter(char *line);

/**
 * Updates the address of each symbol entry in a symbol table that contains data, by adding the instruction counter to their current address.
 * @param st - A pointer to a symbol table to update.
 * @param ic - The instruction counter value.
 */
void correcting_data(symbol_table *st, int ic);

/**
 * Frees the memory allocated for a symbol table and all its entries.
 * @param table the symbol table to free
 */
void free_symbol_table(symbol_table *st);

#endif /*MMN14_FIRST_PASS_H*/