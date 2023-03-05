#include "utils.h"

FILE* open_file(const char* filename, const char* mode) {
    FILE* fp = fopen(filename, mode);
    if (fp == NULL) {
        error_msg(filename, 0, "opening file", 0);
        exit(EXIT_FAILURE);
    }
    return fp;
}

void generate_filename(const char *filename, const char *suffix, char **new_filename) {
    int len = strlen(filename) + strlen(suffix) + 1; /* add 1 for null terminator */
    char *temp_filename = malloc(len * sizeof(char));
    if (temp_filename == NULL) {
        ALLOCATION_ERROR_(filename, NO_ARGUMENT, EMPTY_STRING, "new filename", NO_ARGUMENT);
        exit(EXIT_FAILURE);
    }
    strcpy(temp_filename, filename);
    strcat(temp_filename, suffix);
    *new_filename = temp_filename;
}


void delete_file(const char *file_name, const char *suffix) {
    char file_to_delete[FILENAME_MAX];

    strcpy(file_to_delete, file_name);
    if(suffix != NULL){
        strcat(file_to_delete, suffix);
    }

    if (remove(file_to_delete) != 0) {
        error_msg(file_to_delete, NO_ARGUMENT, DELETE_FILE_ERROR, NO_ARGUMENT);
        exit(EXIT_FAILURE);
    }
}

/*
int search_string_in_file(FILE *fp, const char *string) {
    char buffer[LINE];
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        if (strstr(buffer, string) != NULL) {
            rewind(fp);
            return 1; */
/* true *//*

        }
    }
    rewind(fp);
    return 0; */
/* false *//*

}
*/

/* This function takes an integer count followed by any number of file pointers
   and closes each of them if they are not null. */
void close_files(int count, ...) {
    va_list args;
    int i;
    FILE* fp;

    va_start(args, count); /* Initialize the argument list. */
    /* Loop through each file pointer passed in. */
    for (i = 0; i < count; i++) {
        /* Get the next file pointer from the argument list. */
        fp = va_arg(args, FILE*);

        /* If the file pointer is not null, close it. */
        if (fp != NULL) {
            fclose(fp);
        }
    }

    va_end(args); /* Clean up the argument list. */
}


/* Frees number of pointers */
void free_pointers(int count, ...) {
    int i;
    va_list args;

    va_start(args, count);  /* initialize variable argument list */
    for (i = 0; i < count; i++) {
        void* ptr = va_arg(args, void*); /* get next pointer from argument list */
        free(ptr);
    }
    va_end(args); /* end variable argument list */
}