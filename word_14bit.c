
#include "word_14bit.h"


/* prints the binary representation of a word's address */
void print_word(word w) {
    int i;
    for (i = 13; i >= 0; i--) {
        printf("%d", (w.address >> i) & 1);
    }
    printf("\n");
}

word num_to_word(char *num_char) {
    int num = atoi(num_char);  // Convert string to integer
    word w;
    if (num_char[0] == '-') { /* negative number */
        int abs_value = abs(num);
        int complement = ~abs_value + 1;
        w.address = (complement & MASK) | 0x2000;  // Set sign bit
    } else {
        // Convert positive value to binary
        w.address = num & MASK;
    }
    return w;
}