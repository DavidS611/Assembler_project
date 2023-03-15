#include "assembler.h"


void assembler(int number_of_arg, char *file[]){
    FILE *fp=NULL, *fp_am=NULL;
    char *as_filename, *expanded_macro_file=NULL;
    int i=1;

    /* Check if there is at least one command line argument.
     * If not, print an error message and exit the program. */
    if (number_of_arg == 1) {
        error_msg(NULL, NO_ARGUMENT, NO_ARGUMENT, 1, "no file detected");
        exit(EXIT_FAILURE);
    }

    /* Loop through each file argument */
    while(number_of_arg>i){
        /* Generate filename with the suffix of '.as' */
        generate_filename(file[i], ".as", &as_filename);
        if (as_filename==NULL){
            exit(EXIT_FAILURE);
        }

        /* Opening the file.as in read only mode */
        fp = open_file(as_filename, "r");
        if (fp==NULL){
            free(as_filename);
            exit(EXIT_FAILURE);
        }

        /* Generate filename with the suffix of '.am' */
        generate_filename(file[i], ".am", &expanded_macro_file);
        if (expanded_macro_file==NULL){
            free_pointers(2, as_filename, fp);
            exit(EXIT_FAILURE);
        }
        /* Opening the file.am in reading and writing mode */
        fp_am = open_file(expanded_macro_file, "w+");
        if (fp_am==NULL){
            free_pointers(3, as_filename, fp, expanded_macro_file);
            exit(EXIT_FAILURE);
        }

        /* Expanding the macros inside the original file: <file>.as */
        preassembler(as_filename, fp, fp_am);

        /* First pass: creating symbol table & hash map in the second pass */
        first_pass(file[i], fp_am);

        /* Deleting the expanded macro file */
        delete_file(expanded_macro_file, NULL);

        i++; /* Continue to the next file */

        /* Freeing memory & closing files */
        free_pointers( 2, as_filename, expanded_macro_file);
        close_files(2, fp, fp_am);
    }
}
