#ifndef EMPLOYEE_H
#define EMPLOYEE_H

struct employee{
    char Employee_ID[10];
    char First_name[40];
    char Last_name[40];
    char full_name[80];
    char Role[40];
    char Status[40];
    int hours_worked;
    float Hourly_rate;
};

void Add_Employee(struct employee *emp);
void list_emplyees(struct employee *emp);
void find_employee(struct employee *emp);
void Update_Record(struct employee *emp);
void Delete_Record(struct employee *emp);
void Valid_role(struct employee *emp);
void write_header();

int valid_id(char *idPtr);
int Valid_status(char *status);


#endif