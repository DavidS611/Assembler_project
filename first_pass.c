#include "first_pass.h"

symbol_table *first_pass(char *file_name, FILE *fp_am){
    symbol_table *st = (symbol_table*) malloc(sizeof(symbol_table));
    int line_number=1, error_state = NO_ERROR, ic, dc;
    char line[LINE_SIZE], *label_name=NULL;

    /* Initializing */
    ic = START_MEMORY;
    dc = 0;
    st->head = NULL;

    /* Filling the symbol-table */
    while(fgets(line, LINE_SIZE, fp_am)!=NULL) {
        /* Looking for possible errors */
        error_handling(file_name, line, &error_state, line_number, st);
        /* Valid label (not in entry or extern and ends with ':') */
        if(strstr(line, ".entry")==NULL && strstr(line, ".extern")==NULL && strchr(line, ':')!=NULL){
            insert_label(file_name, &error_state, line_number, st, line, &ic, &dc);
        }
        dc += data_counter(line);
        ic += instruction_counter(line);

        line_number++;
    }

    correcting_data(st, ic);

    rewind(fp_am);
    /* Redefine entry and extern labels */
    define_entry_and_extern(file_name, error_state, fp_am, st);

    print_symbol_table(st);
    /* If error was found return NULL and do not continue to the second_pass */
    if(error_state){
        return NULL;
    }
    return st;
}

void define_entry_and_extern(char *file_name, int error_state, FILE *fp_am, symbol_table *st){
    char line[LINE_SIZE], *label_name;
    symbol_entry *curr=NULL;

    while(fgets(line, LINE_SIZE, fp_am)!=NULL) {
        /* Redefine .entry labels */
        if(strstr(line, ".entry")!=NULL){
            label_name = strstr(line, ".entry") + strlen(".entry");
            label_name = strtok(label_name, DELIMITER);
            trim_whitespace(label_name);
            curr = get_label(label_name ,st);
            curr->is_entry = true;
            curr->is_code = false;
            curr->is_data = false;
        }

        /* Redefine .extern labels */
        if(strstr(line, ".extern")!=NULL){
            label_name = strstr(line, ".extern") + strlen(".extern");
            label_name = strtok(label_name, DELIMITER);
            curr = get_label(label_name ,st);
            curr->is_extern = true;
            curr->is_code = false;
            curr->is_data = false;
        }

        /* Check for invalid label - defined as extern and entry simultaneously */
        curr = st->head;
        while (curr != NULL) {
            if(curr->is_extern==true && curr->is_entry==true){
                error_msg(file_name, NO_ARGUMENT, &error_state,
                          3, "Label can't be entry and extern simultaneously: \'", curr->label, "\'.");
            }
            curr = curr->next;
        }
    }

}

int is_label(char *str, symbol_table *st) {
    symbol_entry *curr = st->head;
    while (curr != NULL) {
        if (strcmp(curr->label, str) == 0) {
            return curr->address;
        }
        curr = curr->next;
    }
    return 0;
}

symbol_entry *get_label(char *str, symbol_table *st) {
    symbol_entry *curr = st->head;
    while (curr != NULL) {
        if (strcmp(curr->label, str) == 0) {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}


/* Assume there is no errors in the line (check in the error handling function) */
void insert_label(char *file_name, int *error_state, int line_number, symbol_table *st, char *line, int *instruction_counter, int *data_counter) {
    char copy_line[LINE_SIZE], *label_name;
    symbol_entry *new_entry = malloc(sizeof(symbol_entry));

    if (!new_entry) {
        error_msg(file_name, line_number, error_state,
                  1,"Failed to allocate memory for label entry.");
        exit(EXIT_FAILURE);
    }
    new_entry->is_extern=false;
    new_entry->is_data=false;
    new_entry->is_entry=false;
    new_entry->is_code = false;

    strcpy(copy_line, line);
    label_name = strtok(copy_line, ":");
    new_entry->label = _strdup(file_name, label_name);

    if(strstr(line, ".data")!=NULL){
        new_entry->is_data=true;
        new_entry->address = *data_counter;
    }
    else if(strstr(line, ".string")!=NULL){
        new_entry->is_data = true;
        new_entry->address = *data_counter;
    }
    else{
        new_entry->is_code = true;
        new_entry->address = *instruction_counter;
    }

    new_entry->next = st->head;
    st->head = new_entry;
}

int data_counter(char *line) {
    int counter = 0, is_data = 0, is_string = 0, i;
    char *token, copy_line[LINE_SIZE];

    /* Check if the line is a .string or .data directive */
    is_data = strstr(line, ".data") != NULL;
    is_string = strstr(line, ".string") != NULL;
    if (is_data == 0 && is_string == 0) {
        return 0;
    }
    strcpy(copy_line, line);
    if (is_data) {
        /* counter equal to 1 for the first integer, before each integer there is comma, so I count the commas instead*/
        for (i = 0, counter = 1; i < strlen(line); i++) {
            if (line[i] == ',') {
                counter++;
            }
        }
        return counter;
    } else {
        token = strchr(line, '"'); /* points to the start of the string */
        trim_whitespace(token);
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
        is_jump_instruction = strcmp(instruction, "jmp") == 0 ||
                              strcmp(instruction, "bne") == 0 ||
                              strcmp(instruction, "jsr") == 0;

    /* Get the first parameter of the instruction */
    parameter1 = strtok(NULL, ",");

    /* Calculate the number of memory words required based on the instruction type and parameters */

    /* First group instructions have two operands */
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
            /* One operand without parameters */
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
    symbol_entry *current = st->head;
    while (current != NULL) {
        if (current->is_data==true) {
            current->address += ic;
        }
        current = current->next;
    }
}

void error_handling(char *file, char *line, int *error_state, int line_number, symbol_table *st){
    char copy_line[LINE_SIZE], *label=NULL, *instruction, *param1, *param2, *ptr;
    int i;

    trim_whitespace(line); /* Trimming leading and trailing whitespaces */
    strcpy(copy_line, line);

    /** LABEL CHECK */
    if(strchr(copy_line, ':')!=NULL){

        /* Empty label name check*/
        if(line[0]==':'){
            error_msg(file, line_number, error_state,
                      1, "Invalid label name, label name must contain at least one letter or number.");
        }
        label = strtok(copy_line, ":");
        /* First letter check */
        if(isalpha(label[0])==0){
            error_msg(file, line_number, error_state, 1, "Label name must start with a letter.");
        }
        /* Letters and numbers inside the label name check */
        for(i=0; i< strlen(label); i++) {
            if(isalnum(label[i])==0){
                error_msg(file, line_number, error_state,
                          1, "Invalid label name, label name must contain only letters or numbers.");
            }
        }
        /* Label name length check */
        if(label!=NULL && strlen(label)>LABEL_SIZE){
            error_msg(file, line_number, error_state,
                      1, "Label name can not be longer than 30 characters.");
        }
        /* Old label name check */
        if(is_label(label, st)){
            error_msg(file, line_number, error_state,
                      1, "Label name is already exists.");
        }
        /* Reserved system names check */
        if(is_reserved(label)){
            error_msg(file, line_number, error_state,
                      1, "Label name can not be a reserved word in the system.");
        }

        instruction = strtok(NULL, DELIMITER);

        /* Whitespace check after label name */
        ptr = (strchr(line, ':')+1); /* points to the whitespace after instruction name */
        if(*ptr!='\0' && isspace(*ptr)==0){
            error_msg(file, line_number, error_state,
                      1, "There should be whitespaces between the label name and instruction or directive.");
        }
    }
    else{
        /*  If a label name is not provided */
        instruction = strtok(copy_line, DELIMITER);
        trim_whitespace(instruction);
    }



    /** INSTRUCTION CHECK */
    if(instruction==NULL){
        error_msg(file, line_number, error_state, 1, "Missing instruction or directive.");
    }
    else if(instruction!=NULL && is_instruction(instruction)==0 && is_directive(instruction)==0){
        error_msg(file, line_number, error_state,
                  1, "Undefined instruction or directive.");
    }
    else if(strcmp(instruction, ".data")==0){
        param1 = strtok(NULL, ",");
        while(param1!=NULL){
            trim_whitespace(param1);
            /* First number char check for every number */
            if(param1[0]!='-' && param1[0]!='+' && isdigit(param1[0])==0) {
                error_msg(file, line_number, error_state,
                          1, "Number can start only with '+' or '-' or a digit number.");
            }
            if(atoi(param1)>=8191 || atoi(param1)<(-8192)){
                error_msg(file, line_number, error_state,
                          3, "Invalid number \"", param1, "\" out of range.");
            }
            for(i=1; i< strlen(param1); i++){
                if(isdigit(param1[i])==0){
                    error_msg(file, line_number, error_state,
                              3, "Invalid integer \"", param1,"\".");
                }
            }
            param1 = strtok(NULL, ",");
        }
        if(isdigit(line[strlen(line)-1])==0){
            error_msg(file, line_number, error_state,
                      1, "Extraneous text after end of '.data' directive.");
        }
    }

    else if(strcmp(instruction, ".string")==0){
        param1 = strtok(NULL, "\n");
        trim_whitespace(param1);
        if(param1==NULL){
            error_msg(file, line_number, error_state, 1, "Missing argument.");
        }
        if(param1!=NULL && (param1[0]!='\"' || param1[strlen(param1)-1]!='\"')){
            error_msg(file, line_number, error_state,
                      1, "Invalid string, must start and end with quotation marks.");
        }
    }

    else if(strcmp(instruction, ".extern")==0){
        if(label!=NULL){
            warning_msg(file, line_number, 1,"Label name before '.extern' directive is meaningless.");
        }
        param1 = strtok(NULL, DELIMITER);
        trim_whitespace(param1);
        param2 = strtok(NULL, DELIMITER);
        if(param2!=NULL){
            error_msg(file, line_number, error_state,
                      1,"Extraneous text after end of '.extern' directive.");
        }
    }

    else if(strcmp(instruction, ".entry")==0){
        if(label!=NULL){
            warning_msg(file, line_number, 1,"Label name before '.entry' directive is meaningless.");
        }
        param1 = strtok(NULL, DELIMITER);
        trim_whitespace(param1);
        param2 = strtok(NULL, DELIMITER);
        if(param2!=NULL){
            error_msg(file, line_number, error_state,
                      1,"Extraneous text after end of '.extern' directive.");
        }
    }

    else if(strcmp(instruction, "jmp")==0 || strcmp(instruction, "bne")==0 || strcmp(instruction, "jsr")==0){

    }
}

void print_symbol_table(symbol_table* st) {
    symbol_entry* curr = st->head;

    /* Skip printing if the symbol table is empty */
    if (st->head == NULL) {
        return;
    }
    /* Printing the symbol table */
    printf("\n----------------------------------------------------\n");
    printf("\t\tSymbol Table:\n");
    printf("----------------------------------------------------\n");
    printf("decimal address |\tlabel name  |\ttype\n");
    printf("----------------------------------------------------\n");
    while (curr != NULL) {
        printf("\t%d\t|\t%s\t    |\t%s\n", curr->address, curr->label,
               (curr->is_data==true || curr->is_code==true)? (curr->is_data==true ? "DATA" : "CODE") :
               (curr->is_extern==true ? "EXTERN" : "ENTRY"));
        curr = curr->next;
    }
    putchar('\n');
}
