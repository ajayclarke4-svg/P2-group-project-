import load_employees
import viewEmployee
import payroll
import payroll_report
import time
import os
import sys
from colorama import Fore, Back, Style, init #used for visual design
init(autoreset=True)

def pause():#this function is for design purpose only
# Purpose: Pauses the program and waits for user input
# Description:
# This function prompts the user to press Enter before continuing.
# It is used for user interface design to allow the user time
# to read information before the screen is cleared.
    input(Fore.CYAN + "\n\t\t\t\t\t\tPress Enter to continue...")
    clear_screen()

def database_check():
# Purpose: Checks database files
# Description:
# This function checks if required CSV files exist before the
# system starts. It verifies employee_records.csv and payroll.csv
# and displays the status of each file. If payroll.csv does not
# exist, it informs the user that it will be created.
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

def clear_screen():#this function is for design purpose only
# Purpose: Clears the console screen
# Description:
# This function clears the terminal screen to improve user
# interface appearance. It only runs on Windows systems and
# helps keep the display clean and organized.
    if os.name == 'nt':
        time.sleep(1) 
        os.system('cls')


def cafe_header():#this function is for design purpose only
# Purpose: Displays system header
# Description:
# This function prints the Byte Cafe system banner using
# ASCII art. It is used for visual design and improves
# the user interface experience.
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

def boot_loading():#this function is for design purpose only
# Purpose: Displays system boot loading animation
# Description:
# This function shows a loading progress bar when the
# system starts. It improves user experience by simulating
# system booting.
    print(Fore.CYAN + Style.BRIGHT + "\nBOOTING BYTE CAFE SYSTEM...\n")
    
    length = 30
    
    for i in range(length + 1):
        bar = "#" * i + "-" * (length - i)
        percent = int((i / length) * 100)
    
        sys.stdout.write(f"\r[{bar}] {percent}%")
        sys.stdout.flush()
        time.sleep(0.06)
    
    print(Fore.GREEN + Style.BRIGHT + "\nSYSTEM READY\n")

def bootdown_loading():#this function is for design purpose only
# Purpose: Displays system exit loading animation
# Description:
# This function shows a loading progress bar when the
# program exits. It provides a smooth shutdown animation
# for better user experience.
    print(Fore.CYAN + Style.BRIGHT + "\nEXITING BYTE CAFE SYSTEM...\n")
    
    length = 30
    
    for i in range(length + 1):
        bar = "#" * i + "-" * (length - i)
        percent = int((i / length) * 100)
    
        sys.stdout.write(f"\r[{bar}] {percent}%")
        sys.stdout.flush()
        time.sleep(0.06)

def menu():
# Purpose: Displays main system menu
# Description:
# This function controls the main program flow. It displays
# the menu options and allows the user to select different
# features such as loading employees, viewing employees,
# calculating payroll, viewing reports, and exiting the system.
# It calls the appropriate functions based on user input.
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
        print(Fore.WHITE + "\t\t\t\t\t\t= 6. Load Final Payroll Report         =")
        print(Fore.WHITE + "\t\t\t\t\t\t= q. Exit                              =")
        print(Fore.CYAN + Style.BRIGHT +"\t\t\t\t\t\t========================================")
    
        choice = input(Fore.LIGHTCYAN_EX + Style.BRIGHT + "\t\t\t\t\t\tEnter your choice: ").strip().lower()#asks the user for info. the lower() function reduces chances of bugs
        if choice == "q":# lets user stop the program, maed it a letter to reduce chances of user error, why using the program
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
        elif choice == "6":
            print(Fore.GREEN +"\t\t\t\t\t\tLoad Final Payroll Report selected\n")
            payroll_report.generate_payroll_report()
            pause()
        else:#safety net for user erorrs, wtih inputs
            print(Fore.RED+"\t\t\t\t\t\tInvalid choice! Try again...\n")
if __name__ == '__main__':
    menu()
