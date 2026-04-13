/*Module: Employee Management System
Purpose: Handles admin authentication and employee record management
Description:This program manages employees, including adding, listing, finding,updating, deleting, and cleaning records. 
It also includes admin authentication with login/signup and password masking system.*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "admin.h"
#include "file.h"
#include "paths.h"



void Main_menu() {
/* Purpose: Displays and controls the main employee management menu
Description: Requires admin login before access. After login, provides a menu
for employee operations such as add, list, find, update, delete, and clean records.*/

    struct Admin admin;
    struct employee emp;
    int choice;
    int c;

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

        while ((c = getchar()) != '\n' && c != EOF);

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1:
                Add_Employee(&emp);
                break;
            case 2:
                list_employees(&emp);
                break;
            case 3:    
                find_employee(&emp);
                break;
            case 4:
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
    } while (choice != 7);
}


int Admin_page(struct Admin *adminPtr) {
/* Purpose: Handles admin login and sign-up system
Description: Allows admin to either log in or create an account. Login checks stored credentials from file with masked password.
Signup validates password rules and stores encrypted password.*/

    int choice;

    do {
        printf("\n\n\n=====ADMIN PAGE=====\n\n\n");
        printf("1. Sign in Page\n");
        printf("2. Sign up Page\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            Admin_login(adminPtr);
            return 1;
        }
        else if (choice == 2) {
            Admin_sign_up(adminPtr);
            return 1;
        }
        else {
            printf("Invalid choice. Please try again.\n");
        }
    } while (1);
}


void Admin_sign_up(struct Admin *adminPtr){
// Purpose: Handles sign-up system
// Description: Allows admin to create an account.
// Signup validates password rules and stores encrypted password to txt file.

    int valid = 0;
    int password_length = 0;
    int attempts = 3;
    char Admin_Page_login[11];
    char password[11] = "1234567890";

    while (attempts > 0) {
        printf("Enter Admin Login Password: ");
        scanf("%10s", Admin_Page_login);

        if (strcmp(Admin_Page_login, password) == 0) {
            printf("\n=======Welcome=======\n\n"); 
            break; 
        }
        else {
            attempts--;
            printf("Incorrect password.\n");

            if (attempts > 0) {
                printf("Attempts remaining: %d\n\n", attempts);
            } else {
                printf("No attempts left. Exiting.\n");
                exit(0);
            }
        }
    }

    while (valid != 1) {

        const char Alpha[27]   = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        const char Masking[27] = "!@#$%^&*()-+=_{}[]|:;'<>?~";

        printf("\n\n\n====ADMIN SIGN UP====\n\n\n");
        printf("Create Username: ");
        scanf("%39s", adminPtr->username);

        printf("Create Password: ");
        scanf("%39s", adminPtr->password);

        if (adminPtr->password[0] == '\0') {
            printf("Password cannot be empty. Please try again.\n");
            continue;
        }

        password_length = strlen(adminPtr->password);

        if (password_length < 6) {
            printf("Password must be at least 6 characters long. Please try again.\n");
            continue;
        }
        
        if (password_length > 20) {
            printf("Password must be less than 20 characters long. Please try again.\n");
            continue;
        }

        for (int i = 0; adminPtr->password[i]; i++){
            adminPtr->password[i] = toupper(adminPtr->password[i]);
        }

        for (int i = 0; adminPtr->password[i] != '\0'; i++) {
            for (int j = 0; j < 26; j++){
                if (adminPtr->password[i] == Alpha[j]) {
                    adminPtr->password[i] = Masking[j];
                    break;
                }
            }
        }
        
        FILE *file = fopen(admin_path, "a");

        if (file == NULL){
            file = fopen(admin_path, "w");
            printf("File was not found. File will be created. Try again.\n");
            return;
        }

        if (file != NULL) {
            fprintf(file, "%s,%s\n", adminPtr->username, adminPtr->password);
            fclose(file);
            printf("Admin account created successfully!\n");
            valid = 1;
        }
        else {
            printf("Sign in failed.\n");
        }
    }
}


void Admin_login(struct Admin *adminPtr){

    char line[200];
    char collected_Username[40], collected_Password[40];
    char storedUser[40], storedPassword[40], decodedPass[40];
    int  attempts = 3;
    const char Alpha[27]   = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const char Masking[27] = "!@#$%^&*()-+=_{}[]|:;'<>?~";

    while (attempts > 0) {

        printf("\n\n\n=====ADMIN LOGIN=====\n\n\n");
        printf("Enter Username: ");
        scanf("%39s", collected_Username);

        printf("Enter Password: ");
        scanf("%39s", collected_Password);

        for (int i = 0; collected_Password[i]; i++){
            collected_Password[i] = toupper(collected_Password[i]);
        }

        FILE *file = fopen(admin_path, "r");

        if (file == NULL) {
            printf("Error: Admin file not found.\n");
            attempts--;
            break;
        }

        while (fgets(line, sizeof(line), file) != NULL) {
            
            sscanf(line, "%[^,],%s", storedUser, storedPassword);

            for (int i = 0; storedPassword[i] != '\0'; i++){
                decodedPass[i] = storedPassword[i];
                for (int j = 0; j < 26; j++){
                    if (storedPassword[i] == Masking[j]){
                        decodedPass[i] = Alpha[j];
                        break;
                    }
                }
            }

            decodedPass[strlen(storedPassword)] = '\0'; 

            if (strcmp(storedUser, collected_Username) == 0 && strcmp(decodedPass, collected_Password) == 0) {
                printf("\n\n---ACCESS GRANTED!!---\n\n");
                fclose(file);
                return;
            }
        } 

        fclose(file);
        attempts--;

        printf("\nACCESS DENIED!!\n");
        if (attempts > 0){
            printf("Remaining attempts: %d\n", attempts);
        } 
        else {
            printf("No attempts left!!\n\n---ACCESS BLOCKED!!!---\n\n");
            exit(0);
        }
    } 
}