
#ifndef MMN14_ASSEMBLER_H
#define MMN14_ASSEMBLER_H

#include "preassembler.h"
#include "first_pass.h"

/**
 * Assembles one or more source files into machine code.
 * @param number_of_arg - The number of arguments passed to the program.
 * @param file - An array of strings that contains the filenames of the source files to be assembled.
 */
void assembler(int number_of_arg, char *file[]);

#endif /* MMN14_ASSEMBLER_H*/
