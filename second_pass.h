
#ifndef MMN14_SECOND_PASS_H
#define MMN14_SECOND_PASS_H

#include "utils.h"

#define BITS_PER_WORD 14

#define ABSOLUTE (0x0)
#define EXTERNAL (0x1)
#define RELOCATABLE (0x2)

#define DIRECT_ADDR (0x1)
#define JMP_ADDR (0x2)
#define REG_ADDR (0x3)

/* Instruction and directives binary code without shifting */
    #define MOV 0x0
    #define CMP 0x1
    #define ADD 0x2
    #define SUB 0x3
    #define NOT 0x4
    #define CLR 0x5
    #define LEA 0x6
    #define INC 0x7
    #define DEC 0x8
    #define JMP 0x9
    #define BNE 0xa
    #define RED 0xb
    #define PRN 0xc
    #define JSR 0xd
    #define RTS 0xe
    #define STOP 0xf

/* Registers binary code without shifting*/
    #define R0 (0x0)
    #define R1 (0x1)
    #define R2 (0x2)
    #define R3 (0x3)
    #define R4 (0x4)
    #define R5 (0x5)
    #define R6 (0x6)
    #define R7 (0x7)

#define PARAM_1_SIZE_SHIFT 12
#define PARAM_2_SIZE_SHIFT 10
#define OPCODE_SIZE_SHIFT 6
#define SRC_OPERAND_SIZE_SHIFT 4
#define DEST_OPERAND_SIZE_SHIFT 2
#define ARE_SHIFT 2
#define R6_SHIFT 8
#define R7_SHIFT 2

/**
 * Performs the second pass of the assembly process to generate the object file (extern and entry files if needed) for the given assembly program.
 * @param filename - The name of the assembly file.
 * @param st - A pointer to the symbol table of the program.
 * @param fp_am - A pointer to an assembly file.
 * @param ic - The instruction counter indicating the current position in the instruction memory.
 * @param dc - The data counter indicating the current position in the data memory.
*/
void second_pass(char *filename, symbol_table *st, FILE *fp_am, int ic, int dc);

/**
 * Generates the object file for the given assembly program.
 * @param filename - The name of the assembly file.
 * @param map - A hash map containing the memory map of the program.
 * @param ic - The instruction counter.
 * @param dc - The data counter.
 */
void object_file_generate(char *filename, hash_map map, int ic, int dc);

/**
 * Generates an entry symbols file for the given assembly program.
 * @param filename - The name of the assembly file.
 * @param st - A pointer to the symbol table of the program.
 * @param fp_am -  A pointer to an assembly file.
 */
void entry_file_generate(char *filename, symbol_table *st, FILE *fp_am);

/**
 * Generates an external symbols file for the given assembly program.
 * @param filename - The name of the assembly file.
 * @param st - A pointer to the symbol table of the program.
 * @param map - A hash map containing the memory map of the program.
 * @param map_counter - The number of entries in the memory map.
 */
void extern_file_generate(char *filename, symbol_table *st, hash_map map, int map_counter);

/**
 * Converts a binary address to a special representation of dots and slashes.
 * @param binary_address - The binary address to convert.
 * @return Returns a string containing the special representation of the binary address.
 */
char *binary_to_special(word binary_address);

/**
 * Creates a new empty hash map, where all nodes have 0 decimal and binary addresses.
 * @return Returns the created hash map.
 */
hash_map create_hash_map();

/**
 * Computes the hash value for the given decimal address.
 * @param decimal_address - The decimal address to calculate the hash value for.
 * @return Returns the computed hash value.
 */
int hash_function(int decimal_address);

/**
 * Inserts a new hash node to the hash map.
 * @param map - Pointer to the hash map.
 * @param decimal_address - The decimal address of the code.
 * @param code - The code to be inserted.
 * @param binary_address - The binary address of the code.
*/
void insert(struct hash_map *map, int decimal_address, char *code, word binary_address);

/**
 * Retrieves the binary code for a given decimal address from a hash map.
 * @param map - Pointer to the hash map to retrieve the binary code from.
 * @param decimal_address - The decimal address to retrieve the binary code for.
 * @return Returns the binary code (word) for a given decimal address.
 */
word get_binary_code_for_decimal_address(struct hash_map *map, int decimal_address);

/**
 * Prints all addresses in the given hash map.
 * @param map - Pointer to the hash map to print its addresses.
 * @param map_counter - The size of the hash map.
 */
void print_all_addresses(hash_map *map, int map_counter);

/**
 * Converts a label name to its corresponding address method word.
 * @param st - Pointer to the symbol table containing the label.
 * @param label_name - The name of the label to convert.
 * @return Returns the word value of the label's address method.
 */
word label_to_word(symbol_table *st, char *label_name);

/**
 * Determines the type of address method used for the given string.
 * @param st - Pointer to the symbol table to check for label existence.
 * @param str - The string to check for its address method type.
 * @return Returns the corresponding word value of the address method type for the given string.
 */
word num_label_register(symbol_table *st, char *str);

/**
 * Converts a string representing a number to its corresponding value in the word format.
 * @param num - The string to convert, which may have a leading "#" character.
 * @return Returns the value of the number in the word format.
 */
word num_to_word(char *num);

/**
 * Converts a character to its corresponding value in the word format.
 * @param c - The character to convert.
 * @return Returns the value of the character in the word format.
 */
word char_to_word(char c);

/**
 * Calculates the machine code word for a given instruction and operands.
 * @param st - A pointer to the symbol table.
 * @param instruction - An instruction string.
 * @param source_operand - A source operand string, or NULL if not exists.
 * @param dest_operand - A destination operand string, or NULL if not exists.
 * @param parameter1 - A first parameter string, or NULL if not second addressing method.
 * @param parameter2 -  A second parameter string, or NULL if not second addressing method.
 * @return Returns the machine code word for the given instruction and operands.
 */
word bits_calculator(symbol_table *st, char *instruction, char *source_operand, char *dest_operand, char *parameter1, char *parameter2);

/**
 * Maps a register name to its corresponding value in the word format.
 * @param _register - The name of the register to map.
 * @return Returns the value of the register in the word format, or 0 if the register name is invalid.
 */
word register_to_word(char *_register);

/**
 * Prints the binary representation of a given word.
 * @param w - The word to print the binary representation of.
 */
void print_word_bits(word w);

/**
 * Converts a string representation of an instruction to its corresponding word value.
 * @param instruction - An instruction string to convert.
 * @return Returns the corresponding word value for the instruction, or STOP if the instruction is invalid.
 */
word instruction_to_word(char *instruction);

#endif /* MMN14_SECOND_PASS_H */
