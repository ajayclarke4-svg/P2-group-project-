import load_employees
import viewEmployee
import payroll
import report

def menu():
    while True:
            print("\t\t\t\t\t\t========= Payroll System =========")
            print("\t\t\t\t\t\t= 1. Load Employees              =")
            print("\t\t\t\t\t\t= 2. View Employee List ( filter by status) =")
            print("\t\t\t\t\t\t= 3. Calculate Payroll           =")
            print("\t\t\t\t\t\t= 4. View Payroll Summary Report =")
            print("\t\t\t\t\t\t= q. Exit                        =")
            print("\t\t\t\t\t\t==================================")

            choice = input("\t\t\t\t\t\tEnter your choice: ").strip().lower()
            if choice == "q":
                print("\n\t\tGoodbye!\n")
                break
            elif choice == "1":
                load_employees.load_employees()
            elif choice == "2":
                viewEmployee.view_employees()
            elif choice == "3":
                payroll.calculate_payroll()
            elif choice == "4":
                report.view_payroll_report()
            else:
                print("\n\t\t[!] Invalid choice. Please try again.\n")

menu()