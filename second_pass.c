#include "second_pass.h"

void second_pass(symbol_table *st, FILE *fp_am) {
    char line[LINE_SIZE], *ptr, *instruction, *operand1, *operand2, *parameter1, *parameter2;
    word binary_machine_code;
    hash_map map = create_hash_map();
    int i, map_counter=START_MEMORY;


    while (fgets(line, LINE_SIZE, fp_am) != NULL) {
        ptr = line;
        if(strstr(line, ":")!=NULL){
            ptr = strchr(line, ':')+1; /* ptr points after label name */
        }
        /* Getting instruction string */
        instruction = strtok_trimmed(ptr, DELIMITER);

        if(first_group_instructions(instruction)){
            operand1 = strtok_trimmed(NULL, COMMA);
            operand2 = strtok_trimmed(NULL, DELIMITER);
            /* Instruction address with operands */
            binary_machine_code = bits_calculator(st, instruction, operand1, operand2, NULL, NULL);
            insert(&map, map_counter++, binary_machine_code);
            /* If both operands are registers only one new word */
            if(is_register(operand1) && is_register(operand2)){
                binary_machine_code = (register_to_word(operand1)<<R6_SHIFT) | (register_to_word(operand2)<<R7_SHIFT);
                insert(&map, map_counter++, binary_machine_code);
            }
            else {
                /* Source address */
                if (is_register(operand1)){
                    binary_machine_code = (register_to_word(operand1)<<R6_SHIFT);
                }
                else if(is_label(operand1, st)){
                    binary_machine_code = label_to_word(st, operand1);
                }
                else{
                    binary_machine_code = num_to_word(operand1); /**maybe need to be shifted ARE*/
                }
                insert(&map, map_counter++, binary_machine_code);

                /* Destination address */
                if (is_register(operand2)){
                    binary_machine_code = register_to_word(operand2);
                }
                else if(is_label(operand2, st)){
                    binary_machine_code = label_to_word(st, operand2);
                }
                insert(&map, map_counter++, binary_machine_code);
            }
        }

        else if (second_group_instructions(instruction)){
            operand2 = strtok_trimmed(NULL, DELIMITER);

            /* Jump instruction use parameter-based addressing */
            if(strchr(operand2, '(')!=NULL){
                operand2 = strtok_trimmed(operand2, "(");
                parameter1 = strtok_trimmed(NULL, COMMA);
                parameter2 = strtok_trimmed(NULL, ")");
                /* instruction with label and parameters */
                binary_machine_code = bits_calculator(st, instruction, NULL, operand2, parameter1, parameter2);
                insert(&map, map_counter++, binary_machine_code);

                /* label address */
                binary_machine_code = label_to_word(st, operand2);
                insert(&map, map_counter++, binary_machine_code);

                /* First and Second parameters */
                if (is_register(parameter1) && is_register(parameter2)){
                    binary_machine_code = (register_to_word(parameter1)<<R6_SHIFT) | (register_to_word(parameter2)<<R7_SHIFT);
                    insert(&map, map_counter++, binary_machine_code);

                }
                else {
                    /* First parameter */
                    if (is_register(parameter1)){
                        binary_machine_code = register_to_word(parameter1)<<PARAM_1_SIZE_SHIFT;
                    }
                    else if(is_label(parameter1, st)){
                        binary_machine_code = label_to_word(st, parameter1);
                    }
                    else{
                        binary_machine_code = num_to_word(parameter1)<<ARE_SHIFT;
                    }
                    insert(&map, map_counter++, binary_machine_code);

                    /* Second parameter */
                    if (is_register(parameter2)){
                        binary_machine_code = register_to_word(parameter2)<<ARE_SHIFT;
                    }
                    else if(is_label(parameter2, st)){
                        binary_machine_code = label_to_word(st, parameter2);
                    }
                    else{
                        binary_machine_code = num_to_word(parameter2);
                    }
                    insert(&map, map_counter++, binary_machine_code);
                }
            }
            /* Second group instruction without parameters */
            else{
                /* Instruction with operand */
                binary_machine_code = bits_calculator(st, instruction, NULL, operand2,NULL,NULL);
                insert(&map, map_counter++, binary_machine_code);

                /* Source operand */
                if (strcmp(instruction, "prn")==0 && strchr(operand2, '#')!=NULL){
                    binary_machine_code = num_to_word(operand2)<<ARE_SHIFT;
                }
                else{
                    binary_machine_code = label_to_word(st, operand2);
                }
                insert(&map, map_counter++, binary_machine_code);

            }
        }

        else if(third_group_instructions(instruction)){
            binary_machine_code = bits_calculator(NULL, instruction, NULL, NULL, NULL, NULL);
            insert(&map, map_counter++, binary_machine_code);
        }

    }

    rewind(fp_am);
    /* Adding data directives to the map */
    while (fgets(line, LINE_SIZE, fp_am) != NULL) {

        ptr = line;
        if(strstr(line, ":")!=NULL){
            ptr = strchr(line, ':') + 1; /* ptr points after label name */
        }
        instruction = strtok_trimmed(ptr, DELIMITER);

        if (strcmp(instruction, ".string") == 0) {
            operand1 = strtok_trimmed(NULL, DELIMITER);
            for (i = 1; i < strlen(operand1) - 1; i++) {
                binary_machine_code = char_to_word(operand1[i]);
                insert(&map, map_counter++, binary_machine_code);
            }
            binary_machine_code = char_to_word('\0');
            insert(&map, map_counter++, binary_machine_code);
        }
        else if (strcmp(instruction, ".data") == 0) {
            operand1 = strtok_trimmed(NULL, COMMA);
            while (operand1 != NULL) {
                binary_machine_code = num_to_word(operand1);
                insert(&map, map_counter++, binary_machine_code);
                operand1 = strtok_trimmed(NULL, COMMA);
            }
        }
    }

    /* Prints the map */
    /*print_all_addresses(&map, map_counter);*/
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
    int i;

    /* If the map is empty */
    if(map==NULL){
        return;
    }

    printf("---------------------------------------\n");
    printf("\t\tMAP:\n");
    printf("---------------------------------------\n");
    printf("Decimal address\t|\tBinary address\n");
    printf("---------------------------------------\n");
    for (i = START_MEMORY; i < map_counter; i++) {
        printf("\t%d\t|\t", map->nodes[i].decimal_address);
        print_word_bits(map->nodes[i].binary_address);
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
    return ((decimal_address<<ARE_SHIFT)|RELOCATABLE) ;
}

word num_label_register(symbol_table *st, char *str){

    if (is_register(str)){
        return REG_ADDR;
    }
    if (is_label(str, st)){
        return DIRECT_ADDR;
    }
    /* For number */
    return ABSOLUTE;
}


word num_to_word(char *num) {
    int number;
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
        instruction_word = instruction_to_word(instruction);

        source_word = num_label_register(st, source_operand);
        dest_word = num_label_register(st, dest_operand);

        result = instruction_word<<OPCODE_SIZE_SHIFT | source_word<<SRC_OPERAND_SIZE_SHIFT | dest_word<<DEST_OPERAND_SIZE_SHIFT;
    }
    else if(second_group_instructions(instruction)){
        instruction_word = instruction_to_word(instruction);
        dest_word = num_label_register(st, dest_operand);
        if(parameter2!=NULL){
            dest_word = JMP_ADDR;
            param1_word = num_label_register(st, parameter1);
            param2_word = num_label_register(st, parameter2);
            result = instruction_word<<OPCODE_SIZE_SHIFT | dest_word<<DEST_OPERAND_SIZE_SHIFT
                    | param1_word<<PARAM_1_SIZE_SHIFT | param2_word<<PARAM_2_SIZE_SHIFT;
        }
        else{
            result = instruction_word<<OPCODE_SIZE_SHIFT | dest_word<<DEST_OPERAND_SIZE_SHIFT;
        }
    }
    else if (third_group_instructions(instruction)) {
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