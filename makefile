# Set the name of the final program we will create
TARGET = assembler

# compiler to use
CC = gcc

# compiler flags
CFLAGS = -g -Wall -ansi -pedantic

# source files
SRCS = main.c utils.c structures.c assembler.c preassembler.c first_pass.c error_handling.c second_pass.c

# object files
OBJS = $(SRCS:.c=.o)

# header files
HEADERS = utils.h structures.h assembler.h preassembler.h first_pass.h error_handling.h second_pass.h

# command for deleting files
RM = rm -f

# default target
all: $(TARGET)

# building the program
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

# compiling c source files into object files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $*.o

clean:
	$(RM) $(OBJS) $(TARGET) *.am *.ob *.ent *.ext
	
gdb:
	gdb --args ./assembler $(a)
