#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RED "\033[91m"
#define GREEN "\033[92m"
#define ENDC "\033[0m"

void compare_files(char *file1, char *file2) {
    FILE *f1 = fopen(file1, "r");
    FILE *f2 = fopen(file2, "r");

    if (f1 == NULL || f2 == NULL) {
        printf("Error: Could not open file(s).\n");
        exit(1);
    }

    int difference_detected = 0;
    char line1[100], line2[100];
    int line_num = 0;

    while (fgets(line1, sizeof(line1), f1) != NULL && fgets(line2, sizeof(line2), f2) != NULL) {
        line_num++;

        if (strcmp(line1, line2) != 0) {
            difference_detected = 1;
            printf("Files '%s' and '%s' %sdiffer%s:\n", file1, file2, RED, ENDC);
            printf("\"%s\" \t \"%s\"\n", strtok(line1, "\n"), strtok(line2, "\n"));
        }
    }

    if (!difference_detected) {
        printf("Files '%s' and '%s' are %sidentical%s.\n", file1, file2, GREEN, ENDC);
    }

    fclose(f1);
    fclose(f2);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s file1 file2\n", argv[0]);
        return 1;
    }

    char *file1 = argv[1];
    char *file2 = argv[2];

    compare_files(file1, file2);

    return 0;
}

