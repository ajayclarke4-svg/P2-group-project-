import csv# used to enable use of csv files
import dataforp2# imported the universal list 
from colorama import Fore, Back, Style, init#used for the color and design in the code
init(autoreset=True)

def load_employees():
    dataforp2.employees = []  # Reset employee list before loading new data

    try:# this is a saftey net
        with open("employee_records.csv", "r") as file:
            reader = csv.DictReader(file, fieldnames=["EmployeeID","FirstName","LastName","Role","Status","HourlyRate", "HoursWorked"])# Assign headers manually because CSV file has no header row

            for row in reader:#now for all the headers made above the code is going to add the data in that row
                worker = {
                    "EmployeeID": row["EmployeeID"],
                    "FullName": row["FirstName"].capitalize() + " " + row["LastName"].capitalize(),# Combine first and last name into one formatted full name
                    "Role": row["Role"].upper(),
                    "Status": row["Status"].strip().upper(),
                    "HourlyRate": float(row["HourlyRate"]) if row["HourlyRate"] else 0.0,# safety precaution just incase the data from is not read
                    "HoursWorked": float(row["HoursWorked"]) if row["HoursWorked"] else 0.0
                }
                dataforp2.employees.append(worker)# Add employee dictionary to shared employee list

        if not dataforp2.employees:# saftey net if no data is found in dataforp2
            print(Fore.RED + "\t\t\t\t\t\tNo employees found!")
            return
        print(Fore.GREEN+f"\t\t\t\t\t\t{len(dataforp2.employees)} Employees loaded successfully!")
        
    except FileNotFoundError:#this is the saftey net for a no file found error
        print(Fore.RED + "\t\t\t\t\t\tEmployees file not found!")

if __name__ == '__main__':#only allows the function to run if called in main
    load_employees()
