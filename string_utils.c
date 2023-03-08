
#include "string_utils.h"


int empty_line(char line[]){
    int i=0;

    /* Skip leading whitespaces */
    for(; isspace(line[i]); i++);
    /* Check if the line is a comment or empty */
    if(line[i]==';' || line[i] == '\n' || line[i]=='\0')
        return 1;
    return 0;

}

void remove_comment(char *str){
    char *end;
    /* Find the first occurrence of the ';' character */
    end = strchr(str, ';');

    /* If there is a comment, terminate the string at the ';' character */
    if (end != NULL) {
        *end = '\0';
        /* Add newline character to the end of the string */
        strcat(str, "\n");
    }
}

void trim_whitespace(char *str) {
    int len, i;

    /* Check for null pointer */
    if (str == NULL)
        return;

    /* Remove leading whitespace */
    for(i=0; isspace(str[i]); i++);
    memmove(str, str + i, strlen(str + i) + 1); /* Add 1 to copy the null terminator */

    /* Remove trailing whitespace */
    len = strlen(str);
    while (len > 0 && isspace(str[len-1])) {
        len--;
    }
    str[len] = '\0';
}

char * strtok_trimmed(char *str, const char *delim) {
    char *token = strtok(str, delim);
    trim_whitespace(token);
    return token;
}


char *_strdup(char *filename, const char *str){
    char *p;

    if(str == NULL){
        return NULL;
    }

    p = malloc(strlen(str) + 1); /* adding 1 for '\0'*/
    if (p==NULL){
        error_msg(filename, NO_ARGUMENT, NO_ARGUMENT,
                  3, "Failed to allocate memory for \"", str, "\".");
        exit(EXIT_FAILURE);
    }
    strcpy(p, str);
    return p;
}

/* assumed: str doesn't have leading and trailing whitespaces */
int is_register(const char *str){
    const char *register_names[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};
    int i, num_registers = sizeof(register_names) / sizeof(register_names[0]);
    int str_len;

    if(str==NULL) {
        return NO_ERROR;
    }
    str_len = (strchr(str, '\n') ? strlen(str) - 1 : strlen(str));

    for (i = 0; i < num_registers; i++) {
        if (strncmp(str, register_names[i], str_len) == 0) {
            return ERROR; /* str is a reserved register name */
        }
    }
    return NO_ERROR;
}

int first_group_instructions(const char *instruction){
    const char *instruction_names[] = {"mov", "cmp", "add", "sub", "lea"};
    int i, num_instructions = sizeof(instruction_names) / sizeof(instruction_names[0]);
    int instructions_len;

    if(instruction==NULL){
        return NO_ERROR;
    }

    instructions_len = (strchr(instruction, '\n') ? strlen(instruction) - 1 : strlen(instruction));

    for (i = 0; i < num_instructions; i++) {
        if (strncmp(instruction, instruction_names[i], instructions_len) == 0) {
            return true; /* instruction is a reserved instruction name */
        }
    }
    return 0;
}

int second_group_instructions(const char *instruction){
    const char *instruction_names[] = {"not","clr","inc", "dec",
                                       "jmp","bne", "red", "prn","jsr"};
    int i, num_instructions = sizeof(instruction_names) / sizeof(instruction_names[0]);
    int instructions_len;

    if(instruction==NULL){
        return NO_ERROR;
    }
    instructions_len = (strchr(instruction, '\n') ? strlen(instruction) - 1 : strlen(instruction));

    for (i = 0; i < num_instructions; i++) {
        if (strncmp(instruction, instruction_names[i], instructions_len) == 0) {
            return ERROR; /* instruction is a reserved instruction name */
        }
    }
    return NO_ERROR;
}

int third_group_instructions(const char *instruction){
    const char *instruction_names[] = {"rts", "stop"};
    int i, num_instructions = sizeof(instruction_names) / sizeof(instruction_names[0]);
    int instructions_len;

    if(instruction==NULL){
        return NO_ERROR;
    }
    instructions_len = (strchr(instruction, '\n') ? strlen(instruction) - 1 : strlen(instruction));

    for (i = 0; i < num_instructions; i++) {
        if (strncmp(instruction, instruction_names[i], instructions_len) == 0) {
            return ERROR; /* instruction is a reserved instruction name */
        }
    }
    return NO_ERROR;
}

/* assumed: str doesn't have leading and trailing whitespaces a part from newline at the end */
/* @return 1 if it is reserved instruction name, and 0 if it is not */
int is_instruction(const char *str){
    if(first_group_instructions(str) ||
        second_group_instructions(str) ||
        third_group_instructions(str))
        return true;
    return 0;
}

int is_directive(const char *str){
    if(strcmp(str, ".extern")==0 ||
        strcmp(str, ".entry")==0 ||
        strcmp(str, ".data")==0 ||
        strcmp(str, ".string")==0){
        return true;
    }
    return 0;
}

/* check with the manhim if data string extern entry are one of the reserved words*/
int is_reserved(const char *str){
    const char *reserved_names[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7",
                                    "mov", "cmp", "add", "sub", "not", "clr", "lea",
                                    "inc", "dec", "jmp", "bne", "red", "prn","jsr",
                                    "rts", "stop", ".data", ".string",
                                    ".extern", ".entry","mcr", "endmcr"};
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

void error_msg(char* filename, const int line_number, int* error_state, int count, ...) {
    int i;
    va_list args;
    va_start(args, count);

    fprintf(stdout, ERROR_ );
    if(filename!=NULL) {
        fprintf(stdout, BLUE"%s: "WHITE, filename);
    }
    if (line_number != 0) {
        fprintf(stdout, LINE_ "%d: ", line_number);
    }
    for (i = 0; i < count; i++) {
        char* arg = va_arg(args, char*);
        fprintf(stdout, "%s", arg);
    }
    fprintf(stdout, "\n");

    /* Update the error state */
    if (error_state != 0) {
        *error_state = 1;
    }
    va_end(args);
}

void warning_msg(char* filename, int line_number, int count, ...){
    int i;
    va_list args;
    va_start(args, count);

    fprintf(stdout, WARNING );
    if(filename!=NULL) {
        fprintf(stdout, BLUE"%s: "WHITE, filename);
    }
    if (line_number != 0) {
        fprintf(stdout, LINE_ "%d: ", line_number);
    }
    for (i = 0; i < count; i++) {
        char* arg = va_arg(args, char*);
        fprintf(stdout, "%s", arg);
    }
    fprintf(stdout, "\n");
    va_end(args);
}


