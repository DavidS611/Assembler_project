#ifndef MMN14_STRUCTURES_H
#define MMN14_STRUCTURES_H

#include <string.h>

#define LABEL_SIZE 31 /* Maximum label name length with null terminator */
#define HASH_MAP_SIZE 256  /* Maximum 256 words in the system */

typedef enum boolean {TRUE=0, false = 0, true = 1, FALSE=1} bool;

typedef unsigned short word;

/* Represents a macro node
 *
 * name - The name of the macro
 * data - The data related to the macro's name
 * next - Pointer to the next macro node in the linked list */
typedef struct mcr_node {
    char *name;
    char *data;
    struct mcr_node *next;
} macro;
/** Macro setters */
/* Set the name of the macro */
void set_macro_name(macro *mc, char *name);
/* Set the data of the macro */
void set_macro_data(macro *mc, char *data);
/* Set the next node of the macro */
void set_macro_next(macro *mc, macro *next);
/** Macro getters */
/* Get the name of the macro */
char *get_macro_name(macro *mc);
/* Get the data of the macro */
char *get_macro_data(macro *mc);
/* Get the next node of the macro */
macro *get_macro_next(macro *mc);

/*
 * Represents a symbol table entry.
 *
 * label - the symbol's name.
 * address - the symbol's decimal address in memory.
 * is_code - true if the symbol is defined in the code section, false otherwise.
 * is_data - true if the symbol is defined in the data section, false otherwise.
 * is_extern - true if the symbol is external, false otherwise.
 * is_entry - true if the symbol is an entry point, false otherwise.
 * next - a pointer to the next entry in the symbol table.
 */
typedef struct symbol_entry {
    char* label;
    int address;
    bool is_code;
    bool is_data;
    bool is_extern;
    bool is_entry;
    struct symbol_entry* next;
} symbol_entry;

/** Symbol entry setters */
/* Sets the label of the symbol_entry to the specified value. */
void set_label(symbol_entry* entry, char* label);
/* Sets the address of the symbol_entry to the specified value. */
void set_address(symbol_entry* entry, int address);
/* Sets the is_code flag of the symbol_entry to the specified value. */
void set_is_code(symbol_entry* entry, bool is_code);
/* Sets the is_data flag of the symbol_entry to the specified value. */
void set_is_data(symbol_entry* entry, bool is_data);
/* Sets the is_extern flag of the symbol_entry to the specified value. */
void set_is_extern(symbol_entry* entry, bool is_extern);
/* Sets the is_entry flag of the symbol_entry to the specified value. */
void set_is_entry(symbol_entry* entry, bool is_entry);
/* Sets the next pointer of the symbol_entry to the specified value. */
void set_next(symbol_entry* entry, struct symbol_entry* next);
/** Symbol entry getters */
/* Returns the label of the symbol_entry. */
char* get_label(symbol_entry* entry);
/* Returns the address of the symbol_entry. */
int get_address(symbol_entry* entry);
/* Returns true if the symbol_entry is defined in the code section, false otherwise. */
bool get_is_code(symbol_entry* entry);
/* Returns true if the symbol_entry is defined in the data section, false otherwise. */
bool get_is_data(symbol_entry* entry);
/* Returns true if the symbol_entry is external, false otherwise. */
bool get_is_extern(symbol_entry* entry);
/* Returns true if the symbol_entry is an entry point, false otherwise. */
bool get_is_entry(symbol_entry* entry);
/* Returns a pointer to the next symbol_entry in the symbol table. */
struct symbol_entry* get_next(symbol_entry* entry);


/* Symbol table struct */
typedef struct symbol_table {
    symbol_entry* head;
} symbol_table;

/* Symbol table setters */
void set_head(symbol_table* table, symbol_entry* head);
/* Symbol table getters */
symbol_entry* get_head(symbol_table* table);

/* Represents a node in the hash map
 *
 * decimal_address - The decimal address of a hash node
 * code - The code of a hash node
 * binary address - The binary address of a hash node */
typedef struct hash_node {
    int decimal_address;
    char code[LABEL_SIZE];
    word binary_address;
} hash_node;
/* Represents a hash map. */
typedef struct hash_map {
    hash_node nodes[HASH_MAP_SIZE];
} hash_map;

/** Hash node setters */
/* Sets the decimal address of the hash node */
void set_decimal_address(hash_node* node, int decimal_address);
/* Sets the code of the hash node */
void set_code(hash_node* node, char* code);
/* Sets the binary address of the hash node */
void set_binary_address(hash_node* node, word binary_address);
/* Sets the hash node at the specified index in the hash map */
void set_hash_node(hash_map* map, int index, hash_node* node);
/** Hash node getters */
/* Returns the decimal address of the hash node */
int get_decimal_address(hash_node* node);
/* Returns the code of the hash node. */
char* get_code(hash_node* node);
/* Returns the binary address of the hash node */
word get_binary_address(hash_node* node);
/* Returns a pointer to the hash node at the specified index in the hash map. */
hash_node* get_hash_node(hash_map* map, int index);

#endif /* MMN14_STRUCTURES_H */