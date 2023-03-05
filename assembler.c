
#include "assembler.h"


void assembler(int number_of_arg, char *file[]){
    FILE *fp=NULL, *fp_am=NULL;
    char *expanded_macro_file=NULL;
    int i=1, instruction_counter=0, data_counter=0;
    symbol_table *symtab = NULL;

    if (number_of_arg == 1) {
        error_msg(NULL, NO_ARGUMENT, FILE_DETECT_ERROR, NO_ARGUMENT);
        exit(EXIT_FAILURE);
    }

    while(number_of_arg>i){
        fp = open_file(file[i], "r");
        generate_filename(file[i], ".am", &expanded_macro_file);
        fp_am = open_file(expanded_macro_file, "w+");

        preassembler(fp, fp_am);

        /*generate_filename(file[i], ".ob", files_name);
        fp_ob = open_file(files_name, "w");
        if(search_string_in_file(fp, ".entry")){
            generate_filename(file[i], ".ent", files_name);
            fp_ent = open_file(files_name, "w");
        }
        else
            fp_ent = NULL;
        if(search_string_in_file(fp, ".extern")){
            generate_filename(file[i], ".ext", files_name);
            fp_ext = open_file(files_name, "w");
        }
        else
            fp_ext = NULL;
        */
        rewind(fp_am);
        symtab = first_pass(file[i], fp_am, &instruction_counter, &data_counter);
        /*if(symtab !=NULL)
        second_pass();*/

        delete_file(file[i], ".am");
        i++;
    }
    free_pointers( 1, expanded_macro_file);
    close_files(2, fp, fp_am);
}
