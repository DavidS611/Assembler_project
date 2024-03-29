#include "utils.h"

FILE* open_file(char* filename, const char* mode) {
    FILE* fp = fopen(filename, mode);
    if (fp == NULL) {
        error_msg(filename, NO_ARGUMENT, NO_ARGUMENT, 1,"Failed to open the file.");
    }
    return fp;
}

void generate_filename(char *filename, char *suffix, char **new_filename) {
    int len = strlen(filename) + strlen(suffix) + 1; /* add 1 for null terminator */
    char *temp_filename = malloc(len * sizeof(char));

    if (temp_filename == NULL) {
        error_msg(filename, NO_ARGUMENT, NO_ARGUMENT,4, "Failed to allocate memory for file name: \'", filename, suffix, "\'");
    }

    strcpy(temp_filename, filename);
    strcat(temp_filename, suffix);
    *new_filename = temp_filename;
}

void delete_file(char *file_name, char *suffix) {
    char file_to_delete[FILENAME_MAX];

    strcpy(file_to_delete, file_name);
    /* If suffix is provided build the full filename to delete */
    if(suffix != NULL){
        strcat(file_to_delete, suffix);
    }

    /* Remove file, if fail, print an error message. */
    if (remove(file_to_delete) != 0) {
        error_msg(file_name, NO_ARGUMENT, NO_ARGUMENT,
                  3, "Failed to delete \"", file_to_delete, "\"");
    }
}

void close_files(int count, ...) {
    va_list args;
    int i;
    FILE* fp;

    va_start(args, count); /* Initialize the argument list. */
    /* Loop through each file pointer */
    for (i = 0; i < count; i++) {
        /* Get the next file pointer from the argument list. */
        fp = va_arg(args, FILE*);
        /* If the file pointer is not null, close it. */
        if (fp != NULL) {
            fclose(fp);
        }
    }
    va_end(args); /* Clean up the argument list. */
}

void free_pointers(int count, ...) {
    int i;
    va_list args;

    va_start(args, count);  /* initialize variable argument list */
    /* Loop through each pointer */
    for (i = 0; i < count; i++) {
        /* Get the next pointer from argument list */
        void* ptr = va_arg(args, void*);
        free(ptr);
    }
    va_end(args); /* Clean up the argument list. */
}

bool empty_line(char line[]){
    int i=0;

    /* Skip leading whitespaces */
    for(; isspace(line[i]); i++);
    /* Check if the line is a comment or empty */
    if(line[i]==';' || line[i] == '\n' || line[i]=='\0')
        return true;
    return false;
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

    /* Null pointer check */
    if(str == NULL){
        return NULL;
    }

    p = malloc(strlen(str) + 1); /* adding 1 for '\0'*/
    if (p==NULL){
        error_msg(filename, NO_ARGUMENT, NO_ARGUMENT,
                  3, "Failed to allocate memory for \'", str, "\'.");
        return NULL;
    }
    strcpy(p, str);
    return p;
}

bool is_register(const char *str){
    const char *register_names[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};
    int i, num_registers = sizeof(register_names) / sizeof(register_names[0]);
    int str_len;

    /* Null pointer check */
    if(str==NULL) {
        return false;
    }
    /* Length of str both cases with and without newline at the end */
    str_len = (strchr(str, '\n') ? strlen(str) - 1 : strlen(str));
    /* Loop through each register string for comparison */
    for (i = 0; i < num_registers; i++) {
        if (strncmp(str, register_names[i], str_len) == 0) {
            return true; /* str is a reserved register name */
        }
    }
    return false;
}

bool first_group_instructions(const char *instruction){
    int i, num_instructions, instructions_len;
    const char *instruction_names[] = {"mov", "cmp", "add", "sub", "lea"};

    /* Number of instruction names array */
    num_instructions = sizeof(instruction_names) / sizeof(instruction_names[0]);
    /* Null pointer check */
    if(instruction==NULL){
        return false;
    }
    /* Length of instruction string with and without a newline at the end */
    instructions_len = (strchr(instruction, '\n') ? strlen(instruction) - 1 : strlen(instruction));
    /* Loop through each instruction string for comparison */
    for (i = 0; i < num_instructions; i++) {
        if (strncmp(instruction, instruction_names[i], instructions_len) == 0) {
            return true; /* instruction is a reserved instruction name */
        }
    }
    return false;
}

bool second_group_instructions(const char *instruction){
    int i, num_instructions, instructions_len;
    const char *instruction_names[] = {"not","clr","inc", "dec","jmp","bne", "red", "prn","jsr"};

    /* Number of instruction names array */
    num_instructions = sizeof(instruction_names) / sizeof(instruction_names[0]);
    /* Null pointer check */
    if(instruction==NULL){
        return false;
    }
    /* Length of instruction string with and without a newline at the end */
    instructions_len = (strchr(instruction, '\n') ? strlen(instruction) - 1 : strlen(instruction));
    /* Loop through each instruction string for comparison */
    for (i = 0; i < num_instructions; i++) {
        if (strncmp(instruction, instruction_names[i], instructions_len) == 0) {
            return true; /* instruction is a reserved instruction name */
        }
    }
    return false;
}

bool third_group_instructions(const char *instruction){
    const char *instruction_names[] = {"rts", "stop"};
    int i, num_instructions = sizeof(instruction_names) / sizeof(instruction_names[0]);
    int instructions_len;

    /* Null pointer check */
    if(instruction==NULL){
        return false;
    }
    /* Length of instruction string with and without a newline at the end */
    instructions_len = (strchr(instruction, '\n') ? strlen(instruction) - 1 : strlen(instruction));
    /* Loop through each instruction string for comparison */
    for (i = 0; i < num_instructions; i++) {
        if (strncmp(instruction, instruction_names[i], instructions_len) == 0) {
            return true; /* instruction is a reserved instruction name */
        }
    }
    return false;
}

bool is_instruction(const char *str){
    if(first_group_instructions(str) || second_group_instructions(str) || third_group_instructions(str))
        return true;
    return false;
}

bool is_directive(const char *str){
    if(strcmp(str, ".extern")==0 || strcmp(str, ".entry")==0 || strcmp(str, ".data")==0 || strcmp(str, ".string")==0){
        return true;
    }
    return false;
}

bool is_reserved(const char *str){
    int i, str_len, num_names;
    const char *reserved_names[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7",
                                    "mov", "cmp", "add", "sub", "not", "clr", "lea",
                                    "inc", "dec", "jmp", "bne", "red", "prn","jsr",
                                    "rts", "stop", ".data", ".string",
                                    ".extern", ".entry","mcr", "endmcr"};

    /* Number of reserved names array */
    num_names = sizeof(reserved_names) / sizeof(reserved_names[0]);
    /* Null pointer check */
    if(str==NULL)
        return false;
    /* Length of string with and without a newline at the end */
    str_len = (strchr(str, '\n') ? strlen(str) - 1 : strlen(str));
    /* Loop through each reserved string for comparison */
    for (i = 0; i < num_names; i++) {
        if (strncmp(str, reserved_names[i], str_len) == 0 && reserved_names[i][str_len]=='\0') {
            return true; /* str is a reserved register name */
        }
    }
    return false;
}

bool is_label(symbol_table *st, char *str) {
    symbol_entry *curr = get_head(st);

    /* Null pointer check */
    if (str==NULL)
        return false;
    /* Loop through each label name for comparison */
    while (curr != NULL) {
        if (strcmp(get_label(curr), str) == 0) {
            return true;
        }
        curr = get_next(curr);
    }
    return false;
}

bool is_num(char *str_num){
    int i;
    char *ptr;

    /* First char have to be '#' check */
    if (str_num[0]!='#')
        return false;
    ptr = str_num+1; /* Points after '#' char */
    /* First number char check */
    if (*ptr!='+'&& *ptr!='-' && isdigit(*ptr)==false)
        return false;
    ptr++; /* Points after first number char */
    /* All the other chars are digits check  */
    for (i=0; i< strlen(ptr); i++){
        if (isdigit(*ptr)==false)
            return false;
    }
    return true;
}

symbol_entry *lookup_label(symbol_table *st, char *str) {
    symbol_entry *curr = get_head(st);

    /* Null pointer check */
    if (str == NULL)
        return NULL;
    /* Loop through each label for comparison */
    while (curr != NULL) {
        if (strcmp(get_label(curr), str) == 0) {
            return curr;
        }
        curr = get_next(curr);
    }
    return NULL;
}

void error_msg(char* filename, const int line_number, int* error_state, int count, ...) {
    int i;
    va_list args;
    va_start(args, count);

    fprintf(stdout, RED_CLR"%s:", ERROR_MSG );
    /* File name print */
    if(filename!=NULL) {
        fprintf(stdout, "%s: "WHITE, filename);
    }
    /* Line number print */
    if (line_number != 0) {
        fprintf(stdout, LINE_MSG "%d: ", line_number);
    }
    /* Arguments print */
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

    fprintf(stdout, LIGHT_BLACK_CLR"%s:", WARNING_MSG );
    /* File name print */
    if(filename!=NULL) {
        fprintf(stdout, "%s: "WHITE, filename);
    }
    /* Line number print */
    if (line_number != 0) {
        fprintf(stdout, LINE_MSG "%d: ", line_number);
    }
    /* Arguments print */
    for (i = 0; i < count; i++) {
        char* arg = va_arg(args, char*);
        fprintf(stdout, "%s", arg);
    }
    fprintf(stdout, "\n");
    va_end(args);
}
