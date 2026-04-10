#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "admin.h"
#include "file.h"
#include "paths.h"

void Main_menu() {
    struct Admin admin;
    struct employee emp;
    int choice;

    if(Admin_page(&admin) == 0){
        printf("Exiting Employee Management System. Goodbye!\n");
        exit(0);
    }

    do {
        printf("\n\n\n================EMPLOYEE MANAGEMENT SYSTEM================\n\n\n");
        printf("                       1. Add Employee\n");
        printf("                       2. List Employees\n");
        printf("                       3. Find Employee\n");
        printf("                       4. Update Employee Record\n");
        printf("                       5. Delete Employee Record\n");
        printf("                       6. Clean Employee Records\n");
        printf("                       7. Exit\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1:
                Add_Employee(&emp);
                break;
            case 2:
                list_emplyees(&emp);
                break;
            case 3:    
                find_employee(&emp);
                break;      
            case 4 :
                Update_Record(&emp);    
                break;
            case 5:  
                Delete_Record(&emp);
                break;      
            case 6:
                Clean_file(&emp);
                break;
            case 7: 
                printf("Exiting Employee Management System. Goodbye!\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }  while (choice != 6);

}

int Admin_page(struct Admin *adminPtr) {
    int choice;
    printf("\n\n\n=====ADMIN PAGE=====\n\n\n");
    printf("1. Sign in Page\n");
    printf("2. Sign up Page\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
 
    if (choice == 1) {
        char line[200];
        char storeUsername[40];
        char storepassword[40];
        int  attempts = 1000;
 
        while (attempts > 0) {
            printf("\n\n\n=====ADMIN LOGIN=====\n\n\n");
            printf("Enter Username: ");
            scanf("%39s", storeUsername);
            printf("Enter Password: ");
            scanf("%39s", storepassword);
 
            for (int i = 0; storepassword[i]; i++)
                storepassword[i] = toupper(storepassword[i]);
 
            FILE *file = fopen(admin_path, "r");
            if (file == NULL) {
                printf("Error: Admin file not found.\n");
                return 0;
            }
 
            while (fgets(line, sizeof(line), file) != NULL) {
                char storedUser[40], storedPass[40];
                sscanf(line, "%[^,],%39s", storedUser, storedPass);
 
                char decodedPass[40];
                Unmask_password(storedPass, decodedPass);
 
                if (strcmp(storedUser, storeUsername) == 0 &&
                    strcmp(decodedPass, storepassword) == 0) {
                    printf("\n\n---ACCESS GRANTED!!---\n\n");
                    fclose(file);
                    return 1;
                }
            }
            fclose(file);
 
            attempts--;
            printf("\nACCESS DENIED!!\n");
            if (attempts > 0)
                printf("Remaining attempts: %d\n", attempts);
            else
                printf("No attempts left!!\n\n---ACCESS BLOCKED!!!---\n\n");
        }
        return 1;
    }
    else if (choice == 2) {
        const char Alpha[26]   = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        const char Masking[26] = "!@#$%^&*()-+=_{}[]|:;'<>?";
        int valid = 0;
 
        while (valid != 1) {
            printf("\n\n\n====ADMIN SIGN UP====\n\n\n");
            printf("Create Username: ");
            scanf("%39s", adminPtr->username);
 
            while (getchar() != '\n');
            printf("Create Password: ");
            fgets(adminPtr->password, sizeof(adminPtr->password), stdin);
            adminPtr->password[strcspn(adminPtr->password, "\n")] = '\0';
 
            if (adminPtr->password[0] == '\0') {
                printf("Password cannot be empty. Please try again.\n");
                continue;
            }
 
            int password_length = (int)strlen(adminPtr->password);
            if (password_length < 6) {
                printf("Password must be at least 6 characters long. Please try again.\n");
                continue;
            }
            if (password_length > 20) {
                printf("Password must be less than 21 characters long. Please try again.\n");
                continue;
            }
 
            for (int i = 0; adminPtr->password[i]; i++)
                adminPtr->password[i] = toupper(adminPtr->password[i]);
 
            for (int i = 0; adminPtr->password[i] != '\0'; i++) {
                for (int j = 0; j < 26; j++) {
                    if (adminPtr->password[i] == Alpha[j]) {
                        adminPtr->password[i] = Masking[j];
                        break;
                    }
                }
            }
 
            FILE *file = fopen(admin_path, "a");
            if (file == NULL)
                file = fopen(admin_path, "w");
            if (file != NULL) {
                fprintf(file, "%39s,%39s\n", adminPtr->username, adminPtr->password);
                fclose(file);
            }
 
            printf("Admin account created successfully!\n");
            valid = 1;
        }
    }
    else {
        printf("Invalid choice. Please try again.\n");
        return 0;
    }
}

 
void Unmask_password(char *stored, char *out) {
    const char Alpha[26]   = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const char Masking[26] = "!@#$%^&*()-+=_{}[]|:;'<>?";
    int len = (int)strlen(stored);
    int k   = 0;
 
    for (int i = 0; i < len; i++) {
        int mapped = 0;
        for (int j = 0; j < 26; j++) {
            if (stored[i] == Masking[j]) {
                out[k++] = Alpha[j];
                mapped = 1;
                break;
            }
        }
        if (!mapped) out[k++] = stored[i];
    }
    out[k] = '\0';
}

