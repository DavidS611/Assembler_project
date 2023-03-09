# Project Title

Assembler for a custom CPU architecture with 8 registers and a 256-word RAM.

## Description

This project involves working with C source files (.c) and header files (.h), as well as compiled and linked executable files. A makefile is also included to automate the build process. Assembly inputs and outputs, as well as screenshots of any errors encountered, are required.

The hardware used for the project includes eight registers (r0-r7), each with a capacity of 14 bits, as well as a program status word (PSW) register that contains flags for the CPU. The RAM has 256 cells (0-255), with each cell also known as a "word" and containing 14 bits. Numbers are represented using 2's complement for negatives, while characters are represented by their ASCII code. Only integer values are allowed.

The project uses a word format consisting of the following fields:
param1, param2, opCode, src_addr, dst_addr,and ARE.

    --------------------------------------------------
    |13  12|11  10|9  8  7  6|  5  4  |  3  2  | 1 0 |
    --------------------------------------------------
    |param1|param2|  opCode  |src_addr|dst_addr| ARE |
    --------------------------------------------------

The values of each bit in the word have specific meanings:

Bits 0-1: represent E, R, and A.

A (Absolute) has a value of 00 and indicates that the content of the word is not dependent on where the machine code will be loaded. E (External) has a value of 01 and indicates that the content of the word depends on an external symbol. For example, a label defined in a different file. R (Relocatable) has a value of 10 and indicates that the content of the word is dependent on where the machine code will be loaded. For example, a label defined in the same file as the word. Bits 2-3: represent the destination operand's addressing mode:

00 represents an immediate number (e.g., #4) 01 represents a label (e.g., HELLO) 11 represents a register (e.g., r1) Bits 4-5: represent the source operand's addressing mode, and use the same codes as the destination operand.

Bits 6-9: represent the opcode, with 16 available operations (0-15).

Bits 10-13: represent the addressing modes for the parameters, and are relevant only for jmp/bne/jsr operations. These codes use the same addressing modes as the source and destination operands.

Addressing Mode
The assembler supports different addressing modes to allow for flexible operand specification. Each addressing mode has a code and a way of writing the operand.

Code: 0. Mode: Instant. Additional Words: Contains the number in 12 bits. Way of Writing: Operand starts with '#' followed by a number. Example: mov #15, r0.

Code: 1. Mode: Direct. Additional Words: Destination of a word in memory. Way of Writing: The operand is a label declared by .data, .string, or extern. Example: mov x, r1.

Code: 2. Mode: Relative. Additional Words: 3 Direct. Way of Writing: At most 3 words, the first is the label and the 2 others are the parameters. If both parameters are registers then they will be combined into 1 word. Example: jmp L1(r2,r3).

Code: 3. Mode: Register. Additional Words: None. Way of Writing: A register, if it’s a source then it will contain in bits 2-7 the register number, if it’s destination than it will be bits 8-13. Example: mov r1, r2.

For jmp, bne, and jsr operations, there are additional parameters addressing modes that are relevant. These addressing modes are the same as source and destination codes
