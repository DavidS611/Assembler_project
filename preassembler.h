#ifndef MMN14_PREASSEMBLER_H
#define MMN14_PREASSEMBLER_H

#include "utils.h"
#include "string_utils.h"

typedef struct mcr_table {
    char *name;
    char *data;
    struct mcr_table *next;
} macro;

void preassembler(char *as_filename, FILE *fp, FILE *fp_am);

void read_new_macro(char *as_filename, FILE *fp, macro **head, char macro_name[]);

macro* create_macro(char *as_filename);

char *get_macro_data(macro *head, char name[]);

/* Free memory of all macros in list */
void free_macro(macro **head_ptr);

void macro_print(struct mcr_table *head);

int check_macro_name(char *mcr_name, macro *head);

#endif /*MMN14_PREASSEMBLER_H */
