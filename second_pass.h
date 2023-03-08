
#ifndef _MMN14_SECOND_PASS_H
#define _MMN14_SECOND_PASS_H

#include "utils.h"
#include "first_pass.h"

#define BITS_PER_WORD 14

#define ABSOLUTE (0x0)
#define EXTERNAL (0x1)
#define RELOCATABLE (0x2)

#define REG_ADDR (0x3)
#define LABEL_ADDR (0x2)


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


void second_pass(symbol_table *st, FILE *fp_am);

hash_map create_hash_map();

int hash_function(int decimal_address);

void insert(struct hash_map *map, int decimal_address, unsigned short int binary_address);

unsigned short int get_binary_code_for_decimal_address(struct hash_map *map, int decimal_address);

void print_all_addresses(hash_map *map, int map_counter);

word *set_label_bits(symbol_table *st, char *label_name);

word label_to_word(symbol_table *st, char *label_name);

word num_label_register(symbol_table *st, char *str);


word num_to_word(char *num);

word char_to_word(char c);

word bits_calculator(symbol_table *st, char *instruction, char *source_operand, char *dest_operand, char *parameter1, char *parameter2);

word register_to_word(char *_register);

void print_word_bits(word w);

word instruction_to_word(char *instruction);

#endif /*_MMN14_SECOND_PASS_H */
