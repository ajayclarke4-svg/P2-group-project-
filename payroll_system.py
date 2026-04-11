import load_employees
import viewEmployee
import payroll
import time
import os
import sys
from colorama import Fore, Back, Style, init #used for visual design
init(autoreset=True)

def pause():
    input(Fore.CYAN + "\n\t\t\t\t\t\tPress Enter to continue...")
    clear_screen()

def database_check():
    print(Fore.CYAN + "Checking system files...\n")

    # Employees file
    if os.path.exists("employee_records.csv"):
        print(Fore.GREEN + "Employees database ..... OK")
    else:
        print(Fore.RED + "Employees database ..... NOT FOUND")

    # Payroll file
    if os.path.exists("payroll.csv"):
        print(Fore.GREEN + "Payroll database ....... OK")
    else:
        print(Fore.YELLOW + "Payroll database ....... Will be created")

    print(Fore.GREEN + "\nSystem check complete\n")
    time.sleep(1)

def clear_screen():#rmr to check this
    if os.name == 'nt':
        time.sleep(1) 
        os.system('cls')


def cafe_header():
    tabs = "\t\t\t\t"

    header = [
        f"{tabs}██████╗ ██╗   ██╗████████╗███████╗     ██████╗ █████╗ ███████╗███████╗",
        f"{tabs}██╔══██╗╚██╗ ██╔╝╚══██╔══╝██╔════╝    ██╔════╝██╔══██╗██╔════╝██╔════╝",
        f"{tabs}██████╔╝ ╚████╔╝    ██║   █████╗      ██║     ███████║█████╗  █████╗",
        f"{tabs}██╔══██╗  ╚██╔╝     ██║   ██╔══╝      ██║     ██╔══██║██╔══╝  ██╔══╝",
        f"{tabs}██████╔╝   ██║      ██║   ███████╗    ╚██████╗██║  ██║██║     ███████╗",
        f"{tabs}╚═════╝    ╚═╝      ╚═╝   ╚══════╝     ╚═════╝╚═╝  ╚═╝╚═╝     ╚══════╝"
    ]

    print(Fore.CYAN + Style.BRIGHT)

    for line in header:
        print(line)
        time.sleep(0.2) 

def boot_loading():
    print(Fore.CYAN + Style.BRIGHT + "\nBOOTING BYTE CAFE SYSTEM...\n")

    length = 30

    for i in range(length + 1):
        bar = "#" * i + "-" * (length - i)
        percent = int((i / length) * 100)

        sys.stdout.write(f"\r[{bar}] {percent}%")
        sys.stdout.flush()
        time.sleep(0.06)

    print(Fore.GREEN + Style.BRIGHT + "\nSYSTEM READY\n")
def bootdown_loading():
    print(Fore.CYAN + Style.BRIGHT + "\nEXITING BYTE CAFE SYSTEM...\n")

    length = 30

    for i in range(length + 1):
        bar = "#" * i + "-" * (length - i)
        percent = int((i / length) * 100)

        sys.stdout.write(f"\r[{bar}] {percent}%")
        sys.stdout.flush()
        time.sleep(0.06)

def menu():
    boot_loading()
    database_check()
    clear_screen()
    while True:
        cafe_header()    
        print(Fore.CYAN + Style.BRIGHT +"\n\t\t\t\t\t\t============ PAYROLL SYSTEM =============")
        print(Fore.WHITE + "\t\t\t\t\t\t= 1. Load Employees                    =")
        print(Fore.WHITE + "\t\t\t\t\t\t= 2. View Employee List                =")
        print(Fore.WHITE + "\t\t\t\t\t\t= 3. Calculate Payroll                 =")
        print(Fore.WHITE + "\t\t\t\t\t\t= 4. View Payroll Summary Report       =")
        print(Fore.WHITE + "\t\t\t\t\t\t= 5. View Top 3 Earners                =")
        print(Fore.WHITE + "\t\t\t\t\t\t= q. Exit                              =")
        print(Fore.CYAN + Style.BRIGHT +"\t\t\t\t\t\t========================================")

        choice = input(Fore.LIGHTCYAN_EX + Style.BRIGHT + "\t\t\t\t\t\tEnter your choice: ").strip().lower()
        if choice == "q":
            bootdown_loading()
            clear_screen()
            break
        elif choice == "1":
            print(Fore.GREEN +"\t\t\t\t\t\tLoad Employees selected\n")
            load_employees.load_employees()
            pause()
        elif choice == "2":
            print(Fore.GREEN +"\t\t\t\t\t\tView Employee List selected\n")
            viewEmployee.view_employees()
            pause()
        elif choice == "3":
            print(Fore.GREEN +"\t\t\t\t\t\tCalculate Payroll selected\n")
            payroll.calculate_payroll()
            pause()
        elif choice == "4":
            print(Fore.GREEN +"\t\t\t\t\t\tView Payroll Summary Report selected\n")
            viewEmployee.payroll_summary()
            pause()
        elif choice == "5":
            print(Fore.GREEN +"\t\t\t\t\t\tView Top 3 Earners selected\n")
            payroll.top_3_highest_paid()
            pause()
        else:
            print(Fore.RED+"\t\t\t\t\t\tInvalid choice! Try again...\n")
menu()