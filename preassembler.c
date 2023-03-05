#include "preassembler.h"


void preassembler(FILE *fp, FILE *fp_am){
    macro *head = NULL;
    char line[LINE_SIZE];

    while(fgets(line, LINE_SIZE, fp) != NULL){
        if(empty_line(line)) continue; /* skipping empty lines or comments*/
        remove_comment(line);
        remove_whitespace(line); /* removing leading/ending whitespaces*/
        if(strstr(line, "mcr")!=NULL){ /* new macro */
            read_new_macro(fp, &head, line);
        }
        else if(get_macro_data(head, line)==NULL){
            fprintf(fp_am, "%s", line);
        }
        else{
            fprintf(fp_am, "%s", get_macro_data(head, line));
        }
    }
    free_macro(&head);
}


void read_new_macro(FILE *fp, macro **head, char name[]) {
    char line[LINE_SIZE];
    char *data = NULL, *new_data = NULL;
    int num_lines_data = 0, flag_mcr = on;
    macro *new_node = create_macro();

    /* Extract the macro name */
    if (name_macro(name, new_node->name)==NULL) {
        error_msg(name,  NO_ARGUMENT, "Preassembler " MACRO_NAME_ERROR, NO_ARGUMENT);
    }

    while (flag_mcr && fgets(line, LINE_SIZE, fp) != NULL) {
        if (strstr(line, "endmcr") == NULL) {
            num_lines_data++;
            new_data = realloc(data, (num_lines_data * LINE_SIZE + 1) * sizeof(char)); /* adding one for '\0' */
            if (new_data == NULL) {
                ALLOCATION_ERROR_(NULL, NO_ARGUMENT, name, " in preassembler", NO_ARGUMENT);
                exit(EXIT_FAILURE);
            }

            data = new_data;
            remove_whitespace(line);
            remove_comment(line);
            if(empty_line(line)) continue; /* skipping empty lines */
            if(num_lines_data == 1){
                strcpy(data, line);
            } else{
                strcat(data, line);
            };
        }
        else {
            /* Add null character to end of data string */
            data[num_lines_data * LINE_SIZE - 1] = '\0';
            new_node->data = _strdup(data); /* allocate memory for data and copy data to it */
            flag_mcr = off;
        }
    }

    /* Add new node to beginning of linked list */
    new_node->next = *head;
    *head = new_node;
    free(new_data);
}

macro* create_macro() {
    macro *new_node = (macro *) malloc(sizeof(macro));

    if (new_node == NULL) {
        ALLOCATION_ERROR(NULL, NO_ARGUMENT, "when creating new macro", NO_ARGUMENT);
        exit(EXIT_FAILURE);
    }
    return new_node;
}

char *name_macro(char line[], char *macro_name) {
    char *token = strtok(line, DELIMITER); /* Tokenize the line "mcr" */

    /* Copy the macro name to the output parameter */
    token = strtok(NULL, DELIMITER); /* macro_name */
    remove_whitespace(token); /* Remove any leading or trailing whitespaces */
    if(is_reserved(token) == ERROR){
        RESERVED_NAME_ERROR(NULL, NO_ARGUMENT, "Macro", token, NO_ARGUMENT);
    }
    strcpy(macro_name, token);

    /* Check if the macro name is valid */
    if (strlen(macro_name) == 0) {
        error_msg( NULL, NO_ARGUMENT, INVALID_MACRO_NAME_ERROR, NO_ARGUMENT);
        return NULL;
    }

    return token;
}

char *get_macro_data(macro *head, char name[]) {
    int i=0;
    macro *current = head;

    remove_whitespace(name);
    while (current != NULL) {
        /* compare name with exact macro's name in the linked list.*/
        if (strncmp(current->name, name+i, strlen(current->name)) == 0) {
            return current->data;
        }
        current = current->next;
    }
    return NULL;  /* Macro not found */
}

void free_macro(macro **head_ptr) {
    macro *current = *head_ptr;
    while (current != NULL) {
        macro *next = current->next;
        free(current->data);
        free(current);
        current = next;
    }
}

void macro_print(struct mcr_table *head) {
    /* Prints the name and characters of all macros in the linked list */
    macro *current;
    current = head;
    while (current != NULL) {
        /* print the name of the macro */
        printf("Macro name: \n%s", current->name);
        /* print the characters of the macro */
        printf("Macro characters:\n");
        printf("%s\n", current->data);

        current = current->next;
    }
}