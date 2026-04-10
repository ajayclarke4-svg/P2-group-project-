#include <stdio.h>
#include <stdlib.h>

#include "file.h"
#include "paths.h"



// Open File
FILE *open_file();

// Close File
void close_file(FILE *file);

void Clean_file(struct employee *empPtr) {
    int clean_file;
    printf("Do you want a clean Document 1 for no and 0 for yes: ");
    scanf("%d", &clean_file);
 
    if (clean_file == 0) {
        FILE *file = fopen(employee_path, "w");
        if (file) fclose(file);
    }
    Add_Employee(empPtr);
}