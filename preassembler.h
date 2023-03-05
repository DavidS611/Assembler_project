#ifndef _MMN14_PREASSEMBLER_H
#define _MMN14_PREASSEMBLER_H

#include "utils.h"
#include "string_utils.h"

typedef struct mcr_table {
    char name[LINE_SIZE];
    char *data;
    struct mcr_table *next;
} macro;

void preassembler(FILE *fp, FILE *fp_am);

void read_new_macro(FILE *fp, macro **head, char macro_name[]);

macro* create_macro();

char *name_macro(char line[], char *name);

char *get_macro_data(macro *head, char name[]);

/* Free memory of all macros in list */
void free_macro(macro **head_ptr);

void macro_print(struct mcr_table *head);

#endif /*_MMN14_PREASSEMBLER_H */
