#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
using namespace std;

// Global salary components (modifiable via menu)
float da_rate = 0.10;
float hra_rate = 0.10;
int medical_allowance = 1000;
int professional_tax = 200;

// Employee Base Class
class Employee {
public:
    int emp_no;
    string emp_name, emp_add, emp_dept, emp_desg;

    virtual void getData() {
        cout << "Enter Employee No: "; cin >> emp_no;
        cin.ignore();
        cout << "Enter Name: "; getline(cin, emp_name);
        cout << "Enter Address: "; getline(cin, emp_add);
        cout << "Enter Department: "; getline(cin, emp_dept);
        cout << "Enter Designation: "; getline(cin, emp_desg);
    }

    virtual void displayData() const {
        cout << "\nEmployee No: " << emp_no;
        cout << "\nName: " << emp_name;
        cout << "\nAddress: " << emp_add;
        cout << "\nDepartment: " << emp_dept;
        cout << "\nDesignation: " << emp_desg << endl;
    }

    virtual void generatePaySlip() = 0; // Pure virtual
};

// Permanent Employee Class
class PermanentEmployee : public Employee {
public:
    float emp_basic, emp_da, emp_hra, emp_pf, gross, net;
    int income_tax;

    void getData() override {
        Employee::getData();
        cin.ignore(); // <-- Add this line to clear the newline
        cout << "Enter Basic Salary: ";
        cin >> emp_basic;
        cout << "Enter Income Tax: ";
        cin >> income_tax;
        calculate();
    }

    void calculate() {
        emp_da = da_rate * emp_basic;
        emp_hra = hra_rate * emp_basic;
        gross = emp_basic + emp_da + emp_hra + medical_allowance;
        emp_pf = 0.12 * (emp_basic + emp_da);
        net = gross - (emp_pf + professional_tax + income_tax);
    }

    void displayData() const override {
        Employee::displayData();
        cout << fixed << setprecision(2);
        cout << "Basic: " << emp_basic << ", DA: " << emp_da
             << ", HRA: " << emp_hra << ", MA: " << medical_allowance
             << "\nPF: " << emp_pf << ", PTax: " << professional_tax
             << ", ITax: " << income_tax << "\nGross: " << gross
             << ", Net: " << net << endl;
    }

    void generatePaySlip() override {
        cout << "\n--- PAY SLIP [Permanent Employee] ---\n";
        displayData();
    }
};

// Contractual Employee Class
class ContractualEmployee : public Employee {
public:
    float gross, net;
    int income_tax;

    void getData() override {
        Employee::getData();
        cin.ignore(); // <-- Add this line to clear the newline
        cout << "Enter Gross Salary: ";
        cin >> gross;
        cout << "Enter Income Tax: ";
        cin >> income_tax;
        calculate();
    }

    void calculate() {
        net = gross - (professional_tax + income_tax);
    }

    void displayData() const override {
        Employee::displayData();
        cout << fixed << setprecision(2);
        cout << "Gross: " << gross
             << ", PTax: " << professional_tax
             << ", ITax: " << income_tax
             << ", Net: " << net << endl;
    }

    void generatePaySlip() override {
        cout << "\n--- PAY SLIP [Contractual Employee] ---\n";
        displayData();
    }
};

// Storage
vector<PermanentEmployee> perm_emps;
vector<ContractualEmployee> cont_emps;

// Helper Functions
void setRates() {
    int choice;
    cout << "\n1. Set DA Rate\n2. Set HRA Rate\n3. Set Medical Allowance\n4. Set Professional Tax\nChoose: ";
    cin >> choice;
    switch (choice) {
        case 1: cout << "Enter new DA rate (as decimal): "; cin >> da_rate; break;
        case 2: cout << "Enter new HRA rate (as decimal): "; cin >> hra_rate; break;
        case 3: cout << "Enter new Medical Allowance: "; cin >> medical_allowance; break;
        case 4: cout << "Enter new Professional Tax: "; cin >> professional_tax; break;
        default: cout << "Invalid choice!\n";
    }
}

void searchEmployee() {
    int id;
    cout << "Enter Employee No to search: ";
    cin >> id;

    for (auto& e : perm_emps) {
        if (e.emp_no == id) {
            e.displayData();
            return;
        }
    }

    for (auto& e : cont_emps) {
        if (e.emp_no == id) {
            e.displayData();
            return;
        }
    }

    cout << "Employee not found.\n";
}

void computeOvertime() {
    int id, hours;
    cout << "Enter Employee No: ";
    cin >> id;

    for (auto& e : perm_emps) {
        if (e.emp_no == id) {
            cout << "Enter OT hours: "; cin >> hours;
            cout << "Overtime Pay: ₹" << (400 * hours) << endl;
            return;
        }
    }

    for (auto& e : cont_emps) {
        if (e.emp_no == id) {
            cout << "Enter OT hours: "; cin >> hours;
            cout << "Overtime Pay: ₹" << (400 * hours) << endl;
            return;
        }
    }

    cout << "Employee not found.\n";
}

void mainMenu() {
    int choice;
    while (true) {
        cout << "\n--- PAY SLIP GENERATION SYSTEM ---\n";
        cout << "1. Add Permanent Employee\n";
        cout << "2. Add Contractual Employee\n";
        cout << "3. Set Salary Components\n";
        cout << "4. Generate Pay Slips\n";
        cout << "5. Search Employee\n";
        cout << "6. Compute Overtime Pay\n";
        cout << "7. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                PermanentEmployee e;
                e.getData();
                perm_emps.push_back(e);
                break;
            }
            case 2: {
                ContractualEmployee e;
                e.getData();
                cont_emps.push_back(e);
                break;
            }
            case 3: setRates(); break;
            case 4:
                for (auto& e : perm_emps) e.generatePaySlip();
                for (auto& e : cont_emps) e.generatePaySlip();
                break;
            case 5: searchEmployee(); break;
            case 6: computeOvertime(); break;
            case 7: return;
            default: cout << "Invalid choice!\n";
        }
    }
}

int main() {
    mainMenu();
    return 0;
}
