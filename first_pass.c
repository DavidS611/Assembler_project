#include "first_pass.h"

void first_pass(char *file_name, FILE *fp_am){
    symbol_table *st = (symbol_table*) malloc(sizeof(symbol_table));
    int line_number=1, error_state = NO_ERROR, ic, dc;
    char line[LINE_SIZE], *file_am;

    /* Initializing */
    rewind(fp_am);
    ic = START_MEMORY;
    dc = 0;
    set_head(st, NULL);
    generate_filename(file_name, ".am", &file_am);

    /* Filling the symbol-table */
    while(fgets(line, LINE_SIZE, fp_am)!=NULL) {
        /* Valid label (not in entry or extern and ends with ':') */
        if(strstr(line, ".entry")==NULL && (strchr(line, ':')!=NULL || strstr(line, ".extern")!=NULL)){
            insert_label(file_am, &error_state, line_number, st, line, &ic, &dc);
        }
        dc += data_counter(line); /* Update data counter */
        ic += instruction_counter(line); /* Update instruction counter */
        line_number++;
    }
    /* Directives will be after instructions */
    correcting_data(st, ic);
    /* Redefine entry labels */
    define_entry_labels(file_am, error_state, fp_am, st);

    /* Error check */
    line_number=1;
    rewind(fp_am);
    while(fgets(line, LINE_SIZE, fp_am)!=NULL) {
        syntax_errors(file_am, line, &error_state, line_number++, st);
    }

    /* Check if the combined size of instruction and data memory exceeds the maximum allowed memory size for the system */
    if(ic+dc > MAX_SYSTEM_MEMORY){
        error_msg(file_name, NO_ARGUMENT, &error_state, 1, "Maximum system memory capacity reached.");
    }

    /* If error was found don't continue to the second pass */
    if(error_state){
        free(file_am);
        free_symbol_table(st);
        return;
    }

    second_pass(file_name, st, fp_am, ic, dc);
    free(file_am);
    free_symbol_table(st);
}

void define_entry_labels(char *file_name, int error_state, FILE *fp_am, symbol_table *st){
    char line[LINE_SIZE], *label_name;
    symbol_entry *curr=NULL;

    rewind(fp_am);
    while(fgets(line, LINE_SIZE, fp_am)!=NULL) {
        /* Define .entry labels */
        if(strstr(line, ".entry")!=NULL){
            label_name = strstr(line, ".entry") + strlen(".entry"); /* Points after .entry directive */
            label_name = strtok_trimmed(label_name, DELIMITER); /* Label name to be defined as entry */
            curr = lookup_label(st, label_name); /* curr points to the label inside the symbol table if exists */
            /* If label name doesn't exist */
            if (curr==NULL){
                return;
            }
            set_is_entry(curr, true);
        }

        /* Check for invalid label - defined as extern and entry simultaneously */
        curr = get_head(st);
        while (curr != NULL) {
            if(get_is_extern(curr) && get_is_entry(curr)){
                error_msg(file_name, NO_ARGUMENT, &error_state,3, "Label can't be entry and extern simultaneously: \'", get_label(curr), "\'.");
            }
            curr = get_next(curr);
        }
    }

}

void insert_label(char *file_name, int *error_state, int line_number, symbol_table *st, char *line, int *instruction_counter, int *data_counter) {
    char copy_line[LINE_SIZE], *label_name, *instruction;
    symbol_entry *new_entry = malloc(sizeof(symbol_entry));

    /* Allocation check */
    if (!new_entry) {
        error_msg(file_name, line_number, error_state,1,"Failed to allocate memory for label entry.");
    }
    /* Initializing */
    set_is_code(new_entry, false);
    set_is_data(new_entry, false);
    set_is_extern(new_entry, false);
    set_is_entry(new_entry, false);

    strcpy(copy_line, line);
    /* Optional label name check */
    if (strchr(copy_line, ':')!=NULL) {
        label_name = strtok(copy_line, ":");
        instruction = strtok_trimmed(NULL, DELIMITER);
    }else{
        instruction = strtok_trimmed(copy_line, DELIMITER);
    }
    /* No instruction check */
    if (instruction==NULL){
        free(new_entry);
        return;
    }
    if(strcmp(instruction, ".data")==TRUE || strcmp(instruction, ".string")==TRUE){
        set_is_data(new_entry, true);
        set_address(new_entry, *data_counter);
    }
    else if(strcmp(instruction, ".extern")==TRUE){
        label_name = strtok_trimmed(NULL, DELIMITER);
        /* Empty extern label */
        if (label_name==NULL){
            free(new_entry);
            return;
        }
        set_is_extern(new_entry, true);
    }
    else{
        /* Check if the label is already exists */
        if (is_label(st, label_name)){
            error_msg(file_name, line_number, error_state, 1, "Label name already exists.");
            free(new_entry);
            return;
        }
        set_is_code(new_entry, true);
        set_address(new_entry, *instruction_counter);
    }
    /* Insert the new label */
    set_label(new_entry, _strdup(file_name, label_name));
    set_next(new_entry, get_head(st));
    set_head(st, new_entry);
}

int data_counter(char *line) {
    int counter = 0, i;
    bool is_data, is_string;
    char *token, copy_line[LINE_SIZE];

    /* Check if the line is a .string or .data directive */
    is_data = strstr(line, ".data") != NULL;
    is_string = strstr(line, ".string") != NULL;
    /* If it's not .data or .string directive */
    if (is_data == false && is_string == false) {
        return 0;
    }

    strcpy(copy_line, line);
    if (is_data) {
        /* Counter equal to 1 for the first integer,
         * before each integer there is comma, so I count the commas instead*/
        for (i = 0, counter = 1; i < strlen(line); i++) {
            if (line[i] == ',') {
                counter++;
            }
        }
        return counter;
    }
    else { /* In case it is .string directive */
        token = strchr(line, '"'); /* points to the start of the string */
        trim_whitespace(token);
        /* If the string is only one quotation mark */
        if(token==NULL)
            return counter;
        /* The length of the string with quotation marks - 1 is equal to the string with the null terminator */
        return strlen(token) - 1;
    }
}

int instruction_counter(char *line){
    char line_copy[LINE_SIZE], *token, *instruction, *parameter1=NULL, *parameter2;
    int is_jump_instruction=0;

    /* Create a copy of the input line to work on */
    strcpy(line_copy, line);

    /* Check if the line contains a label */
    if(strchr(line_copy, ':')!=NULL){
        token = strtok(line_copy, ":"); /* label name */
        instruction = strtok(NULL, DELIMITER); /* instruction name */
    } else {
        instruction = strtok(line_copy, DELIMITER); /* instruction name */
    }
    /* Trim leading and trailing white spaces from the instruction */
    trim_whitespace(instruction);

    /* Check if the instruction is one of the jump instruction */
    if(instruction!=NULL)
        is_jump_instruction = strcmp(instruction, "jmp") == 0 || strcmp(instruction, "bne") == 0 || strcmp(instruction, "jsr") == 0;

    /* Get the first parameter of the instruction */
    parameter1 = strtok(NULL, ",");

    /* Calculate the number of memory words required based on the instruction type and parameters */
    if(first_group_instructions(instruction)){
        parameter2 = strtok(NULL, DELIMITER);
        trim_whitespace(parameter1);
        trim_whitespace(parameter2);
        if(is_register(parameter1) && is_register(parameter2)){
            return 2; /* Explain: instruction word + registers (both in the same word) */
        }
        else {
            return 3; /* Explain: instruction word + source operand word + destination operand word */
        }
    }
    /* Second group instructions have one operand, and might have two parameters (in case of one of the jmp instructions) */
    else if(parameter1!=NULL && second_group_instructions(instruction)){
        parameter2 = strtok(NULL, DELIMITER);
        /* Case of two parameters for one of the jmp instructions */
        if(is_jump_instruction && strchr(parameter1, '(')!=NULL){
            token = strtok(parameter1, "("); /* label word inside token variable */
            token = strtok(NULL, ","); /* parameter-1 inside token variable */
            parameter2 = strtok(parameter2, ")"); /* parameter-2 inside parameter 2 variable*/
            if(is_register(token) && is_register(parameter2)){
                return 3; /* Explain: instruction word + label word + registers(both in the same word)*/
            }
            else{
                return 4;  /* Explain: instruction word + label word + parameter 1 word + parameter 2 word */
            }
        }
        /* Third group one operand without parameters */
        else{
            return 2; /* Explain: instruction word + destination operand word */
        }
    }
    /* Third group instructions have no parameters */
    else if(third_group_instructions(instruction)){
        return 1; /* Explain: instruction word */
    }
    return 0;
}

void correcting_data(symbol_table *st, int ic) {
    symbol_entry *current = get_head(st);
    int new_address;
    while (current != NULL) {
        if (get_is_data(current)) {
            new_address = get_address(current) + ic;
            set_address(current, new_address);
        }
        current = get_next(current);
    }
}

void print_symbol_table(symbol_table* st) {
    symbol_entry* curr = get_head(st);
    char type[6]; /* The longest type is 'EXTERN', so the array is of size 6 to accommodate any of the possible types */

    /* Skip printing if the symbol table is empty */
    if (curr == NULL) {
        return;
    }
    /* Printing the symbol table */
    printf("\n----------------------------------------------------\n");
    printf("\t\tSymbol Table:\n");
    printf("----------------------------------------------------\n");
    printf("decimal address |\tlabel name  |\ttype\n");
    printf("----------------------------------------------------\n");
    while (curr != NULL) {
        printf("\t%d\t|\t%s\t    |\t", (get_is_extern(curr) ?  0: get_address(curr)), get_label(curr));
        strcpy(type, get_is_data(curr) ? "DATA" : (get_is_code(curr) ? "CODE" : get_is_entry(curr)? "ENTRY" : "EXTERN"));
        printf("%s\n", type);
        curr = get_next(curr);
    }
    putchar('\n');
}

void free_symbol_table(symbol_table* table) {
    symbol_entry* entry = get_head(table);
    symbol_entry* next_entry;

    while (entry != NULL) {
        next_entry = get_next(entry);
        free(get_label(entry));
        free(entry);
        entry = next_entry;
    }

    free(table);
}

