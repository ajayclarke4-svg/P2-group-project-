#include <stdio.h>
#include <stdlib.h>

#include "file.h"
#include "employee.h"
#include "paths.h"

FILE *open_file();
void close_file(FILE *file);

void Clean_file(struct employee *empPtr) {
    int clean_file;
    printf("Do you want to clean the document? (1 = No, 0 = Yes): ");
    scanf("%d", &clean_file);
 
    if (clean_file == 0) {
        FILE *file = fopen(employee_path, "w");
        if (file) fclose(file);

        write_header();
    }
    Add_Employee(empPtr);
}