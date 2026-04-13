#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// FIX: Include windows.h for GetModuleFileName() so paths are resolved
// at runtime from the actual .exe location, not the compile-time __FILE__ path.
#ifdef _WIN32
#include <windows.h>
#endif

char base_dir[512];
char parent_dir[512];
char cmd[1024];

void clear_screen() {
    // FIX: Hardcoded "cls" only worked on Windows. Added Unix fallback.
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pause_screen() {
    // FIX: Flush input buffer before blocking so one Enter press always works
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    printf("\nPress Enter to continue....");
    getchar();
}

void build_dirs() {
    // FIX: Use GetModuleFileName on Windows so the path comes from the
    // running .exe at runtime. Fall back to __FILE__ on other platforms.
#ifdef _WIN32
    GetModuleFileName(NULL, base_dir, sizeof(base_dir));
#else
    strncpy(base_dir, __FILE__, sizeof(base_dir));
#endif

    // FIX: Guarantee null termination
    base_dir[sizeof(base_dir) - 1] = '\0';

    // Strip filename to get the Python code/ folder path
    for (int i = (int)strlen(base_dir) - 1; i >= 0; i--) {
        if (base_dir[i] == '\\' || base_dir[i] == '/') {
            base_dir[i] = '\0';
            break;
        }
    }

    // Strip one more level to get the ByteCafe/ root for CSV sync
    strncpy(parent_dir, base_dir, sizeof(parent_dir));
    parent_dir[sizeof(parent_dir) - 1] = '\0';

    for (int i = (int)strlen(parent_dir) - 1; i >= 0; i--) {
        if (parent_dir[i] == '\\' || parent_dir[i] == '/') {
            parent_dir[i] = '\0';
            break;
        }
    }
}

void sync_csv() {
    // FIX: Check snprintf truncation before running the copy command
    int written = snprintf(cmd, sizeof(cmd),
#ifdef _WIN32
        "copy \"%s\\C code\\data\\employee_records.csv\" "
        "\"%s\\employee_records.csv\" >nul 2>&1",
#else
        "cp \"%s/C code/data/employee_records.csv\" "
        "\"%s/employee_records.csv\" 2>/dev/null",
#endif
        parent_dir, base_dir
    );
    if (written < 0 || written >= (int)sizeof(cmd)) {
        printf("Error: path too long for sync command.\n");
        return;
    }
    printf("Syncing employee records from C system...\n");
    system(cmd);
}

void run_python() {
    sync_csv();

    printf("Installing dependencies...\n");
    system("pip install colorama --quiet");

    int written = snprintf(cmd, sizeof(cmd),
#ifdef _WIN32
        "cd \"%s\" && py payroll_system.py",
#else
        "cd \"%s\" && python3 payroll_system.py",
#endif
        base_dir
    );
    if (written < 0 || written >= (int)sizeof(cmd)) {
        printf("Error: path too long to launch Python program.\n");
        return;
    }
    system(cmd);
}

int main() {
    build_dirs();

    clear_screen();
    printf("=======================================\n");
    printf("         BYTE CAFE PAYROLL SYSTEM\n");
    printf("=======================================\n\n");

    run_python();

    pause_screen();
    return 0;
}