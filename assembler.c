
#include "assembler.h"


void assembler(int number_of_arg, char *file[]){
    FILE *fp=NULL, *fp_am=NULL;
    char *as_filename, *expanded_macro_file=NULL;
    int i=1;

    if (number_of_arg == 1) {
        error_msg(NULL, NO_ARGUMENT, NO_ARGUMENT, 1, "no file detected");
        exit(EXIT_FAILURE);
    }

    while(number_of_arg>i){
        generate_filename(file[i], ".as", &as_filename);
        fp = open_file(as_filename, "r");
        generate_filename(file[i], ".am", &expanded_macro_file);
        fp_am = open_file(expanded_macro_file, "w+");

        preassembler(as_filename, fp, fp_am);

        first_pass(file[i], fp_am);

        delete_file(expanded_macro_file, NULL);

        i++;
        free_pointers( 2, as_filename, expanded_macro_file);
        close_files(2, fp, fp_am);
    }
}
