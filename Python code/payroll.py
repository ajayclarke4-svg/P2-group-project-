
import csv
import os
from datetime import datetime
import dataforp2

PAYROLL_FILE = "payroll.csv"
PAYROLL_FREQUENCY = "Weekly" 

PAYROLL_HEADERS = [
    "Date", "EmployeeID", "FullName",
    "HoursWorked", "HourlyRate",
    "GrossPay", "NIS", "EduTax", "NetPay"
]

def write_payroll_header():
    """Write header row only if file doesn't exist yet."""
    if not os.path.exists(PAYROLL_FILE):
        with open(PAYROLL_FILE, "w", newline="") as f:
            writer = csv.writer(f)
            writer.writerow(PAYROLL_HEADERS)


def save_payroll(emp_id, fullname, hours, rate, gross, nis, edu_tax, net):
    write_payroll_header()
    with open(PAYROLL_FILE, "a", newline="") as file:
        writer = csv.writer(file)
        writer.writerow([
            datetime.now().strftime("%Y-%m-%d"),
            emp_id, fullname,
            hours, rate,
            f"{gross:.2f}", f"{nis:.2f}", f"{edu_tax:.2f}", f"{net:.2f}"
        ])


def calculate_payroll():
    if not dataforp2.employees:
        print("\n\t\t[!] No employees loaded. Please load employees first.\n")
        return

    print(f"\n\t\t=== {PAYROLL_FREQUENCY} Payroll Processing ===\n")
    processed = 0

    for worker in dataforp2.employees:
        status = worker.get("Status", "").strip().lower()
        name   = worker.get("FullName", "Unknown")
        emp_id = worker.get("EmployeeID", "?")

        # Block non-active employees
        if status == "terminated":
            print(f"\t\t[BLOCKED] {name} is TERMINATED. Payroll skipped.")
            continue
        if status == "on leave":
            print(f"\t\t[BLOCKED] {name} is ON LEAVE. Payroll skipped.")
            continue
        if status != "active":
            print(f"\t\t[SKIPPED] {name} — unknown status '{status}'.")
            continue

        # Get hours
        try:
            hours = float(input(f"\t\tEnter hours worked for {name}: "))
            if hours < 0:
                print("\t\tInvalid hours. Skipping.")
                continue
        except ValueError:
            print("\t\tInvalid input. Skipping.")
            continue

        rate    = float(worker.get("HourlyRate", 0))
        gross   = hours * rate
        nis     = gross * 0.025
        edu_tax = (gross - nis) * 0.0225
        net     = gross - nis - edu_tax

        save_payroll(emp_id, name, hours, rate, gross, nis, edu_tax, net)
        processed += 1

        print(f"\n\t\t--- Payroll Result: {name} ---")
        print(f"\t\tGross Pay     : ${gross:.2f}")
        print(f"\t\tNIS (2.5%)    : ${nis:.2f}")
        print(f"\t\tEdu Tax(2.25%): ${edu_tax:.2f}")
        print(f"\t\tNet Pay       : ${net:.2f}\n")

    print(f"\n\t\tPayroll complete. {processed} employee(s) processed.\n")


if __name__ == '__main__':
    calculate_payroll()