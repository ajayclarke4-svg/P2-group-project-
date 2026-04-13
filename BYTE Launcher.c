#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* FIX 1: Include windows.h for GetModuleFileName() so base_dir is resolved
   at runtime from the actual .exe location, not the compile-time __FILE__ path */
#ifdef _WIN32
#include <windows.h>
#endif

char base_dir[512];
char cmd[1024];

void clear_screen() {
    /* FIX 6: Support both Windows and Unix instead of hardcoding cls */
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pause_screen() {
    /* FIX 7: Flush any leftover input before blocking, so the user only
       ever needs to press Enter once regardless of prior scanf calls */
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    printf("\nPress Enter to continue....");
    getchar();
}

void build_base_dir() {
    /* FIX 1: Use GetModuleFileName on Windows (runtime exe path) so the
       launcher still works if the exe is moved or shared with someone else.
       Fall back to __FILE__ on other platforms. */
#ifdef _WIN32
    GetModuleFileName(NULL, base_dir, sizeof(base_dir));
#else
    strncpy(base_dir, __FILE__, sizeof(base_dir));
#endif

    /* FIX 3: Guarantee null termination in case the path exactly fills
       the buffer and strncpy did not append a '\0' */
    base_dir[sizeof(base_dir) - 1] = '\0';

    int found = 0;
    for (int i = (int)strlen(base_dir) - 1; i >= 0; i--) {
        if (base_dir[i] == '\\' || base_dir[i] == '/') {
            base_dir[i] = '\0';
            found = 1;
            break;
        }
    }
    if (!found) {
        strncpy(base_dir, ".", sizeof(base_dir));
        base_dir[sizeof(base_dir) - 1] = '\0';
    }
}

int compile_c() {
    /* FIX 5: Check snprintf return value — if the command was truncated
       (return >= sizeof(cmd)) abort early instead of running a broken command.
       FIX 4: Added -lm to link the math library required by Employee.c */
    int written = snprintf(cmd, sizeof(cmd),
        "cd \"%s\" && gcc Main.c Admin.c Employee.c file.c -o program.exe -lm",
        base_dir
    );

    if (written < 0 || written >= (int)sizeof(cmd)) {
        printf("Error: path too long, compile command was truncated.\n");
        return 0;
    }

    printf("Compiling C program...\n");

    int result = system(cmd);
    if (result != 0) {
        printf("\nCompilation FAILED. See errors above.\n");
        /* FIX 2: Removed pause_screen() from here — main() already calls it,
           so leaving it here caused the user to press Enter twice on failure */
        return 0;
    }

    /* FIX 5: Check mkdir command truncation too */
#ifdef _WIN32
    written = snprintf(cmd, sizeof(cmd), "mkdir \"%s\\data\" 2>nul", base_dir);
#else
    written = snprintf(cmd, sizeof(cmd), "mkdir -p \"%s/data\"", base_dir);
#endif

    if (written < 0 || written >= (int)sizeof(cmd)) {
        printf("Error: path too long for mkdir command.\n");
        return 0;
    }
    system(cmd);

    printf("Compilation successful!\n");
    return 1;
}

void run_c() {
    if (compile_c() == 0) return;

    /* FIX 5 & 6: Check snprintf truncation; use correct separator per platform */
#ifdef _WIN32
    int written = snprintf(cmd, sizeof(cmd), "\"%s\\program.exe\"", base_dir);
#else
    int written = snprintf(cmd, sizeof(cmd), "\"%s/program\"", base_dir);
#endif

    if (written < 0 || written >= (int)sizeof(cmd)) {
        printf("Error: path too long to launch program.\n");
        return;
    }
    system(cmd);
}

int main() {
    build_base_dir();

    clear_screen();
    printf("=======================================\n");
    printf("    EMPLOYEE MANAGEMENT SYSTEM (C)\n");
    printf("=======================================\n\n");

    run_c();

    /* FIX 2: Only one pause here — compile_c() no longer calls pause_screen()
       on failure, so this single call handles both success and failure paths */
    pause_screen();
    return 0;
}