import dataforp2
import csv
from datetime import datetime# for real date for payroll tracker
from colorama import Fore, Back, Style, init

init(autoreset=True)

def save_payroll(emp_id, hours, rate, gross, nis, tax, net):

# Purpose: Saves payroll information to payroll.csv
# Parameters:
#   emp_id - employee ID
#   hours - hours worked
#   rate - hourly rate
#   gross - gross pay
#   nis - NIS deduction
#   tax - education tax deduction
#   net - net pay
# Description:
# This function checks if the payroll file exists. If not, it creates
# the file and writes the header. It then appends the payroll data
# for an employee to the file.

    file_exists = False
    try:
        with open("payroll.csv", "r"):
            file_exists = True
    except FileNotFoundError:
        pass

    with open("payroll.csv", "a", newline="") as file:
        writer = csv.writer(file)

        # Write header if file is new
        if not file_exists:
            writer.writerow(["date","EmployeeID","hours","rate","gross","nis","tax","net"])

        writer.writerow([
            datetime.now().date(),
            emp_id,
            hours,
            rate,
            gross,
            nis,
            tax,
            net
        ]) 


def calculate_payroll():   
# Purpose: Calculates payroll for all active employees
# Description:
# This function loops through the employee list and processes only
# employees with ACTIVE status. It retrieves hours worked and
# hourly rate, calculates gross pay, NIS, education tax, and net pay.
# The payroll data is saved using save_payroll() and results
# are displayed to the user.
    NIS_RATE = 0.025
    EDU_TAX_RATE = 0.0225
    if not dataforp2.employees:
        print(Fore.RED + "\t\t\t\t\t\tLoad employees first!")
        return

    for worker in dataforp2.employees:

        # Only process ACTIVE employees
        if worker["Status"].upper() != "ACTIVE":
            continue

        try:
            hours = worker["HoursWorked"]
        except (KeyError, ValueError):#saftey net against key word error and value error
            print(Fore.RED + f"\t\t\t\t\t\tInvalid hours for {worker['FullName']}. Skipping.")
            continue

        try:
            rate = float(worker["HourlyRate"])
        except (KeyError, ValueError):
            print(Fore.RED + f"\t\t\t\t\t\tInvalid hourly rate for {worker['FullName']}. Skipping.")
            continue
        
        gross = hours * rate

        nis = gross * NIS_RATE
        edu_tax = (gross - nis) * EDU_TAX_RATE

        net = gross - nis - edu_tax

        save_payroll(worker["EmployeeID"], hours, rate, gross, nis, edu_tax, net)

        print(Fore.CYAN + "\n\t\t\t\t\t\t--- Payroll Result ---")
        print(Fore.MAGENTA + f"\t\t\t\t\t\tName: {worker['FullName']}")
        print(Fore.WHITE + f"\t\t\t\t\t\tGross Pay: ${gross:.2f}")
        print(Fore.YELLOW + f"\t\t\t\t\t\tNIS: ${nis:.2f}")
        print(Fore.YELLOW + f"\t\t\t\t\t\tEducation Tax: ${edu_tax:.2f}")
        print(Fore.GREEN + f"\t\t\t\t\t\tNet Pay: ${net:.2f}\n")

def top_3_highest_paid():
# Purpose: Displays the top 3 highest paid employees
# Description:
# This function reads payroll.csv and stores employee payroll data.
# It sorts the employees by net pay in descending order and
# displays the top three employees with the highest net pay.
    payroll_list = []

    try:
        with open("payroll.csv", "r") as file:
            reader = csv.DictReader(file)

            for row in reader:
                try:
                    payroll_list.append({
                        "EmployeeID": row["EmployeeID"],
                        "net": float(row["net"]),
                        "date": row["date"]
                    })
                except ValueError:
                    continue

    except FileNotFoundError:
        print(Fore.RED + "\t\t\t\t\t\tPayroll file not found!")
        return

    if not payroll_list:
        print(Fore.RED + "\t\t\t\t\t\tNo payroll data available!")
        return

    # Sort by Net Pay (highest first)
    payroll_list.sort(key=lambda x: x["net"], reverse=True)

    print(Fore.CYAN + "\n\t\t\t\t\t\t=== Top 3 Highest Paid Employees ===\n")

    for i, worker in enumerate(payroll_list[:3], start=1):
        print(Fore.YELLOW + f"\t\t\t\t\t\t#{i}")
        print(Fore.WHITE + f"\t\t\t\t\t\tEmployee ID: {worker['EmployeeID']}")
        print(Fore.GREEN + f"\t\t\t\t\t\tNet Pay: ${worker['net']:.2f}")
        print(Fore.MAGENTA + f"\t\t\t\t\t\tDate: {worker['date']}\n")

if __name__ == '__main__':
    calculate_payroll()
