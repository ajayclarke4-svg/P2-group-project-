import dataforp2
import csv
import time
from colorama import Fore, Back, Style, init
init(autoreset=True)
import os
import time

def save_terminated_employees():
# Purpose: Saves terminated employees to a separate CSV file
# Description:
# This function scans the employee list and collects all employees
# with TERMINATED status. If any are found, it writes their details
# to terminated_employees.csv including ID, name, role, status,
# hourly rate, and hours worked. If no terminated employees are
# found, a message is displayed.
    terminated = []

    # collect terminated employees
    for worker in dataforp2.employees:
        if worker["Status"].upper() == "TERMINATED":
            terminated.append(worker)

    # check if any found
    if not terminated:
        print(Fore.YELLOW + "\t\t\t\t\t\tNo terminated employees found.")
        return

    # save to file
    with open("terminated_employees.csv", "w", newline="") as file:
        writer = csv.writer(file)

        # headers
        writer.writerow([
            "EmployeeID",
            "FullName",
            "Role",
            "Status",
            "HourlyRate",
            "HoursWorked"
        ])

        # write data
        for worker in terminated:
            writer.writerow([
                worker["EmployeeID"],
                worker["FullName"],
                worker["Role"],
                worker["Status"],
                worker["HourlyRate"],
                worker["HoursWorked"]
            ])

    print(Fore.GREEN + "\t\t\t\t\t\tTerminated employees saved successfully!")

def load_payroll_history():
# Purpose: Loads payroll history from payroll.csv
# Description:
# This function reads payroll.csv and stores payroll records
# in a dictionary grouped by EmployeeID. Each employee can
# have multiple payslips. The function returns the payroll
# history dictionary for use in reports and employee views.
# If the file does not exist, an empty dictionary is returned.
    payroll_history = {}
    try:
        with open("payroll.csv", "r") as file:
            reader = csv.DictReader(file, fieldnames=["date", "EmployeeID", "hours", "rate", "gross", "nis", "tax", "net"])
            for row in reader:
                # CSV format: Date, EmployeeID, Hours, Rate, Gross, NIS, Tax, Net
                emp_id = row["EmployeeID"]
                payslip = {
                    "date": row["date"],
                    "hours": row['hours'] ,
                    "rate": row['rate'],
                    "gross": row['gross'],
                    "nis": row['nis'],
                    "tax": row['tax'],
                    "net": row["net"]
                }
                if emp_id not in payroll_history:
                    payroll_history[emp_id] = []
                payroll_history[emp_id].append(payslip)
    except FileNotFoundError: 
        pass  # no payroll yet
    return payroll_history

def payroll_summary():
# Purpose: Displays payroll summary report
# Description:
# This function loads payroll history and calculates total
# gross pay, NIS, education tax, and net pay for each employee.
# It also calculates overall totals for all employees and
# displays the results in a formatted summary report.
    payroll_history = load_payroll_history()
    if not payroll_history:
        print(Fore.RED + Style.BRIGHT +"\t\t\t\t\t\tNo payroll records found!\n")
        return

    overall_gross = 0
    overall_nis = 0
    overall_tax = 0
    overall_net = 0

    print("\n\t\t\t\t\t\t--- Payroll Summary Report ---\n")
    for worker in dataforp2.employees:
        emp_id = worker["EmployeeID"]
        payslips = payroll_history.get(emp_id, [])
        if not payslips:
            continue

        total_gross = sum(float(p['gross']) for p in payslips)
        total_nis = sum(float(p['nis']) for p in payslips)
        total_tax = sum(float(p['tax']) for p in payslips)
        total_net = sum(float(p['net']) for p in payslips)

        overall_gross += total_gross
        overall_nis += total_nis
        overall_tax += total_tax
        overall_net += total_net

        
        print(Fore.MAGENTA + f"\t\t\t\t\t\tEmployee: {worker['FullName']} (ID: {emp_id})")
        print(Fore.WHITE + f"\t\t\t\t\t\tTotal Gross Pay       : {total_gross:0.2f}")
        print(Fore.WHITE + f"\t\t\t\t\t\tTotal NIS Deduction   : {total_nis:0.2f}")
        print(Fore.WHITE + f"\t\t\t\t\t\tTotal Education Tax   : {total_tax:0.2f}")
        print(Fore.GREEN + f"\t\t\t\t\t\tTotal Net Pay         : {total_net:0.2f}\n")
    
    print(Fore.CYAN + "\t\t\t\t\t\t---------------------------------------------")
    print(Fore.YELLOW + f"\t\t\t\t\t\tOverall Gross Pay       : {overall_gross:.2f}")
    print(Fore.YELLOW + f"\t\t\t\t\t\tOverall NIS Deduction   : {overall_nis:.2f}")
    print(Fore.YELLOW + f"\t\t\t\t\t\tOverall Education Tax   : {overall_tax:.2f}")
    print(Fore.GREEN + f"\t\t\t\t\t\tOverall Net Pay         : {overall_net:.2f}\n")


def view_employees():
# Purpose: Displays employee information
# Description:
# This function displays all employees and allows filtering
# by status (Active, Leave, Terminated, or All). It shows
# employee details such as ID, name, role, status, hourly rate,
# and hours worked. It also displays the last payroll record
# for each employee if available.
# Terminated employees are saved using save_terminated_employees().
    if not dataforp2.employees:
        print(Fore.RED + "\t\t\t\t\t\tLoad employees first!")
        return

    payroll_history = load_payroll_history()  # load full payroll history

    status_filter = input(Fore.LIGHTCYAN_EX + "\t\t\t\t\t\tFilter (Active/Leave/Terminated/All): ")

    for worker in dataforp2.employees:
        if status_filter.lower() != "all" and worker.get("Status", "N/A").lower() != status_filter.lower():
            continue

        print(Fore.CYAN + "\n\t\t\t\t\t\t------------------")
        print(Fore.MAGENTA + f"\t\t\t\t\t\tID    : {worker.get('EmployeeID', 'N/A')}")
        time.sleep(0.3)
        print(Fore.WHITE + f"\t\t\t\t\t\tName  : {worker.get('FullName', 'N/A')}")
        time.sleep(0.3)
        print(Fore.WHITE + f"\t\t\t\t\t\tRole  : {worker.get('Role', 'N/A')}")
        time.sleep(0.3)
        print(Fore.WHITE + f"\t\t\t\t\t\tStatus: {worker.get('Status', 'N/A')}")
        time.sleep(0.3)
        print(Fore.YELLOW + f"\t\t\t\t\t\tHourly Rate: {worker.get('HourlyRate', 'N/A')}")
        time.sleep(0.3)
        print(Fore.YELLOW + f"\t\t\t\t\t\tHour's Worked: {worker.get('HoursWorked', 'N/A')}")
        emp_id = worker.get("EmployeeID")
        emp_payslips = payroll_history.get(emp_id, [])

        print(Fore.CYAN + f"\n\t\t\t\t\t\t--- Last Payroll Record ---")
        if emp_payslips:
            last = emp_payslips[-1]
            print(Fore.WHITE + f"\t\t\t\t\t\tLast Paid Date : {last['date']}")
            print(Fore.WHITE + f"\t\t\t\t\t\tHours Worked   : {last['hours']}")
            print(Fore.WHITE + f"\t\t\t\t\t\tHourly Rate    : {last['rate']}")
            print(Fore.WHITE + f"\t\t\t\t\t\tGross Pay      : {last['gross']}")
            print(Fore.WHITE + f"\t\t\t\t\t\tNIS            : {last['nis']}")
            print(Fore.WHITE + f"\t\t\t\t\t\tEducation Tax  : {last['tax']}")
            print(Fore.GREEN + f"\t\t\t\t\t\tNet Pay        : {last['net']}\n")
        else:
            print(Fore.RED + "\t\t\t\t\t\tLast Paid Date : None")
            print(Fore.RED + "\t\t\t\t\t\tHours Worked   : None")
            print(Fore.RED + "\t\t\t\t\t\tHourly Rate    : None")
            print(Fore.RED + "\t\t\t\t\t\tGross Pay      : None")
            print(Fore.RED + "\t\t\t\t\t\tNIS            : None")
            print(Fore.RED + "\t\t\t\t\t\tEducation Tax  : None")
            print(Fore.RED + "\t\t\t\t\t\tNet Pay        : None")
    save_terminated_employees()
if __name__ == "__main__":
    view_employees()
