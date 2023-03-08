
#ifndef MMN14_FIRST_PASS_H
#define MMN14_FIRST_PASS_H

#include "utils.h"
#include "string_utils.h"
#include "second_pass.h"

/*enum type{Absolute, External, Relocatable};*/

word word_generator(int count, ...);

void second_pass(symbol_table *st, FILE *fp_am);

symbol_table *first_pass(char *file_name, FILE *fp_am);

void define_entry_and_extern(char *file_name, int error_state, FILE *fp_am, symbol_table *st);

void error_handling(char *file_name, char *line, int *error_state, int line_counter, symbol_table *st);

int increasement_line(char *line);

int is_label(char *str, symbol_table *st);

symbol_entry *get_label(char *str, symbol_table *st);

void insert_label(char *file_name, int *error_state, int line_number, symbol_table *st, char *name, int *instruction_counter, int *data_counter);

void print_symbol_table(symbol_table* st);

int data_counter(char *line);

int instruction_counter(char *line);

void correcting_data(symbol_table *st, int ic);

void free_symbol_table(symbol_table *st);

#endif /*MMN14_FIRST_PASS_H*/