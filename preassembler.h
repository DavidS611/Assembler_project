#ifndef MMN14_PREASSEMBLER_H
#define MMN14_PREASSEMBLER_H

#include "utils.h"

/**
 * This function performs preassembling on the input assembly file.
 * @param as_filename - The name of the assembly file to preassemble.
 * @param fp - A pointer to the input assembly file.
 * @param fp_am - A pointer to the output preassembled file.
 */
void preassembler(char *as_filename, FILE *fp, FILE *fp_am);

/**
 * This function reads in a new macro definition from the input assembly file ,
 * and adds it to a linked list of macro structs with there data.
 * @param as_filename - The name of the assembly file currently being processed.
 * @param fp - A pointer to the input assembly file.
 * @param head - A pointer to the head of the linked list of macros.
 * @param macro_name - The name of the macro being defined.
 */
void read_new_macro(char *as_filename, FILE *fp, macro **head, char macro_name[]);

/**
 * Creates a new macro node and allocates memory for it.
 * @param as_filename - The name of the assembly file currently being processed.
 * @return - Returns a pointer to the new macro node if memory allocation succeeds, NULL otherwise.
 */
macro* create_macro(char *as_filename);

/**
 * This function retrieves the macro data for a given macro name from a linked list of macro structs.
 * @param head - A pointer to the head of the linked list of macros.
 * @param name - The name of the macro to retrieve data for.
 * @return - Returns a pointer to the macro data if found, NULL otherwise.
 */
char *get_macros_data(macro *head, char name[]);

/**
 * Frees the memory allocated for a linked list of macro structs.
 * @param head_ptr - A pointer to the head of the linked list.
 */
void free_macro(macro **head_ptr);

/**
 * This function prints out the all macro name with its data.
 * @param head - A pointer to the head of the linked list of macro nodes.
 */
void macro_print(struct mcr_node *head);

/**
 * Checks if a given macro name exists in a linked list of macro structs.
 * @param mcr_name - The name of the macro to search for.
 * @param head - A pointer to the head of the linked list.
 * @return - Returns true if the macro name is found in the linked list, otherwise false.
 */
bool check_macro_name(char *mcr_name, macro *head);

#endif /*MMN14_PREASSEMBLER_H */
