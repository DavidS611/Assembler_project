#include "second_pass.h"

void second_pass(char *filename, symbol_table *st, FILE *fp_am, int ic, int dc) {
    char line[LINE_SIZE], *ptr, *instruction, *operand1, *operand2, *parameter1, *parameter2, registers[5];
    word binary_machine_code;
    hash_map map = create_hash_map();
    int i, map_counter=START_MEMORY;

    /* Inserting instructions to the hash map*/
    rewind(fp_am);
    while (fgets(line, LINE_SIZE, fp_am) != NULL) {
        ptr = line;
        /* Point after label name */
        if(strstr(line, ":")!=NULL){
            ptr = strchr(line, ':')+1;
        }
        /* Getting instruction string */
        instruction = strtok_trimmed(ptr, DELIMITER);

        /* First group has two operands */
        if(first_group_instructions(instruction)){
            operand1 = strtok_trimmed(NULL, COMMA);
            operand2 = strtok_trimmed(NULL, DELIMITER);
            /* Converting instruction to binary code */
            binary_machine_code = bits_calculator(st, instruction, operand1, operand2, NULL, NULL);
            insert(&map, map_counter++, instruction, binary_machine_code); /* inserting into the hash map */
            /* Additional words of operands */
            /* If both operands are registers only one new word */
            if(is_register(operand1) && is_register(operand2)){
                binary_machine_code = (register_to_word(operand1)<<R6_SHIFT) | (register_to_word(operand2)<<R7_SHIFT);
                /* Copying into registers array, operand1 (r_) with comma and operand2 (r_) | '_' represents a number between 0 and 7 */
                strcpy(registers, operand1), strcat(registers, ","), strcat(registers, operand2); /* registers = "r[0-7],r[0-7]"*/
                insert(&map, map_counter++, registers, binary_machine_code);
            }
            else {
                /* Source address */
                if (is_register(operand1)){
                    binary_machine_code = (register_to_word(operand1)<<R6_SHIFT);
                }
                else if(is_label(st, operand1)){
                    binary_machine_code = label_to_word(st, operand1);
                }
                else{
                    binary_machine_code = num_to_word(operand1);
                }
                insert(&map, map_counter++, operand1, binary_machine_code);

                /* Destination address */
                if (is_register(operand2)){
                    binary_machine_code = register_to_word(operand2);
                }
                else if(is_label(st, operand2)){
                    binary_machine_code = label_to_word(st, operand2);
                }
                insert(&map, map_counter++, operand2, binary_machine_code);
            }
        }
        /* Second group, one operand or operand with two parameters */
        else if (second_group_instructions(instruction)){
            operand2 = strtok_trimmed(NULL, DELIMITER);
            /* Jump instruction use parameter-based addressing */
            if(strchr(operand2, '(')!=NULL){
                operand2 = strtok_trimmed(operand2, "(");
                parameter1 = strtok_trimmed(NULL, COMMA);
                parameter2 = strtok_trimmed(NULL, ")");
                /* instruction with label and parameters */
                binary_machine_code = bits_calculator(st, instruction, NULL, operand2, parameter1, parameter2);
                insert(&map, map_counter++, instruction, binary_machine_code);
                /* label address */
                binary_machine_code = label_to_word(st, operand2);
                insert(&map, map_counter++, operand2, binary_machine_code);
                /* First and Second parameters */
                if (is_register(parameter1) && is_register(parameter2)){
                    binary_machine_code = (register_to_word(parameter1)<<R6_SHIFT) | (register_to_word(parameter2)<<R7_SHIFT);
                    /* Copying into registers array, parameter1 (r_) with comma and parameter2 (r_) | '_' represents a number between 0 and 7 */
                    strcpy(registers, operand1), strcat(registers, ","), strcat(registers, operand2); /* registers = "r[0-7],r[0-7]"*/
                    insert(&map, map_counter++, registers, binary_machine_code);
                }
                else {
                    /* First parameter */
                    /* Convert to word and shift the bits to parameter 1 */
                    if (is_register(parameter1)){
                        binary_machine_code = register_to_word(parameter1)<<PARAM_1_SIZE_SHIFT;
                    }
                    else if(is_label(st, parameter1)){
                        binary_machine_code = label_to_word(st, parameter1);
                    }
                    else{
                        binary_machine_code = num_to_word(parameter1)<<ARE_SHIFT;
                    }
                    insert(&map, map_counter++, parameter1, binary_machine_code);

                    /* Second parameter */
                    /* Convert to word and shift the bits to parameter 2 */
                    if (is_register(parameter2)){
                        binary_machine_code = register_to_word(parameter2)<<ARE_SHIFT;
                    }
                    else if(is_label(st, parameter2)){
                        binary_machine_code = label_to_word(st, parameter2);
                    }
                    else{
                        binary_machine_code = num_to_word(parameter2);
                    }
                    insert(&map, map_counter++, parameter2, binary_machine_code);
                }
            }
            /* Second group instruction without parameters */
            else{
                /* Converting line to binary code */
                binary_machine_code = bits_calculator(st, instruction, NULL, operand2,NULL,NULL);
                insert(&map, map_counter++, instruction, binary_machine_code);
                /* Destination operand */
                if (strcmp(instruction, "prn")==0 && strchr(operand2, '#')!=NULL){ /* only 'prn' can get a number as destination operand */
                    binary_machine_code = num_to_word(operand2)<<ARE_SHIFT;
                }
                else{
                    binary_machine_code = label_to_word(st, operand2);
                }
                insert(&map, map_counter++, operand2, binary_machine_code);

            }
        }
        else if(third_group_instructions(instruction)){
            /* Converting assembler line to binary code */
            binary_machine_code = bits_calculator(NULL, instruction, NULL, NULL, NULL, NULL);
            insert(&map, map_counter++, instruction, binary_machine_code);
        }
    }

    /* Inserting directives to the hash map */
    rewind(fp_am);
    while (fgets(line, LINE_SIZE, fp_am) != NULL) {
        ptr = line;
        /* Point after label name */
        if(strstr(line, ":")!=NULL){
            ptr = strchr(line, ':') + 1;
        }
        /* Save instruction name */
        instruction = strtok_trimmed(ptr, DELIMITER);
        /* For .string directive */
        if (strcmp(instruction, ".string") == 0) {
            operand1 = strtok_trimmed(NULL, DELIMITER);
            /* Convert each char of the string to word and insert it to hash map */
            for (i = 1; i < strlen(operand1) - 1; i++) {
                binary_machine_code = char_to_word(operand1[i]);
                insert(&map, map_counter++, operand1+i, binary_machine_code);
            }
            /* Insert null terminator to the hash map */
            binary_machine_code = char_to_word('\0');
            insert(&map, map_counter++, "\\0",binary_machine_code);
        }
        /* For .data directive */
        else if (strcmp(instruction, ".data") == 0) {
            /* Save first number */
            operand1 = strtok_trimmed(NULL, COMMA);
            while (operand1 != NULL) {
                /* Convert each number to word and insert it to hash map */
                binary_machine_code = num_to_word(operand1);
                insert(&map, map_counter++, operand1, binary_machine_code);
                /* Save next number */
                operand1 = strtok_trimmed(NULL, COMMA);
            }
        }
    }
    /* Generate object file */
    object_file_generate(filename, map, ic, dc);
    /* Generate entry file (if required) */
    entry_file_generate(filename, st, fp_am);
    /* Generate extern file (if required) */
    extern_file_generate(filename, st, map, map_counter);
}

void extern_file_generate(char *filename, symbol_table *st, hash_map map, int map_counter){
    FILE *fp_ext;
    char *ext_file;
    int numb_of_externs = 0, i;
    symbol_entry *se= get_head(st);

    /* Creating extern file */
    generate_filename(filename, ".ext", &ext_file);
    fp_ext = open_file(ext_file, "w");

    /* Looking for extern & deriving label name from symbol table */
    for (i=START_MEMORY; i<map_counter; i++){
        /* Check for each hash node if it's binary address is an external address */
        if (get_binary_address(get_hash_node(&map, i)) == EXTERNAL) {
            while (se!=NULL) {
                if (strcmp(get_label(se),get_code(get_hash_node(&map, i))) == 0) {
                    /* Print extern label with there decimal address into extern file */
                    fprintf(fp_ext,"%s\t%d\n", get_label(se), get_decimal_address(get_hash_node(&map, i)));
                    numb_of_externs++; /* Count number of externals */
                }
                se = get_next(se);
            }
            se=get_head(st);
        }
    }

    /* Deleting empty extern file */
    if (numb_of_externs == 0){
        delete_file(ext_file, NULL);
    }
    free(ext_file);
    fclose(fp_ext);
}

void entry_file_generate(char *filename, symbol_table *st, FILE *fp_am){
    FILE *fp_ent;
    char *ent_file, line[LINE_SIZE], *entry_label;
    int numb_of_entries = 0;
    symbol_entry *se;

    /* Creating entry file */
    generate_filename(filename, ".ent", &ent_file);
    fp_ent = open_file(ent_file, "w");

    /* Looking for '.entry' directives & checking if exists in the symbol table */
    rewind(fp_am);
    while (fgets(line, LINE_SIZE, fp_am)!=NULL){
        /* Check each line if it contains .entry directive */
        if((entry_label = strstr(line, ".entry"))!=NULL){
            entry_label += strlen(".entry"); /* entry_label points after '.entry' directive */
            trim_whitespace(entry_label); /* Trimming leading and trailing whitespaces */
            se = lookup_label(st, entry_label);
            if (se != NULL){
                /* Print the entry label with it's decimal address into entry file */
                fprintf(fp_ent, "%s\t%d\n", entry_label, get_address(se));
                numb_of_entries++; /* Count number of entry labels */
            }
        }
    }

    /* Deleting empty entry file */
    if (numb_of_entries == 0){
        delete_file(ent_file, NULL);
    }
    free_pointers(1, ent_file);
    fclose(fp_ent);
}

void object_file_generate(char *filename, hash_map map, int ic, int dc){
    FILE *fp_ob;
    char *ob_file, *speical;
    int map_lines = ic+dc, i;
    word binary;

    /* Creating object file */
    generate_filename(filename, ".ob", &ob_file);
    fp_ob = open_file(ob_file, "w");
    /* Print instruction counter and data counter into object file */
    fprintf(fp_ob, "\t%d %d\n", ic-START_MEMORY, dc);

    /* For each word print decimal address and speical representation of it into object file */
    for(i=START_MEMORY; i<map_lines; i++){
        binary = get_binary_code_for_decimal_address(&map, i);
        speical = binary_to_special(binary);
        fprintf(fp_ob, "0%d\t%s\n", i, speical);
        free(speical);
    }

    free_pointers(1, ob_file);
    fclose(fp_ob);
}

char *binary_to_special(word binary_address){
    char special[BITS_PER_WORD+1]; /* +1 for Null terminator */
    word mask = 1 << (BITS_PER_WORD-1); /* initialize mask to the most significant bit */
    int i;

    for (i = 0; i < BITS_PER_WORD; i++, mask >>= 1) { /* shift the mask to right by 1 */
        if (binary_address & mask) {
            special[i] = '/';
            continue;
        }
        special[i] = '.';
    }
    special[BITS_PER_WORD] = '\0';
    return _strdup(NULL, special); /* return a copy of the string */
}

hash_map create_hash_map(){
    hash_map map;
    hash_node *node;
    int i;

    /* Initializing the hash map */
    for (i = 0; i < HASH_MAP_SIZE; i++) {
        node = get_hash_node(&map, i); /* get the i-th node from the hash map */
        set_decimal_address(node, 0);
        set_binary_address(node, 0);
    }
    /* set the decimal address of the first node to START_MEMORY */
    set_decimal_address(get_hash_node(&map, 0), START_MEMORY);

    return map;
}

int hash_function(int decimal_address) {
    return decimal_address % HASH_MAP_SIZE;
}

void insert(struct hash_map *map, int decimal_address, char *code, word binary_address) {
    int index = hash_function(decimal_address);
    hash_node *node = get_hash_node(map, index);

    set_decimal_address(node, decimal_address);
    set_code(node, code);
    set_binary_address(node, binary_address);
}

word get_binary_code_for_decimal_address(struct hash_map *map, int decimal_address) {
    int index = hash_function(decimal_address);
    return get_binary_address(get_hash_node(map, index));
}

void print_all_addresses(hash_map *map, int map_counter) {
    int i;
    hash_node *node;

    /* If the map is empty */
    if(map==NULL){
        return;
    }

    printf("-------------------------------------------------------------\n");
    printf("\t\t\tMAP:\n");
    printf("-------------------------------------------------------------\n");
    printf("Decimal address\t|\tCode \t|\tBinary address\n");
    printf("-------------------------------------------------------------\n");
    for (i = START_MEMORY; i < map_counter; i++) {
        node = get_hash_node(map, i);
        printf("\t%d\t|\t", get_decimal_address(node));
        if (node && get_code(node) && get_code(node)[strlen(get_code(node))-1]=='\"') {
            printf("%c\t|\t", get_code(node)[0]);
        } else {
            printf("%s\t|\t", get_code(node));
        }
        print_word_bits(get_binary_address(node));
    }
}

word label_to_word(symbol_table *st, char *label_name){
    int decimal_address;
    symbol_entry *label_entry;

    /* Looking for the label inside the symbol table */
    label_entry = lookup_label(st, label_name);
    /* If the label is extern label, return only bit 0 on */
    if (get_is_extern(label_entry)){
        return EXTERNAL;
    }
    /* Otherwise get the decimal address related to the label */
    decimal_address = get_address(label_entry);
    /* Shift the bits of the decimal address to be on bits 2-14 and bits 0-1 should be relocatable (10) */
    return ((decimal_address<<ARE_SHIFT) | RELOCATABLE) ;
}

word num_label_register(symbol_table *st, char *str){

    if (is_register(str)){
        /* Addressing method number 3 for registers (11) */
        return REG_ADDR;
    }
    if (is_label(st, str)){
        /* Addressing method number 1 for labels (01) */
        return DIRECT_ADDR;
    }
    /* Addressing method number 0 for numbers (00) */
    return ABSOLUTE;
}

word num_to_word(char *num) {
    int number;
    char *num_ptr;

    /* Pints after '#' */
    if(strstr(num, "#")!=NULL){
        num_ptr = strchr(num, '#')+ strlen("#");
    }
    else{
        num_ptr = num;
    }
    /* Convert num string to number */
    number = atoi(num_ptr);
    /* Positive number */
    if (number >= 0) {
        return (word) number;
    }
        /* Negative number */
    else {
        return (1<<BITS_PER_WORD) | number;
    }
}

word char_to_word(char c) {
    return (word) c;
}

word register_to_word(char *_register) {
    const word register_values[] = {R0, R1, R2, R3, R4, R5, R6, R7};
    int register_index = _register[1] - '0';
    if (register_index >= 0 && register_index < 8) {
        return register_values[register_index];
    }
    return 0;
}

word bits_calculator(symbol_table *st, char *instruction, char *source_operand, char *dest_operand, char *parameter1, char *parameter2){
    word instruction_word, source_word, dest_word, param1_word, param2_word, result;

    instruction_word = source_word = dest_word = param1_word = param2_word = result = 0;
    if(first_group_instructions(instruction)){
        /* Convert instruction to word */
        instruction_word = instruction_to_word(instruction);
        /* Convert source and destination operands to words */
        source_word = num_label_register(st, source_operand);
        dest_word = num_label_register(st, dest_operand);
        /* Combine instruction word (bits 6-9) source operand word (bits 4-5) and destination operand word (bits 2-3) */
        result = instruction_word<<OPCODE_SIZE_SHIFT | source_word<<SRC_OPERAND_SIZE_SHIFT | dest_word<<DEST_OPERAND_SIZE_SHIFT;
    }
    else if(second_group_instructions(instruction)){
        /* Convert instruction to word */
        instruction_word = instruction_to_word(instruction);
        /* Convert destination operand to word */
        dest_word = num_label_register(st, dest_operand);
        if(parameter2!=NULL){
            /* Convert destination operand to addressing method number 2 */
            dest_word = JMP_ADDR;
            /* Convert first parameter to word */
            param1_word = num_label_register(st, parameter1);
            /* Convert second parameter to word */
            param2_word = num_label_register(st, parameter2);
            /* Combine first parameter word (bits 12-13) second parameter word (bits 10-11) instruction word (bits 6-9) and destination operand word (bits 2-3) */
            result = instruction_word<<OPCODE_SIZE_SHIFT | dest_word<<DEST_OPERAND_SIZE_SHIFT | param1_word<<PARAM_1_SIZE_SHIFT | param2_word<<PARAM_2_SIZE_SHIFT;
        }
        else{
            /* Combine instruction word (bits 6-9) and destination word (bits 2-3) */
            result = instruction_word<<OPCODE_SIZE_SHIFT | dest_word<<DEST_OPERAND_SIZE_SHIFT;
        }
    }
    else if (third_group_instructions(instruction)) {
        /* Convert instruction word and shift it to bits 6-9 */
        result = instruction_to_word(instruction)<<OPCODE_SIZE_SHIFT;
    }
    return result;
}

void print_word_bits(word w) {
    int i;
    /* Print the binary representation of the word */
    for (i = BITS_PER_WORD - 1; i >= 0; i--) {
        if (i==1 || i==3 || i==5 || i==9 || i==11){
            putchar(' ');
        }
        printf("%d", (w >> i) & 1);
    }
    printf("\n");
}

word instruction_to_word(char *instruction){
    if (strcmp(instruction, "mov")==0){
        return MOV;
    }
    else if(strcmp(instruction, "cmp")==0){
        return CMP;
    }
    else if(strcmp(instruction, "add")==0){
        return ADD;
    }
    else if(strcmp(instruction, "sub")==0){
        return SUB;
    }
    else if(strcmp(instruction, "not")==0){
        return NOT;
    }
    else if(strcmp(instruction, "clr")==0){
        return CLR;
    }
    else if(strcmp(instruction, "lea")==0){
        return LEA;
    }
    else if(strcmp(instruction, "inc")==0){
        return INC;
    }
    else if(strcmp(instruction, "dec")==0){
        return DEC;
    }
    else if(strcmp(instruction, "jmp")==0){
        return JMP;
    }
    else if(strcmp(instruction, "bne")==0){
        return BNE;
    }
    else if(strcmp(instruction, "red")==0){
        return RED;
    }
    else if(strcmp(instruction, "prn")==0){
        return PRN;
    }
    else if(strcmp(instruction, "jsr")==0){
        return JSR;
    }
    else if(strcmp(instruction, "rts")==0){
        return RTS;
    }
    else{
        return STOP;
    }
}