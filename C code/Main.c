#include <stdio.h>
#include <stdlib.h>

#include "employee.h"
#include "admin.h"
#include "file.h"
#include "paths.h"

#define BASE_DIR "data/"

char employee_path[100];
char temp_path[100];
char admin_path[100];

void init_paths() {
    snprintf(employee_path, sizeof(employee_path), "%semployee_records.csv", BASE_DIR);
    snprintf(temp_path,     sizeof(temp_path),     "%stemp.csv",             BASE_DIR);
    snprintf(admin_path,    sizeof(admin_path),     "%sadmin_records.txt",    BASE_DIR);
}

int main(){
    
    init_paths();
    write_header();
    Main_menu();
    return 0;
}