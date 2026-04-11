#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "Employee.h"
#include "file.h"
#include "paths.h"


void Add_Employee(struct employee *empPtr) {
    int starter = 0;
    int check = 0;
 
    do {
        starter = 1;
        if (starter == -999) 
            break;

        if (starter != 1) { 
            printf("Invalid option.\n"); 
            break; 
        }
 
        do {
            check = 0;
            printf("Enter Employee ID (max 10 characters): ");
            scanf("%15s", empPtr->Employee_ID);

            if (strlen(empPtr->Employee_ID) > 10 && strlen(empPtr->Employee_ID)< 10) {
                printf("\n ID too long or too short. must be 10 characters.\n");
                check = 1;
            }

            if (valid_id(empPtr->Employee_ID) == 1) {
                printf("\nError: ID already exists. Try a different ID.\n");
                check = 1;
            }
            
        } while (check == 1);

        printf("Enter Employee name: ");
        scanf("%39s", empPtr->First_name);
 
        printf("Enter Employee Last name: ");
        scanf("%39s", empPtr->Last_name);
    
        snprintf(empPtr->full_name, sizeof(empPtr->full_name), "%s %s", empPtr->First_name, empPtr->Last_name);

        Valid_role(empPtr);
        
        do {
            printf("Enter Employee Status (ACTIVE, LEAVE, TERMINATED): ");
            scanf("%39s", empPtr->Status);

            if (Valid_status(empPtr->Status) == 0){
                printf("Error: Invalid Status.\n");
            }

        } while (Valid_status(empPtr->Status) == 0);
 
        do {
            printf("Enter Employee Hours Worked (0-80): ");
            scanf("%d", &empPtr->hours_worked);

            if (empPtr->hours_worked < 0 || empPtr->hours_worked > 80){
                printf("Invalid Hours Worked. Must be between 0 and 80.\n");
            }

        } while (empPtr->hours_worked < 0 || empPtr->hours_worked > 80);
 
        do {
            printf("Enter Employee Hourly Rate (0-160): ");
            scanf("%f", &empPtr->Hourly_rate);

            if (empPtr->Hourly_rate < 0 || empPtr->Hourly_rate > 160){
                printf("Invalid Hourly Rate. Must be between 0 and 160.\n");
            }

        } while (empPtr->Hourly_rate < 0 || empPtr->Hourly_rate > 160);
 

        FILE *file = fopen(employee_path, "a");
        if (file == NULL) {
                printf("File Failed to open\n"); 
            }
 
        fprintf(file, "%s,%s,%s,%s,%d,%.2f\n",
            empPtr->Employee_ID,
            empPtr->full_name,
            empPtr->Role,
            empPtr->Status,
            empPtr->hours_worked,
            empPtr->Hourly_rate);

        fclose(file);
        printf("\n Employee added successfully.\n");

        printf("\n Press 1 Add Employee or -999 to stop: ");
        scanf("%d", &starter);
 
    } while (starter != -999);
}


void list_emplyees(struct employee *empPtr) {
    char line[200];
    char Full_name[80];
 
    FILE *file = fopen(employee_path, "r");
    if (file == NULL) { 
        printf("Error: Employee file not found.\n");
        return; 
    }
 
    fseek(file, 0, SEEK_END);

    if (ftell(file) == 0) {
        printf("No employees found. File is empty.\n");
        fclose(file);
        return;
    }

    rewind(file);
 
    printf("\n%-10s %-20s %-20s %-15s %-15s %-15s\n",
        "ID", "Full Name", "Role", "Status", "Hours Worked", "Hourly Rate");
    printf("---------------------------------------------------------------------------------------------------------\n");
 
    while (fgets(line, sizeof(line), file) != NULL) {
        sscanf(line, "%[^,],%[^,],%[^,],%[^,],%d,%f",
            empPtr->Employee_ID, 
            empPtr->full_name,
            empPtr->Role, 
            empPtr->Status,
            &empPtr->hours_worked, 
            &empPtr->Hourly_rate);

 
        printf("%-10s %-20s %-20s %-15s %-15d %-15.2f\n",
            empPtr->Employee_ID, 
            empPtr->full_name, 
            empPtr->Role,
            empPtr->Status, 
            empPtr->hours_worked, 
            empPtr->Hourly_rate);
    }
    printf("---------------------------------------------------------------------------------------------------------\n");
    fclose(file);
}

void find_employee(struct employee *empPtr) {
    char line[200], ID_search[10], ID_in_system[10], Full_name[80];
    int  found = 0;
 
    printf("Enter the ID number of the person you wish to find: ");
    scanf("%9s", ID_search);
 
    FILE *file = fopen(employee_path, "r");
    if (file == NULL) { 
        printf("Error: Employee file not found.\n");
    }
 
    while (fgets(line, sizeof(line), file) != NULL) {
        
        sscanf(line, "%9[^,]", ID_in_system);
        if (strcmp(ID_in_system, ID_search) == 0) {
            found = 1;
            sscanf(line, "%[^,],%[^,],%[^,],%[^,],%d,%f",
                empPtr->Employee_ID, 
                empPtr->full_name,
                empPtr->Role, 
                empPtr->Status,
                &empPtr->hours_worked, 
                &empPtr->Hourly_rate);
 
            printf("\n%-10s %-20s %-20s %-15s %-15s %-15s\n",
                "ID", "Full Name", "Role", "Status", "Hours Worked", "Hourly Rate");
            printf("---------------------------------------------------------------------------------------\n");
            printf("%-10s %-20s %-20s %-15s %-15d %-15.2f\n",
                empPtr->Employee_ID, 
                empPtr->full_name, 
                empPtr->Role,
                empPtr->Status, 
                empPtr->hours_worked, 
                empPtr->Hourly_rate);
            printf("---------------------------------------------------------------------------------------\n");
            break;
        }
    }
    if (!found) 
    printf("Employee with ID '%s' not found.\n", ID_search);

    fclose(file);
}

void Update_Record(struct employee *empPtr) {
    char line[200], ID_search[10];
    int  found = 0, choice;
 
    printf("Enter the ID number of the record you wish to update: ");
    scanf("%9s", ID_search);
 
    FILE *file = fopen(employee_path, "r");
    if (file == NULL) { printf("Error: Employee file not found.\n"); return; }
 
    FILE *temp = fopen(temp_path, "w");
    if (temp == NULL) {
        printf("Error: Could not create temp file.\n");
        fclose(file);
        return;
    }
 
    while (fgets(line, sizeof(line), file) != NULL) {
        sscanf(line, "%[^,],%[^,],%[^,],%[^,],%d,%f",
            empPtr->Employee_ID, 
            empPtr->full_name, 
            empPtr->Role, 
            empPtr->Status,
            &empPtr->hours_worked, 
            &empPtr->Hourly_rate);
 
        if (strcmp(empPtr->Employee_ID, ID_search) == 0) {
            found = 1;
            printf("Found: %s %s, Role: %s, Status: %s, Hours Worked: %d, Hourly Rate: %.2f\n",
                empPtr->Employee_ID, 
                empPtr->full_name,
                empPtr->Role, 
                empPtr->Status,
                empPtr->hours_worked, 
                empPtr->Hourly_rate);
 
            do {
                printf("What would you like to update?\n");
                printf("1. First Name\n2. Last Name\n3. Role\n4. Status\n5. Hours Worked\n6. Hourly Rate\n7. Stop updating\n");
                printf("Enter your choice: ");
                scanf("%d", &choice);
 
                switch (choice) {
                    case 1:
                        printf("Enter new First Name: ");
                        scanf("%39s", empPtr->First_name);
                        break;
                    case 2:
                        printf("Enter new Last Name: ");
                        scanf("%39s", empPtr->Last_name);
                        break;
                    case 3:
                        Valid_role(empPtr);
                        break;
                    case 4:
                        do {
                            printf("Enter new Status (ACTIVE, LEAVE, TERMINATED): ");
                            scanf("%39s", empPtr->Status);
                            if (Valid_status(empPtr->Status) == 0)
                                printf("Invalid Status. Please try again.\n");
                        } while (Valid_status(empPtr->Status) == 0);
                        break;
                    case 5:
                        do {
                            printf("Enter new Hours Worked (0-80): ");
                            scanf("%d", &empPtr->hours_worked);
                            if (empPtr->hours_worked < 0 || empPtr->hours_worked > 80)
                                printf("Invalid. Must be between 0 and 80.\n");
                        } while (empPtr->hours_worked < 0 || empPtr->hours_worked > 80);
                        break;
                    case 6:
                        do {
                            printf("Enter new Hourly Rate (0-160): ");
                            scanf("%f", &empPtr->Hourly_rate);
                            if (empPtr->Hourly_rate < 0 || empPtr->Hourly_rate > 160)
                                printf("Invalid. Must be between 0 and 160.\n");
                        } while (empPtr->Hourly_rate < 0 || empPtr->Hourly_rate > 160);
                        break;
                    case 7:
                        printf("Finished updating.\n");
                        break;
                    default:
                        printf("Invalid choice. Please try again.\n");
                }
            } while (choice != 7);
        }
 
        fprintf(temp, "%s,%s,%s,%s,%d,%.2f\n",
            empPtr->Employee_ID, 
            empPtr->full_name,
            empPtr->Role, 
            empPtr->Status,
            empPtr->hours_worked, 
            empPtr->Hourly_rate);
    }
 
    fclose(file);
    fclose(temp);
 
    if (found == 0) {
        printf("Employee with ID '%s' not found.\n", ID_search);
        remove(temp_path);
        return;
    }
    remove(employee_path);
    rename(temp_path, employee_path);
    printf("Employee updated successfully.\n");
}

void Delete_Record(struct employee *empPtr) {
    char line[200], ID_search[10];
    int  found = 0;
 
    printf("Enter the ID number of the record you wish to delete: ");
    scanf("%9s", ID_search);
 
    FILE *file = fopen(employee_path, "r");
    if (file == NULL) { printf("Error: Employee file not found.\n"); return; }
 
    FILE *temp = fopen(temp_path, "w");
    if (temp == NULL) {
        printf("Error: Could not create temp file.\n");
        fclose(file);
        return;
    }
 
    while (fgets(line, sizeof(line), file) != NULL) {
        sscanf(line, "%[^,],%[^,],%[^,],%[^,],%d,%f",
            empPtr->Employee_ID,
            empPtr->full_name,
            empPtr->Role, 
            empPtr->Status,
            &empPtr->hours_worked, 
            &empPtr->Hourly_rate);
 
        if (strcmp(empPtr->Employee_ID, ID_search) != 0) {
            fprintf(temp, "%s,%s,%s,%s,%d,%.2f\n",
                empPtr->Employee_ID, 
                empPtr->full_name,
                empPtr->Role, 
                empPtr->Status,
                empPtr->hours_worked, 
                empPtr->Hourly_rate);
        } else {
            found = 1;
        }
    }
 
    fclose(file);
    fclose(temp);
 
    if (found) {
        remove(employee_path);
        rename(temp_path, employee_path);
        printf("Employee with ID '%s' deleted successfully.\n", ID_search);
    } else {
        printf("Employee with ID '%s' not found.\n", ID_search);
        remove(temp_path);
    }
}

int valid_id(char *idPtr) {
    char existingID[10];
    char line[200];
 
    FILE *file = fopen(employee_path, "r");
    if (file == NULL) return 0;
 
    while (fgets(line, sizeof(line), file) != NULL) {
        sscanf(line, "%9[^,]", existingID);
        if (strcmp(existingID, idPtr) == 0) {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
} 
 
int Valid_status(char *Status) {
    char temp[40];
    strncpy(temp, Status, sizeof(temp) - 1);
    temp[sizeof(temp) - 1] = '\0';
    for (int i = 0; temp[i]; i++) temp[i] = toupper(temp[i]);
 
    if (strcmp(temp, "ACTIVE")     == 0) return 1;
    if (strcmp(temp, "LEAVE")      == 0) return 1;
    if (strcmp(temp, "TERMINATED") == 0) return 1;
    printf("Invalid Input\n");
    return 0;
}
 
void Valid_role(struct employee *emp) {
    char temp[40];
    int check = 0;

    do{
        
        printf("Enter Employee Role (BARISTA, SERVER, COOK, CASHIER, KITCHEN ASSISTANT, SUPERVISOR ): ");
        scanf("%s", emp->Role);

        while (getchar() != '\n');  

        int len = strlen(emp->Role);
        
        for (int i = 0; i < len; i++){
            temp[i] = toupper(emp->Role[i]);
            temp[len] = '\0'; 
        }


        if (strcmp(temp, "BARISTA")         == 0) check = 1;
        if (strcmp(temp, "SERVER")          == 0) check = 1;
        if (strcmp(temp, "COOK")            == 0) check = 1;
        if (strcmp(temp, "CASHIER")         == 0) check = 1;
        if (strcmp(temp, "KITCHEN ASSISTANT") == 0) check = 1;
        if (strcmp(temp, "SUPERVISOR")      == 0) check = 1;
        

        if (check == 0) {
            printf("Invalid Role. Please try again.\n");
    
        } 
        else {
            strncpy(emp->Role, temp, sizeof(emp-> Role));
            check =  1;
        }
        
    } while (check == 0);

}

void write_header() {
    FILE *file = fopen(employee_path, "r");
    if (file != NULL) { fclose(file); 
    return;
    } 

    file = fopen(employee_path, "w");
    if (file) {
        fprintf(file, "EmployeeID,FullName,Role,Status,HoursWorked,HourlyRate\n");
        fclose(file);
    }
}