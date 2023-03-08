#include "second_pass.h"

void second_pass(symbol_table *st, FILE *fp_am) {
    char line[LINE_SIZE], *ptr, *instruction, *operand1, *operand2, *parameter1, *parameter2;
    word binary_machine_code;
    hash_map map = create_hash_map();
    int i, map_counter=0;


    while (fgets(line, LINE_SIZE, fp_am) != NULL) {
        ptr = line;
        if(strstr(line, ":")!=NULL){
            ptr = strchr(line, ':')+1; /* ptr points after label name */
        }

        instruction = strtok(ptr, DELIMITER);

        if(first_group_instructions(instruction)){
            operand1 = strtok(NULL, COMMA);
            operand2 = strtok(NULL, DELIMITER);
            trim_whitespace(operand1), trim_whitespace(operand2);
            /* Instruction address with operands */
            map.nodes[map_counter].decimal_address = START_MEMORY + map_counter;
            map.nodes[map_counter++].binary_address = bits_calculator(NULL, instruction, operand1, operand2, NULL, NULL);
            /* If both operands are registers only one new word */
            if(is_register(operand1) && is_register(operand2)){
                map.nodes[map_counter].decimal_address = START_MEMORY + map_counter;
                binary_machine_code = (register_to_word(operand1)<<8) | (register_to_word(operand2)<<2);
                 map.nodes[map_counter++].binary_address = binary_machine_code;
            }
            else {
                /* Source address */
                map.nodes[map_counter].decimal_address = START_MEMORY + map_counter;
                if (is_register(operand1)){
                    binary_machine_code = (register_to_word(operand1)<<8);
                }
                else if(is_label(operand1, st)){
                    binary_machine_code = label_to_word(st, operand1);
                }
                else{
                    binary_machine_code = num_to_word(operand1);
                }
                map.nodes[map_counter++].binary_address = binary_machine_code;

                /* Destination address */
                map.nodes[map_counter].decimal_address = START_MEMORY + map_counter;
                if (is_register(operand2)){
                    binary_machine_code = register_to_word(operand2);
                }
                else if(is_label(operand2, st)){
                    binary_machine_code = label_to_word(st, operand2);
                }
                map.nodes[map_counter++].binary_address = binary_machine_code;
            }
        }

        else if (second_group_instructions(instruction)){
            operand1 = strtok(NULL, DELIMITER);

            /* Jump instruction use parameter-based addressing */
            if(strchr(operand1, '(')!=NULL){
                operand1 = strtok(operand1, "(");
                parameter1 = strtok(NULL, COMMA);
                parameter2 = strtok(NULL, ")");
                map.nodes[map_counter].decimal_address = START_MEMORY + map_counter;
                /* instruction with label and parameters */
                map.nodes[map_counter++].binary_address = bits_calculator(st, instruction, operand1, NULL, parameter1, parameter2);
                /* label address */
                map.nodes[map_counter].decimal_address = START_MEMORY + map_counter;
                map.nodes[map_counter++].binary_address = label_to_word(st, operand1);
                /* First and Second parameters */
                if (is_register(parameter1) && is_register(parameter2)){
                    map.nodes[map_counter].decimal_address = START_MEMORY + map_counter;
                    binary_machine_code = (register_to_word(parameter1)<<8) | (register_to_word(parameter2)<<2);
                    map.nodes[map_counter++].binary_address = binary_machine_code; /* first and second registers */
                }
                else {
                    /* First parameter */
                    if (is_register(parameter1)){
                        binary_machine_code = register_to_word(parameter1)<<8;
                    }
                    else if(is_label(parameter1, st)){
                        binary_machine_code = label_to_word(st, parameter1);
                    }
                    else{
                        binary_machine_code = num_to_word(parameter1)<<2;
                    }
                    map.nodes[map_counter].decimal_address = START_MEMORY + map_counter;
                     map.nodes[map_counter++].binary_address = binary_machine_code; /* first parameter */

                    /* Second parameter */
                    if (is_register(parameter2)){
                        binary_machine_code = register_to_word(parameter2)<<2;
                    }
                    else if(is_label(parameter2, st)){
                        binary_machine_code = label_to_word(st, parameter2);
                    }
                    else{
                        binary_machine_code = num_to_word(parameter2);
                    }
                    map.nodes[map_counter].decimal_address = START_MEMORY + map_counter;
                     map.nodes[map_counter++].binary_address = binary_machine_code; /* second parameter */
                }
            }
            /* Second group instruction without parameters */
            else{
                operand2 = strtok(NULL, DELIMITER);
                /* Instruction with operand */
                map.nodes[map_counter].decimal_address = START_MEMORY + map_counter;
                map.nodes[map_counter++].binary_address = bits_calculator(st, instruction, operand1, NULL,NULL,NULL);
                /* Source operand */
                map.nodes[map_counter].decimal_address = START_MEMORY + map_counter;
                if (strcmp(instruction, "prn")==0 && strchr(operand1, '#')!=NULL){
                    map.nodes[map_counter++].binary_address = num_to_word(operand1)<<ARE_SHIFT;
                }
                else{
                    map.nodes[map_counter++].binary_address = label_to_word(st, operand1);
                }
            }
        }

        else if(third_group_instructions(instruction)){
            map.nodes[map_counter].decimal_address = START_MEMORY + map_counter;
            map.nodes[map_counter++].binary_address = bits_calculator(NULL, instruction, NULL, NULL, NULL, NULL);
        }

    }

    rewind(fp_am);
    /* Adding data directives to the map */
    while (fgets(line, LINE_SIZE, fp_am) != NULL) {

        ptr = line;
        if(strstr(line, ":")!=NULL){
            ptr = strchr(line, ':') + 1; /* ptr points after label name */
        }
        instruction = strtok(ptr, DELIMITER);

        if (strcmp(instruction, ".string") == 0) {
            operand1 = strtok(NULL, DELIMITER);
            for (i = 1; i < strlen(operand1) - 1; i++) {
                map.nodes[map_counter].decimal_address = START_MEMORY + map_counter;
                map.nodes[map_counter++].binary_address = char_to_word(operand1[i]);
            }
            map.nodes[map_counter].decimal_address = START_MEMORY + map_counter;
            map.nodes[map_counter++].binary_address = char_to_word('\0');
        }
        else if (strcmp(instruction, ".data") == 0) {
            operand1 = strtok(NULL, COMMA);
            while (operand1 != NULL) {
                map.nodes[map_counter].decimal_address = START_MEMORY + map_counter;
                map.nodes[map_counter++].binary_address = num_to_word(operand1);
                operand1 = strtok(NULL, COMMA);
            }
        }
    }

    /* Prints the map */
    print_all_addresses(&map, map_counter);
}

word get_word_data(char* label, symbol_table* sym_table) {
    symbol_entry* entry = get_label(label, sym_table);

    if (entry->is_extern) {
        /* external symbol */
        return 0x1;
    }
    return (word)((entry->address << 2) | RELOCATABLE);
}

hash_map create_hash_map(){
    hash_map map;
    int i;

    for (i = 0; i < HASH_MAP_SIZE; i++) {
        map.nodes[i].decimal_address = 0;
        map.nodes[i].binary_address = 0;
    }

    map.nodes[0].decimal_address = START_MEMORY;

    return map;
}

int hash_function(int decimal_address) {
    return decimal_address % HASH_MAP_SIZE;
}

void insert(struct hash_map *map, int decimal_address, unsigned short int binary_address) {
    int index = hash_function(decimal_address);
    map->nodes[index].decimal_address = decimal_address;
    map->nodes[index].binary_address = binary_address;
}

unsigned short int get_binary_code_for_decimal_address(struct hash_map *map, int decimal_address) {
    int index = hash_function(decimal_address);
    return map->nodes[index].binary_address;
}

void print_all_addresses(hash_map *map, int map_counter) {
    int i, j, bit;

    /* If the map is empty */
    if(map==NULL){
        return;
    }

    printf("---------------------------------------\n");
    printf("\t\tMAP:\n");
    printf("---------------------------------------\n");
    printf("Decimal address\t|\tBinary address\n");
    printf("---------------------------------------\n");
    for (i = 0; i < map_counter; i++) {
        printf("\t%d\t|\t", map->nodes[i].decimal_address);
        for (j = 13; j >= 0; j--) {
            bit = (map->nodes[i].binary_address >> j) & 1;
            printf("%d", bit);
        }
        printf("\n");
    }
}

word label_to_word(symbol_table *st, char *label_name){
    int decimal_address;
    symbol_entry *label_entry;

    label_entry = get_label(label_name, st);
    decimal_address = label_entry->address;
    if (label_entry->is_extern==true){
        return EXTERNAL;
    }
    return ((decimal_address<<2)|RELOCATABLE) ;
}

word num_label_register(symbol_table *st, char *str){

    if (is_register(str)){
        return REG_ADDR;
    }
    if (is_label(str, st)){
        return LABEL_ADDR;
    }
    /* For number */
    return ABSOLUTE;
}


word num_to_word(char *num) {
    int i, number;
    int leading_zeros = 0;
    char *num_ptr;

    if(strstr(num, "#")!=NULL){
        num_ptr = strchr(num, '#')+ strlen("#");
    }
    else{
        num_ptr = num;
    }

    number = atoi(num_ptr);
    /* Positive number */
    if (number >= 0) {
        return (word) number;
    }
        /* Negative number */
    else {
        word w = (word) ((1 << BITS_PER_WORD) + number);
        /* count leading zeros */
        while (w >> leading_zeros == 0) {
            leading_zeros++;
        }
        for (i = BITS_PER_WORD - 1; i >= leading_zeros; i--) {
            (w >> i) & 1;
        }
        return w;
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
        instruction_word = instruction_to_word(instruction);

        if (is_register(source_operand)){
            source_word = REG_ADDR;
        }
        /* Otherwise if it doesn't have '#' then it's a label name, and label name is RELOCATABLE */
        else if(strchr(source_operand, '#')==NULL){
            source_word = RELOCATABLE;
        }

        if (is_register(dest_operand)){
            dest_word = REG_ADDR;
        }
            /* Label */
        else {
            dest_word = RELOCATABLE;
        }

        result = instruction_word<<6 | RELOCATABLE<<4 | dest_word<<2;
    }
    else if(second_group_instructions(instruction)){
        instruction_word = instruction_to_word(instruction);
        source_word = num_label_register(st, source_operand);
        if(parameter2!=NULL){
            param1_word = num_label_register(st, parameter1);
            param2_word = num_label_register(st, parameter2);
            result = instruction_word<<OPCODE_SIZE_SHIFT | source_word<<SRC_OPERAND_SIZE_SHIFT
                    | param1_word<<PARAM_1_SIZE_SHIFT | param2_word<<PARAM_2_SIZE_SHIFT;
        }
        else{
            result = instruction_word<<OPCODE_SIZE_SHIFT | source_word<<SRC_OPERAND_SIZE_SHIFT;
        }
    }
    else if (third_group_instructions(instruction)) {
        if (strcmp(instruction, "stop") == 0) {
            result = (STOP << 6);
        } else if (strcmp(instruction, "rts") == 0) {
            result = (RTS << 6);
        }
    }

    return result;
}

void print_word_bits(word w) {
    int i;
    /* Print the binary representation of the word */
    for (i = BITS_PER_WORD - 1; i >= 0; i--) {
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
    else if(strcmp(instruction, "stop")==0){
        return STOP;
    }
    return 0;
}