#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char base_dir[512];
char cmd[1024];

void clear_screen() {
    system("cls");
}

void pause_screen() {
    printf("\nPress Enter to continue....");
    getchar();
    getchar();
}

void build_base_dir() {
    strncpy(base_dir, __FILE__, sizeof(base_dir));
    int found = 0;
    for (int i = strlen(base_dir) - 1; i >= 0; i--) {
        if (base_dir[i] == '\\' || base_dir[i] == '/') {
            base_dir[i] = '\0';
            found = 1;
            break;
        }
    }
    if (!found) {
        strncpy(base_dir, ".", sizeof(base_dir));
    }
}

int compile_c() {

    printf("Closing any running instance of program.exe...\n");
    snprintf(cmd, sizeof(cmd), "taskkill /f /im program.exe >nul 2>&1");
    system(cmd);

    snprintf(cmd, sizeof(cmd),
        "cd \"%s\" && gcc Main.c Admin.c Employee.c file.c -o program.exe",
        base_dir
    );
    printf("Compiling C program...\n");

    int result = system(cmd);
    if (result != 0) {
        printf("\nCompilation FAILED. See errors above.\n");
        pause_screen();
        return 0;
    }


    snprintf(cmd, sizeof(cmd), "mkdir \"%s\\data\" 2>nul", base_dir);
    system(cmd);

    printf("Compilation successful!\n");
    return 1;
}

void run_c() {
    if (compile_c() == 0) return;
    snprintf(cmd, sizeof(cmd), "\"%s\\program.exe\"", base_dir);
    system(cmd);
}

int main() {
    build_base_dir();

    clear_screen();
    printf("=======================================\n");
    printf("    EMPLOYEE MANAGEMENT SYSTEM (C)\n");
    printf("=======================================\n\n");

    run_c();

    pause_screen();
    return 0;
}