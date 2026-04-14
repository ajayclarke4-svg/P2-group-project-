
import csv
import os
from datetime import datetime

PAYROLL_FILE = "payroll.csv"
REPORT_FILE = "summary_report.csv"


def load_payroll_data():
    if not os.path.exists(PAYROLL_FILE):
        print("\n\t\t[!] payroll.csv not found. Run payroll first.\n")
        return []

    records = []
    with open(PAYROLL_FILE, newline="") as f:
        reader = csv.DictReader(f)
        for row in reader:
            records.append(row)

    return records


def generate_payroll_report():
    records = load_payroll_data()
    if not records:
        return

    total_net = 0.0
    total_nis = 0.0
    total_edu_tax = 0.0
    total_gross = 0.0
    net_by_employee = {}

    for r in records:
        net     = float(r.get("net", 0))
        nis     = float(r.get("nis", 0))
        edu_tax = float(r.get("tax", 0))
        gross   = float(r.get("gross", 0))

        emp_id = r.get("EmployeeID", "?")
        name   = r.get("FullName", "Unknown")

        total_net += net
        total_nis += nis
        total_edu_tax += edu_tax
        total_gross += gross

        if emp_id not in net_by_employee:
            net_by_employee[emp_id] = {"name": name, "total_net": 0.0}

        net_by_employee[emp_id]["total_net"] += net

    total_deductions = total_nis + total_edu_tax

    # Top 3 earners
    sorted_emp = sorted(
        net_by_employee.values(),
        key=lambda x: x["total_net"],
        reverse=True
    )

    top3 = sorted_emp[:3]

    export_report(
        total_gross,
        total_nis,
        total_edu_tax,
        total_deductions,
        total_net,
        len(records),
        top3
    )

    print("\n\t\tPayroll report generated successfully!\n")


def export_report(gross, nis, edu_tax, deductions, net, count, top3):
    with open(REPORT_FILE, "w", newline="") as f:
        writer = csv.writer(f)

        writer.writerow(["Report Generated", datetime.now().strftime("%Y-%m-%d %H:%M")])
        writer.writerow([])

        writer.writerow(["SUMMARY"])
        writer.writerow(["Total Gross Pay", f"${gross:.2f}"])
        writer.writerow(["Total NIS", f"${nis:.2f}"])
        writer.writerow(["Total Education Tax", f"${edu_tax:.2f}"])
        writer.writerow(["Total Deductions", f"${deductions:.2f}"])
        writer.writerow(["Total Net Pay", f"${net:.2f}"])
        writer.writerow(["Total Transactions", count])

        writer.writerow([])

        writer.writerow(["TOP 3 EARNERS"])
        writer.writerow(["Rank", "Name", "Net Pay"])

        for rank, emp in enumerate(top3, start=1):
            writer.writerow([rank, emp["name"], f"${emp['total_net']:.2f}"])


if __name__ == "__main__":
    generate_payroll_report()
