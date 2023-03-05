
#include "string_utils.h"

void remove_comment(char *str){
    char *end;
    /* Remove comment, if any */
    end = strchr(str, ';');
    if (end != NULL) {
        *end = '\0';
        /* Add newline character */
        strcat(str, "\n");
    }
}

void remove_whitespace(char *str) {
    char short_line[LINE_SIZE];
    int i = 0, j;

    if(str!=NULL) { /* check that the function doesn't try to operate on a null pointer*/
        /* Remove leading whitespace */
        while (isspace(str[i])) {
            i++;
        }

        /* Remove trailing whitespace */
        j = strlen(str) - 1;
        while (j >= i && isspace(str[j])) {
            j--;
        }
        str[j + 1] = '\0';

        /* Copy the modified string to a new buffer */
        strcpy(short_line, &str[i]);

        /* Add a newline character */
        strcat(short_line, "\n");

        /* Copy the modified string back to the original */
        strcpy(str, short_line);
    }
}

int empty_line(char line[]){
    int i=0;

    for(; isspace(line[i]); i++);
    if(line[i]==';' || line[i] == '\n' || line[i]=='\0') /*  */
        return 1;
    return 0;
}

/* assumed: str doesn't have leading and trailing whitespaces */
int is_register(const char *str){
    const char *register_names[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};
    int i, num_registers = sizeof(register_names) / sizeof(register_names[0]);
    int str_len = (strchr(str, '\n') ? strlen(str) - 1 : strlen(str));

    for (i = 0; i < num_registers; i++) {
        if (strncmp(str, register_names[i], str_len) == 0) {
            return ERROR; /* str is a reserved register name */
        }
    }
    return NO_ERROR;
}

/* assumed: str doesn't have leading and trailing whitespaces a part from newline at the end */
/* @return 1 if it is reserved instruction name, and 0 if it is not */
int is_instruction(const char *str){
    if(first_group_instructions(str) || second_group_instructions(str) || third_group_instructions(str))
        return ERROR;
    return NO_ERROR;
}

int is_reserved(const char *str){
    const char *reserved_names[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7",
                                    "mov", "cmp", "add", "sub", "not", "clr", "lea",
                                    "inc", "dec", "jmp", "bne", "red", "prn","jsr",
                                    "rts", "stop", ".data", "data", ".string", "string",
                                    ".extern", "extern", ".entry", "entry","mcr", "endmcr"};
    int i, str_len, num_names = sizeof(reserved_names) / sizeof(reserved_names[0]);
    if(str==NULL) /* A null pointer was passed in */
        return NO_ERROR;
    str_len = (strchr(str, '\n') ? strlen(str) - 1 : strlen(str));

    for (i = 0; i < num_names; i++) {
        if (strncmp(str, reserved_names[i], str_len) == 0 && reserved_names[i][str_len]=='\0') {
            return ERROR; /* str is a reserved register name */
        }
    }
    return NO_ERROR;
}

char *_strdup(const char *str){
    char *p;

    p = malloc(strlen(str) + 1); /* adding 1 for '\0'*/
    if (p==NULL){
        ALLOCATION_ERROR_(NULL, NO_ARGUMENT, str, "", NO_ARGUMENT);
        exit(EXIT_FAILURE);
    }
    strcpy(p, str);
    return p;
}


void error_msg(const char *file_name, int line_number, const char *msg, int *error_state){
    if(line_number != 0 && file_name == NULL)
        fprintf(stdout, LINE_ "%d: " ERROR_ "%s.\n", line_number, msg);
    else if(line_number==0 && file_name == NULL)
        fprintf(stdout, ERROR_ "%s.\n", msg);
    else if(line_number == 0 && file_name != NULL)
        fprintf(stdout,  "'%s.as'| "ERROR_ "%s.\n", file_name, msg);
    else
        fprintf(stdout, "'%s.as'| " LINE_ "%d: " ERROR_ "%s.\n", file_name, line_number, msg);
    if(error_state != NO_ERROR)
        *error_state = ERROR;
}

int third_group_instructions(const char *instruction){
    const char *instruction_names[] = {"rts", "stop"};
    int i, num_instructions = sizeof(instruction_names) / sizeof(instruction_names[0]);
    int instructions_len = (strchr(instruction, '\n') ? strlen(instruction) - 1 : strlen(instruction));

    for (i = 0; i < num_instructions; i++) {
        if (strncmp(instruction, instruction_names[i], instructions_len) == 0) {
            return ERROR; /* instruction is a reserved instruction name */
        }
    }
    return NO_ERROR;
}

int second_group_instructions(const char *instruction){
    const char *instruction_names[] = {"not","clr","inc", "dec",
                                       "jmp","bne", "red", "prn","jsr"};
    int i, num_instructions = sizeof(instruction_names) / sizeof(instruction_names[0]);
    int instructions_len = (strchr(instruction, '\n') ? strlen(instruction) - 1 : strlen(instruction));

    for (i = 0; i < num_instructions; i++) {
        if (strncmp(instruction, instruction_names[i], instructions_len) == 0) {
            return ERROR; /* instruction is a reserved instruction name */
        }
    }
    return NO_ERROR;
}

int first_group_instructions(const char *instruction){
    const char *instruction_names[] = {"mov", "cmp", "add", "sub", "lea"};
    int i, num_instructions = sizeof(instruction_names) / sizeof(instruction_names[0]);
    int instructions_len = (strchr(instruction, '\n') ? strlen(instruction) - 1 : strlen(instruction));

    for (i = 0; i < num_instructions; i++) {
        if (strncmp(instruction, instruction_names[i], instructions_len) == 0) {
            return ERROR; /* instruction is a reserved instruction name */
        }
    }
    return NO_ERROR;
}