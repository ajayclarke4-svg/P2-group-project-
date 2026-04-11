// Module: Employee Management System
// Purpose: Handles admin authentication and employee record management
// Description:
// This program manages employees, including adding, listing, finding,
// updating, deleting, and cleaning records. It also includes admin
// authentication with login/signup and password masking system.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "admin.h"
#include "file.h"
#include "paths.h"


void Main_menu() {
    // Purpose: Displays and controls the main employee management menu
// Description:
// Requires admin login before access. After login, provides a menu
// for employee operations such as add, list, find, update, delete,
// and clean records.
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
                // Adds a new employee record
                break;

            case 2:
                list_emplyees(&emp);
                // Displays all employees
                break;

            case 3:    
                find_employee(&emp);
                // Searches for employee
                break;

            case 4:
                Update_Record(&emp);
                // Updates employee data
                break;

            case 5:
                Delete_Record(&emp);
                // Deletes employee record
                break;

            case 6:
                Clean_file(&emp);
                // Clears employee file
                break;
            case 7: 
                printf("Exiting Employee Management System. Goodbye!\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }  while (choice != 7);

}


int Admin_page(struct Admin *adminPtr) {
    // Purpose: Handles admin login and sign-up system
// Description:
// Allows admin to either log in or create an account.
// Login checks stored credentials from file with masked password.
// Signup validates password rules and stores encrypted password.
    int choice;
    printf("\n\n\n=====ADMIN PAGE=====\n\n\n");
    printf("1. Sign in Page\n");
    printf("2. Sign up Page\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
 
    if (choice == 1) {
        Admin_sign_in(adminPtr);
    }
    else if (choice == 2) {
        Admin_login(adminPtr);
    }
    else {
        printf("Invalid choice. Please try again.\n");
        return 0;
    }
}


int Admin_sign_in(struct Admin *adminPtr){
    
        char line[200];
        char collected_Username[40], collected_Password[40];
        char storedUser[40], storedPassword[40], decodedPass[40];;
        int  attempts = 3;
 
        while (attempts > 0) {

            printf("\n\n\n=====ADMIN LOGIN=====\n\n\n");
            printf("Enter Username: ");
            scanf("%39s", collected_Username);
            printf("Enter Password: ");
            scanf("%39s", collected_Password);
 
            for (int i = 0; collected_Password[i]; i++)
                collected_Password[i] = toupper(collected_Password[i]);
 
            FILE *file = fopen(admin_path, "r");

            if (file == NULL) {
                printf("Error: Admin file not found.\n");
                return 0;
            }
 
            while (fgets(line, sizeof(line), file) != NULL) {
              
                sscanf(line, "%[^,],%39s", storedUser, storedPassword);
 
                Unmask_password(storedPassword, decodedPass);
 
                if (strcmp(storedUser, collected_Username) == 0 && strcmp(decodedPass, collected_Password) == 0) {
                    printf("\n\n---ACCESS GRANTED!!---\n\n");
                    return 1;
                }
            } 
            fclose(file);
 
            attempts--;
            printf("\nACCESS DENIED!!\n");
            if (attempts > 0){
                printf("Remaining attempts: %d\n", attempts);
            } else {
                printf("No attempts left!!\n\n---ACCESS BLOCKED!!!---\n\n");
            }
        } exit(0);
}

int Admin_login(struct Admin *adminPtr){
    const char Alpha[26]   = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        const char Masking[26] = "!@#$%^&*()-+=_{}[]|:;'<>?";
        int valid = 0;
 
        while (valid != 1) {

            printf("\n\n\n====ADMIN SIGN UP====\n\n\n");
            printf("Create Username: ");
            scanf("%39s", adminPtr->username);
 
            printf("Create Password: ");
            fgets(adminPtr->password, sizeof(adminPtr->password), stdin);

            if (adminPtr->password[0] == '\0') {
                printf("Password cannot be empty. Please try again.\n");
                continue;
            }
 
            int password_length = strlen(adminPtr->password);

            if (password_length < 6) {
                printf("Password must be at least 6 characters long. Please try again.\n");
                continue;
            }
            if (password_length > 20) {
                printf("Password must be less than 20 characters long. Please try again.\n");
                continue;
            }
 
            for (int i = 0; adminPtr->password[i]; i++)
                adminPtr->password[i] = toupper(adminPtr->password[i]);

            // Mask the password
            for (int i = 0; adminPtr->password[i] != '\0'; i++) {
                for (int j = 0; j < 26; j++) {
                    if (adminPtr->password[i] == Alpha[j]) {
                        adminPtr->password[i] = Masking[j];
                        break;
                    }
                }
            }
 
            FILE *file = fopen(admin_path, "a");
            if (file == NULL){
                file = fopen(admin_path, "w");
            }

            if (file != NULL) {
                fprintf(file, "%39s,%39s\n", adminPtr->username, adminPtr->password);
                fclose(file);
                return 1;
            }
 
            printf("Admin account created successfully!\n");
            valid = 1;
        }
}


void Unmask_password(char *stored, char *out) {
// Function: Unmask_password
// Purpose: Decodes masked password
// Description:
// Converts masked characters back into original letters
// during admin login verification.
    const char Alpha[26]   = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const char Masking[26] = "!@#$%^&*()-+=_{}[]|:;'<>?";
    int len = (int)strlen(stored);
    int k   = 0, mapped = 0;
 
    for (int i = 0; i < len; i++) {
        mapped = 0;
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

