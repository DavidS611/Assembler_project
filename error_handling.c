
#include "error_handling.h"

void label_check(char *file, int *error_state, int line_number, char *line){
    char copy_line[LINE_SIZE], *label=NULL, *ptr;
    int i;

    strcpy(copy_line, line);

    label = strtok(copy_line, ":");

    /* Empty label name check*/
    if(line[0]==':'){
        error_msg(file, line_number, error_state,
                  1, "Invalid label name, label name must contain at least one letter or number.");
    }
    else {
        /* First letter check */
        if (label != NULL && isalpha(label[0]) == 0) {
            error_msg(file, line_number, error_state, 1, "Label name must start with a letter.");
        }
        /* Letters and numbers inside the label name check */
        for (i = 0; label != NULL && i < strlen(label); i++) {
            if (isalnum(label[i]) == 0) {
                error_msg(file, line_number, error_state,
                          1, "Invalid label name, label name must contain only letters or numbers.");
            }
        }
        /* Label name length check */
        if (label != NULL && strlen(label) >= LABEL_SIZE - 1) {
            error_msg(file, line_number, error_state,
                      1, "Label name can not be longer than 30 characters.");
        }
        /* Reserved system names check */
        if (label != NULL && is_reserved(label)) {
            error_msg(file, line_number, error_state,
                      1, "Label name can not be a reserved word in the system.");
        }
    }

    /* Whitespace check after label name */
    ptr = (strchr(line, ':')+1); /* points to the whitespace after label name (including ':') */
    if(*ptr!='\0' && isspace(*ptr)==false){
        error_msg(file, line_number, error_state,
                  1, "There should be whitespaces between the label name and instruction or directive.");
    }
}

void data_check(char *file, int *error_state, int line_number ,char *line){
    char *param1, *ptr;
    int i;

    /* Pointer to '.data' directive string */
    param1 = strtok_trimmed(line, DELIMITER);
    /* Pointer to First number */
    param1 = strtok(NULL, ",");

    /* No numbers check */
    if (param1 == NULL){
        error_msg(file, line_number, error_state, 1, "No numbers after '.data' directive.");
    }
    /* Loop through each number */
    while(param1!=NULL){
        trim_whitespace(param1);
        /* Extra comma at the end of the line check */
        if (strcmp(param1, "")==0){
            error_msg(file, line_number, error_state, 1, "Comma at the end of .data directive.");
        }
        /* First number char check for each number provided */
        else if(param1[0]!='-' && param1[0]!='+' && isdigit(param1[0])==0) {
            error_msg(file, line_number, error_state,
                      1, "Number can start only with '+' or '-' or a digit number.");
        }
        /* Range of number check */
        if(atoi(param1)>=8191 || atoi(param1)<(-8192)){
            error_msg(file, line_number, error_state,
                      3, "Invalid number: \'", param1, "\' out of range.");
        }
        /* Check if one of the chars isn't a digit */
        for(i=0; i< strlen(param1); i++){
            if(isdigit(param1[0])==0 && param1[0]!='-' && param1[0]!='+'){
                error_msg(file, line_number, error_state,3, "Invalid integer: \'", param1,"\'.");
                i = strlen(param1); /* exit loop */
            }
            else if(i!=0 && isdigit(param1[i])==0){
                error_msg(file, line_number, error_state,3, "Invalid integer: \'", param1,"\'.");
                i = strlen(param1); /* exit loop */
            }
        }
        param1 = strtok(NULL, ",");
    }
    /* Checks if the last char isn't a char (excluding whitespaces or comments because it was trimmed before) */
    ptr = strstr(line, ".data") + strlen(".data");
    for(; *ptr!='\0'; ptr++){
        if (isdigit(*ptr) || isspace(*ptr) || *ptr==',') {
            continue;
        }
    }
    /* Last char is comma check */
    if (*(ptr-1)==','){
        error_msg(file, line_number, error_state,1, "Extraneous text after end of '.data' directive.");
    }

}

void string_check(char *file, int line_number, int *error_state, char *line) {
    char *param1;

    /* Pointer to '.string' directive string */
    param1 = strtok_trimmed(line, DELIMITER);
    /* Pointer to First number */
    param1 = strtok_trimmed(NULL, NEW_LINE);

    /* No string check */
    if (param1 == NULL) {
        error_msg(file, line_number, error_state, 1, "Missing string after '.string' directive.");
    }
    /* param1 contains the string with quotation marks (e.g., param1="abcdef") */
    else {
        if (strlen(param1)<2){
            error_msg(file, line_number, error_state, 1, "Missing quotation marks.");
        }
        else{
            if (param1[0]!='\"' || param1[strlen(param1)-1]!='\"'){
                error_msg(file, line_number, error_state, 1, "The string must start and end with quotation marks.");
            }
        }
    }
}

void extern_check(char *file, int line_number, int *error_state, char *line){
    char *label=NULL, *extern_label;

    if (strchr(line, ':')!=NULL){
        label = strtok_trimmed(line, DELIMITER);
        extern_label = strtok_trimmed(NULL, DELIMITER); /* instruction string here temporary*/
    }
    else{
        extern_label = strtok_trimmed(line, DELIMITER); /* instruction string here temporary*/
    }

    extern_label = strtok_trimmed(NULL, DELIMITER);

    /* Warning of label name before .extern directive */
    if(label!=NULL){
        warning_msg(file, line_number, 1,"Label name before '.extern' directive is meaningless.");
    }

    /* Empty .extern directive check */
    if (extern_label == NULL){
        error_msg(file, line_number, error_state, 1, "Missing argument.");
    }
    /* Reserved name check */
    if (is_reserved(extern_label)){
        error_msg(file, line_number, error_state, 1, "Reserved system name can not be extern.");
    }
    /* Extra chars check */
    extern_label = strtok_trimmed(NULL, NEW_LINE); /* Chars after extern label name without whitespaces */
    if(extern_label!=NULL){
        error_msg(file, line_number, error_state,
                  1,"Extraneous text after end of '.extern' directive.");
    }
}

void entry_check(char *file, int line_number, int *error_state, symbol_table *st, char *line){
    char *label=NULL, *entry_label;

    if (strchr(line, ':')!=NULL){
        label = strtok_trimmed(line, DELIMITER);
        entry_label = strtok_trimmed(NULL, DELIMITER); /* instruction string here temporary*/
    }
    else{
        entry_label = strtok_trimmed(line, DELIMITER); /* instruction string here temporary*/
    }

    /* Warning label name before .entry directive */
    if(label!=NULL){
        warning_msg(file, line_number, 1,"Label name before '.entry' directive is meaningless.");
    }
    entry_label = strtok_trimmed(NULL, DELIMITER); /* The entry label name */
    if (entry_label==NULL){
        error_msg(file, line_number, error_state, 1, "Missing argument.");
    }
    /* Reserved name check */
    else if (is_reserved(entry_label)){
        error_msg(file, line_number, error_state, 1, "Reserved system name can not be entry.");
    }
    /* Exists label check */
    else if (is_label(st, entry_label)==false){
        error_msg(file, line_number, error_state, 1, "Label name doesn't exists.");
    }

    /* Extra chars check */
    label = strtok_trimmed(NULL, NEW_LINE); /* Chars after entry label name */
    if(label!=NULL){
        error_msg(file, line_number, error_state,
                  1,"Extraneous text after end of '.entry' directive.");
    }
}

void first_group_check(char *file, int line_number, int *error_state, symbol_table *st, char *line){
    char *instruction, *operand1, *operand2, *extra_chars;

    instruction = strtok_trimmed(line, DELIMITER);
    operand1 = strtok_trimmed(NULL, COMMA);

    if (operand1==NULL){
        error_msg(file, line_number, error_state, 1, "Missing argument.");
    }
    else{
        /* Source operand is register or a number and instruction is 'lea' */
        if ((is_register(operand1) || is_num(operand1)) && strcmp(instruction, "lea")==TRUE) {
            error_msg(file, line_number, error_state,1, "For 'lea' instruction source operand must be label.");
        }
        /* Invalid source operand check */
        if (is_num(operand1)==false && is_label(st, operand1)==false && is_register(operand1)==false){
            error_msg(file, line_number, error_state, 1, "Invalid Source operand.");
        }
        if (is_instruction(operand1) || is_directive(operand1)){
            error_msg(file, line_number, error_state, 1, "Source operand can't be reserved system name.");
        }

        operand2 = strtok_trimmed(NULL, DELIMITER);

        if (operand2==NULL){
            error_msg(file, line_number, error_state, 1, "Missing argument.");
        }
        else{
            /* Destination operand is a number and instruction isn't 'cmp' */
            if (is_num(operand2) && strcmp(instruction, "cmp")) {
                error_msg(file, line_number, error_state,1, "Invalid destination operand.");
            }
            /* Invalid destination operand check */
            if (is_num(operand2)==false && is_label(st, operand2)==false && is_register(operand2)==false){
                error_msg(file, line_number, error_state, 1, "Invalid Destination operand.");
            }
            if (is_instruction(operand2) || is_directive(operand2)){
                error_msg(file, line_number, error_state, 1, "Destination operand can't be reserved system name.");
            }
        }
    }

    /* Extra chars check */
    extra_chars = strtok_trimmed(NULL, DELIMITER);
    if (extra_chars!=NULL){
        error_msg(file, line_number, error_state,1,"Extraneous text after end of instruction.");
    }
}

void second_group_check(char *file, int line_number, int *error_state, symbol_table *st, char *line){
    char *instruction, *operand2, *param1, *param2, *extra_chars;

    /* Addressing method number 2 ('10')*/
    if (strchr(line, '(')!=NULL){
        instruction = strtok_trimmed(line, DELIMITER);
        /* Correct instruction for addressing method check */
        if (strcmp(instruction, "jmp")==false && strcmp(instruction, "bne")==false && strcmp(instruction, "jsr")==false){
            error_msg(file, line_number, error_state, 1, "Invalid instruction for addressing method number 2.");
        }

        operand2 = strtok_trimmed(NULL, NEW_LINE); /* the label with both param1 and param2, should be: 'label(param1,param2)'*/
        /* Checking whitespaces inside destination operand */
        if (strchr(operand2, ' ')!=NULL || strchr(operand2, '\t')!=NULL){
            error_msg(file, line_number, error_state, 3, "Unexpected whitespace found inside destination operand: \'", operand2, "\'.");
        }
        /* Checking missing comma between param1 and param2 */
        if (strchr(operand2, ',')==NULL){
            error_msg(file, line_number, error_state, 3, "Missing comma inside destination operand: \'", operand2, "\'.");
        }

        /* Destination operand label check */
        operand2 = strtok(operand2, "(");
        if (is_reserved(operand2)==true){
            error_msg(file, line_number, error_state, 1, "Destination operand can not be reserved system name.");
        }
        else if (is_label(st, operand2)==false){
            error_msg(file, line_number, error_state, 3, "Invalid Destination operand, the label \'", operand2, "\' is not defined.");
        }
        if (is_instruction(operand2) || is_directive(operand2)){
            error_msg(file, line_number, error_state, 1, "Destination operand can't be reserved system name.");
        }

        /* First parameter check */
        param1 = strtok(NULL, COMMA);
        if (param1==NULL){
            error_msg(file, line_number, error_state, 1, "Missing argument.");
        }
        else{
            if (is_num(param1)==false && is_label(st, param1)==false && is_register(param1)==false){
                error_msg(file, line_number, error_state, 1, "Invalid first parameter.");
            }
            if (is_instruction(param1) || is_directive(param1)){
                error_msg(file, line_number, error_state, 1, "First parameter can't be reserved system name.");
            }

            /* Second parameter check */
            param2 = strtok(NULL, ")");
            if (param2 == NULL){
                error_msg(file, line_number, error_state, 1, "Missing argument.");
            }
            else {
                if (is_num(param2)==false && is_label(st, param2)==false && is_register(param2)==false){
                    error_msg(file, line_number, error_state, 1, "Invalid second parameter.");
                }
                if (is_instruction(param2) || is_directive(param2)){
                    error_msg(file, line_number, error_state, 1, "Second parameter can't be reserved system name.");
                }
            }
        }

        /* Extra chars check */
        extra_chars = strtok_trimmed(NULL, NEW_LINE);
        if (extra_chars!=NULL){
            error_msg(file, line_number, error_state,1,"Extraneous text after end of instruction.");
        }
    }
    else {
        instruction = strtok_trimmed(line, DELIMITER);
        operand2 = strtok_trimmed(NULL, DELIMITER);
        if (operand2==NULL){
            error_msg(file, line_number, error_state, 1, "Missing argument.");
        }
        else{
            /* Invalid destination operand check */
            /* Checks that the dest operand isn't a label and not a register.
             * For the 'prn' instruction, check that the dest operand isn't number. */
            if (is_label(st, operand2)==false && is_register(operand2)==false && !(is_num(operand2) && strcmp(instruction, "prn")==0)){
                error_msg(file, line_number, error_state, 1, "Invalid destination operand.");
            }
            /* Reserved system name check */
            if (is_instruction(operand2) || is_directive(operand2)){
                error_msg(file, line_number, error_state, 1, "Destination operand can't be reserved system name.");
            }
            /* Extra chars check */
            extra_chars = strtok_trimmed(NULL, NEW_LINE);
            if (extra_chars!=NULL){
                error_msg(file, line_number, error_state,1,"Extraneous text after end of instruction.");
            }
        }

    }
}

void syntax_errors(char *file, char *line, int *error_state, int line_number, symbol_table *st){
    char copy_line[LINE_SIZE], *instruction, *ptr, *extra_chars;

    /** LABEL ERROR CHECK */ /* only if there is a label name */
    if(strchr(line, ':')!=NULL){ /* Check if there is a label name */
        label_check(file, error_state, line_number, line);
        ptr = strstr(line, ":") + strlen(":");
    } else {
        ptr = line;
    }

    strcpy(copy_line, ptr);
    instruction = strtok_trimmed( copy_line, DELIMITER);

    /** INSTRUCTION ERROR CHECK */
    /* No instruction check */
    if(instruction==NULL){
        error_msg(file, line_number, error_state, 1, "Missing instruction or directive.");
    }
    /* Undefined instruction check */
    else if(instruction!=NULL && is_instruction(instruction)==false && is_directive(instruction)==false){
        error_msg(file, line_number, error_state,
                  1, "Undefined instruction or directive.");
    }
    /* Check errors '.data' directive */
    else if(strcmp(instruction, ".data")==TRUE){
        data_check(file, error_state, line_number, ptr);
    }
    /* Check errors '.string' directive */
    else if(strcmp(instruction, ".string")==TRUE){
        string_check(file, line_number, error_state, ptr);
    }
    /* Check for errors in '.extern' directive */
    else if(strcmp(instruction, ".extern")==TRUE){
        extern_check(file, line_number, error_state, line);
    }
    /* Check for errors in '.entry' directive */
    else if(strcmp(instruction, ".entry")==TRUE){
        entry_check(file, line_number, error_state, st, line);
    }
    /* Check for errors in first instruction group */
    else if(first_group_instructions(instruction)==true){
        first_group_check(file, line_number, error_state, st, ptr);
    }
    /* Check for errors in second instruction group */
    else if(second_group_instructions(instruction)==true){
        second_group_check(file, line_number, error_state, st, ptr);
    }
    /* Check for errors in third instruction group */
    else if(third_group_instructions(instruction)){
        /* Extra chars check */
        extra_chars = strtok_trimmed(NULL, DELIMITER);
        if (extra_chars!=NULL){
            error_msg(file, line_number, error_state,1,"Extraneous text after end of instruction.");
        }
    }
}
