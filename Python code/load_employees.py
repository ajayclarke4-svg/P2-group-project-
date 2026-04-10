import csv
import os
import dataforp2

EMPLOYEE_FILE = os.path.join("..", "C code","data", "employee_records.csv")

def load_employees():
    dataforp2.employees = []

    if not os.path.exists(EMPLOYEE_FILE):
        print("\n\t\t[!] employee_records.csv not found.")
        print("\t\t    Run the C program first to create employee records.\n")
        return

    with open(EMPLOYEE_FILE, newline="") as file:
        reader = csv.DictReader(file)
        for row in reader:
            dataforp2.employees.append(row)

    if len(dataforp2.employees) == 0:
        print("\n\t\tNo employees found. File is empty.\n")
    else:
        print(f"\n\t\t{len(dataforp2.employees)} employee(s) loaded successfully.\n")


if __name__ == '__main__':
    load_employees()