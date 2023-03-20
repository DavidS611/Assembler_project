#include "structures.h"

/* Macro setters */
void set_macro_name(macro *mc, char *name) {
    mc->name = name;
}
void set_macro_data(macro *mc, char *data) {
    mc->data = data;
}
void set_macro_next(macro *mc, macro *next) {
    mc->next = next;
}
/* Macro getters */
char *get_macro_name(macro *mc) {
    return mc->name;
}
char *get_macro_data(macro *mc) {
    return mc->data;
}
macro *get_macro_next(macro *mc) {
    return mc->next;
}

/* Symbol entry setters */
void set_label(symbol_entry* entry, char* label) {
    entry->label = label;
}
void set_address(symbol_entry* entry, int address) {
    entry->address = address;
}
void set_is_code(symbol_entry* entry, bool is_code) {
    entry->is_code = is_code;
}
void set_is_data(symbol_entry* entry, bool is_data) {
    entry->is_data = is_data;
}
void set_is_extern(symbol_entry* entry, bool is_extern) {
    entry->is_extern = is_extern;
}
void set_is_entry(symbol_entry* entry, bool is_entry) {
    entry->is_entry = is_entry;
}
void set_next(symbol_entry* entry, struct symbol_entry* next) {
    entry->next = next;
}
/* Symbol entry getters */
char* get_label(symbol_entry* entry) {
    return entry->label;
}
int get_address(symbol_entry* entry) {
    return entry->address;
}
bool get_is_code(symbol_entry* entry) {
    return entry->is_code;
}
bool get_is_data(symbol_entry* entry) {
    return entry->is_data;
}
bool get_is_extern(symbol_entry* entry) {
    return entry->is_extern;
}
bool get_is_entry(symbol_entry* entry) {
    return entry->is_entry;
}
struct symbol_entry* get_next(symbol_entry* entry) {
    return entry->next;
}

/* Symbol table setters */
void set_head(symbol_table* table, symbol_entry* head) {
    table->head = head;
}
/* Symbol table getters */
symbol_entry* get_head(symbol_table* table) {
    return table->head;
}

/* Hash node setters */
void set_decimal_address(hash_node* node, int decimal_address) {
    node->decimal_address = decimal_address;
}
void set_code(hash_node* node, char* code) {
    strcpy(node->code, code);
}
void set_binary_address(hash_node* node, word binary_address) {
    node->binary_address = binary_address;
}
void set_hash_node(hash_map* map, int index, hash_node* node) {
    map->nodes[index] = *node;
}
/* Hash node getters */
int get_decimal_address(hash_node* node) {
    return node->decimal_address;
}
char* get_code(hash_node* node) {
    return node->code;
}
word get_binary_address(hash_node* node) {
    return node->binary_address;
}
hash_node* get_hash_node(hash_map* map, int index) {
    return &(map->nodes[index]);
}
