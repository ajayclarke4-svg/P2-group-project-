import csv
import os
import dataforp2

PAYROLL_FILE = "payroll.csv"

def load_payroll_history():
    """Returns a dict keyed by EmployeeID with their last payroll record."""
    history = {}
    if not os.path.exists(PAYROLL_FILE):
        return history
    try:
        with open(PAYROLL_FILE, newline="") as f:
            reader = csv.DictReader(f)
            for row in reader:
                # keep overwriting so we end up with the latest entry
                history[row["EmployeeID"]] = row
    except Exception:
        pass
    return history


def view_employees():
    if not dataforp2.employees:
        print("\n\t\t[!] No employees loaded. Please load employees first.\n")
        return

    print("\n\t\tFilter by status:")
    print("\t\t  1. Active")
    print("\t\t  2. On Leave")
    print("\t\t  3. Terminated")
    print("\t\t  4. All")
    status_choice = input("\t\tEnter choice: ").strip()

    filter_map = {"1": "active", "2": "on leave", "3": "terminated", "4": "all"}
    status_filter = filter_map.get(status_choice, "all")

    history = load_payroll_history()

    found_any = False
    for worker in dataforp2.employees:
        worker_status = worker.get("Status", "").strip().lower()
        if status_filter != "all" and worker_status != status_filter:
            continue

        found_any = True
        emp_id = worker.get("EmployeeID", "N/A")
        pay = history.get(emp_id)

        print("\n\t\t" + "-" * 45)
        print(f"\t\tID      : {emp_id}")
        print(f"\t\tName    : {worker.get('FullName', 'N/A')}")
        print(f"\t\tRole    : {worker.get('Role', 'N/A')}")
        print(f"\t\tStatus  : {worker.get('Status', 'N/A')}")
        print(f"\t\t--- Last Payslip ---")
        if pay:
            print(f"\t\tDate    : {pay.get('Date', 'None')}")
            print(f"\t\tGross   : ${float(pay.get('GrossPay', 0)):.2f}")
            print(f"\t\tNIS     : ${float(pay.get('NIS', 0)):.2f}")
            print(f"\t\tEdu Tax : ${float(pay.get('EduTax', 0)):.2f}")
            print(f"\t\tNet Pay : ${float(pay.get('NetPay', 0)):.2f}")
        else:
            print("\t\tDate    : None")
            print("\t\tGross   : None")
            print("\t\tNIS     : None")
            print("\t\tEdu Tax : None")
            print("\t\tNet Pay : None")

    if not found_any:
        print(f"\n\t\tNo employees found with status: {status_filter}\n")

    print("\t\t" + "-" * 45 + "\n")

if __name__ == '__main__':
    view_employees()