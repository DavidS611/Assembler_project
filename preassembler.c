#include "preassembler.h"

void preassembler(char *as_filename, FILE *fp, FILE *fp_am){
    macro *head = NULL;
    char line[LINE_SIZE], line_copy[LINE_SIZE], *mcr_token;

    /* Loop through each line in the input file */
    while(fgets(line, LINE_SIZE, fp) != NULL){

        /* skipping empty lines or comments*/
        if(empty_line(line)) continue;
        /* removing comments from the line */
        remove_comment(line);
        /* Trim leading and trailing whitespace */
        trim_whitespace(line);

        /* Copy the line to use strtok() */
        strcpy(line_copy, line);
        /* Get the first token in the line */
        mcr_token = strtok(line_copy, DELIMITER);
        /* Trim leading and trailing whitespace from the token */
        trim_whitespace(mcr_token);
        /* Check if the token indicates a new macro */
        if(strcmp(mcr_token, "mcr")==0){
            /* Read a new macro, the macro's name and the macro's data */
            read_new_macro(as_filename, fp, &head, line);
        }
        else if(get_macro_data(head, line)==NULL){
            /* Write the line to the output file if it is not part of a macro */
            fprintf(fp_am, "%s\n", line);
        }
        else{
            /* If it is a macro name, write the macro data to the output file instead of macro name*/
            fprintf(fp_am, "%s", get_macro_data(head, line));
        }
    }
    /* Free the memory of all macros in the list */
    free_macro(&head);
}

/* Reads a new macro from the input file and adds it to the macro list */
void read_new_macro(char *as_filename, FILE *fp, macro **head, char macro_name[]) {
    char line[LINE_SIZE], line_copy[LINE_SIZE];
    char *data = NULL, *mcr_name, *extra_chars, *endmcr_token;
    int num_lines_data = 1, flag_mcr = on;
    macro *new_node;

    /* Create a new macro node */
    new_node = create_macro(as_filename);

    /* Parse the macro name from the input string */
    mcr_name = strtok(macro_name, DELIMITER); /* should be 'mcr' because of the if condition on preassembler.c */
    mcr_name = strtok(NULL, DELIMITER); /* macro name */

    /* Check if the macro name is empty. If it is, print an error message and returns. */
    if(mcr_name == NULL){
        error_msg(as_filename, NO_ARGUMENT, NO_ARGUMENT,
                  1, "Invalid macro name, macro name must contain at least one character.");
        return;
    }
    /* Checks if the macro name is a reserved name of the system, and if so, prints an error message and returns. */
    if(is_reserved(mcr_name)){
        error_msg(as_filename, NO_ARGUMENT, NO_ARGUMENT,
                  3,"Macro name is a reserved name of the system \"", mcr_name, "\"");
        return;
    }
    /* Check if the macro name is already exists. if so print an error message. */
    if(check_macro_name(mcr_name, *head)){
        error_msg(as_filename, NO_ARGUMENT, NO_ARGUMENT,
                  3,"Macro name already exists: \"", mcr_name, "\".");

    }

    trim_whitespace(mcr_name); /* trim leading and trailing whitespaces from macro name */
    new_node->name = _strdup(as_filename,mcr_name); /* Allocate memory for the macro name and copy it to the new macro struct name */
    extra_chars = strtok(NULL, DELIMITER); /* Chars after macro name */
    if(extra_chars != NULL){ /* If there are extra chars after macro name , print an error message */
        error_msg(as_filename, NO_ARGUMENT, NO_ARGUMENT,
                  5, "Found extra chars after macro name \"", mcr_name, "\": \"", extra_chars, "\"");
    }

    /* Read the macro's data until the 'endmcr' token is encountered */
    while (flag_mcr && fgets(line, LINE_SIZE, fp) != NULL) {
        if(empty_line(line)) continue; /* skipping empty lines */
        remove_comment(line); /* remove any comments from the line */
        trim_whitespace(line); /* trim leading and trailing whitespace */
        strcpy(line_copy, line);
        endmcr_token = strtok(line_copy, DELIMITER);

        /* Check if the line is part of the macro's data */
        if (strcmp(endmcr_token, "endmcr")!=0) {
            /* Count each line of new data, to allocate memory for*/
            num_lines_data++;
            /* Allocate memory for the new data, each data line is maximum 81 chars including '\0' */
            data = realloc(data, (num_lines_data * LINE_SIZE) * sizeof(char));

            /* Check if memory reallocation succeeded for macro data, and exit with an error message if not. */
            if (data == NULL) {
                error_msg(as_filename, NO_ARGUMENT, NO_ARGUMENT, 3,
                          "Failed to allocate memory for macro data of macro name \"", mcr_name, "\"");
                exit(EXIT_FAILURE);
            }

            /* If this is the first line of the macro's data, initialize the data with this line and add a new line to the end.
             * Otherwise, append the new macro's data to the existing data and add a new line to it. */
            if(num_lines_data==2) { /* Check if it is the first macro's data line */
                strcpy(data, line);
                strcat(data, "\n");
            } else{
                /* Append the line to the existing data and add a new line to it */
                strcat(data, line);
                strcat(data, "\n");
            }
        }
        /* Encountered endmcr token, in other words endmcr_token equal to 'endmcr' string */
        else {
            data[num_lines_data * LINE_SIZE - 1] = '\0'; /* Add null character to end of data string */
            new_node->data = _strdup(as_filename, data); /* Allocate memory for the new node's data and copy the macro's data to it */
            flag_mcr = off; /* Set flag to exit the loop */

            /* Check for extra chars after endmcr token, and if there is print an error */
            extra_chars = strtok(NULL, DELIMITER);
            if(extra_chars!=NULL){
                /* error unwanted chars after 'endmcr' excluding whitespaces*/
                error_msg(as_filename, NO_ARGUMENT, NO_ARGUMENT,
                          3, "Found extra chars after \'endmcr\' token for macro name \"", mcr_name, "\".");
            }
        }
    }

    /* Add new node to the beginning of linked list */
    new_node->next = *head;
    *head = new_node;
    free(data);
}

macro* create_macro(char *as_filename) {
    macro *new_node = (macro *) malloc(sizeof(macro));

    /* Check if memory allocation for a new node failed. If so, print an error message, and exit the program. */
    if (new_node == NULL) {
        error_msg(as_filename, NO_ARGUMENT, NO_ARGUMENT,
                  1, "Failed to allocate memory for new macro.");
        exit(EXIT_FAILURE);
    }
    return new_node;
}

char *get_macro_data(macro *head, char name[]) {
    macro *current = head;

    trim_whitespace(name); /* Remove leading and trailing whitespaces */
    while (current != NULL) {
        /* Compare name with exact macro's name in the linked list. if found return the macro's data */
        if (strcmp(current->name, name) == 0) {
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
        free(current->name); /* Free the memory allocated for macro's name */
        free(current->data); /* Free the memory allocated for macro's data */
        free(current);  /* Free the memory allocated for the macro struct itself */
        current = next;
    }
}

void macro_print(macro *head) {
    /* Prints the name and characters of all macros in the linked list */
    macro *current;
    current = head;
    while (current != NULL) {
        /* print the name of the macro */
        printf("Macro name: \n%s\n", current->name);
        /* print the characters of the macro */
        printf("Macro characters:\n");
        printf("%s", current->data);

        current = current->next;
    }
}

int check_macro_name(char *mcr_name, macro *head){
    macro *curr=head;

    while(curr!=NULL){
        if(strcmp(curr->name, mcr_name)==0)
            return ERROR;
        curr = curr->next;
    }
    return NO_ERROR;
}