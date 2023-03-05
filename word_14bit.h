
#ifndef _MMN14_WORD_14BIT_H
#define _MMN14_WORD_14BIT_H

#include "utils.h"
#define MASK 0b1

typedef struct word {
    unsigned short address : 14;
} word;


void print_word(word w);

word num_to_word(char *num_char);

#endif /*_MMN14_WORD_14BIT_H*/
