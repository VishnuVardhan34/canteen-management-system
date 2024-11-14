#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <thread>
#include <chrono>

using namespace std;

// Base class Person
class Person {
protected:
    string name;
    int id;
    string password;

public:
    Person(string n, int i, string pass) : name(n), id(i), password(pass) {}
    Person(string n, string pass) : name(n), password(pass) {}  // Constructor without ID for Admin
    virtual void displayMenu() = 0;  // Pure virtual function
};

// Derived class Admin
class Admin : public Person {
private:
    struct EmployeeData {
        string name;
        int age;
        int empID;
        double salary;
    };
    vector<EmployeeData> employeeData;

public:
    Admin(string n, string pass) : Person(n, pass) {}

    void addEmployee() {
        EmployeeData newEmp;
        cout << "Enter Employee Name: ";
        cin >> newEmp.name;

        // Check if the name already exists
        for (const auto& emp : employeeData) {
            if (emp.name == newEmp.name) {
                cout << "Employee with this name already exists. Please enter a different name.\n";
                return;  // Exit the function if name already exists
            }
        }

        // Age validation
        do {
            cout << "Enter Employee Age (should not exceed 85): ";
            cin >> newEmp.age;
            if (newEmp.age > 85) {
                cout << "Invalid age. Age cannot be greater than 85. Please re-enter.\n";
            }
        } while (newEmp.age > 85);

        cout << "Enter Employee ID: ";
        cin >> newEmp.empID;

        // Check if the ID already exists
        for (const auto& emp : employeeData) {
            if (emp.empID == newEmp.empID) {
                cout << "Employee with this ID already exists. Please enter a different ID.\n";
                return;  // Exit the function if ID already exists
            }
        }

        cout << "Enter Employee Salary: ";
        cin >> newEmp.salary;

        employeeData.push_back(newEmp);
        cout << "Employee added successfully!\n";
    }

    void deleteEmployee() {
        int choice;
        cout << "Delete Employee by:\n1. Name\n2. ID\nEnter choice: ";
        cin >> choice;

        string empName;
        int empID;
        bool found = false;

        if (choice == 1) {
            cout << "Enter Employee Name to delete: ";
            cin >> empName;
            for (auto it = employeeData.begin(); it != employeeData.end(); ++it) {
                if (it->name == empName) {
                    employeeData.erase(it);
                    cout << "Employee " << empName << " deleted successfully!\n";
                    found = true;
                    break;
                }
            }
        } else if (choice == 2) {
            cout << "Enter Employee ID to delete: ";
            cin >> empID;
            for (auto it = employeeData.begin(); it != employeeData.end(); ++it) {
                if (it->empID == empID) {
                    employeeData.erase(it);
                    cout << "Employee with ID " << empID << " deleted successfully!\n";
                    found = true;
                    break;
                }
            }
        } else {
            cout << "Invalid option!\n";
        }

        if (!found) {
            cout << "Employee not found.\n";
        }
    }

    void viewEmployees() {
        cout << "\n=============================================\n";
        cout << setw(10) << left << "Name" << setw(10) << "Age" << setw(10) << "ID" << setw(10) << "Salary" << endl;
        cout << "=============================================\n";
        for (const auto &emp : employeeData) {
            cout << setw(10) << left << emp.name
                 << setw(10) << emp.age
                 << setw(10) << emp.empID
                 << "$" << setw(9) << emp.salary << endl;
        }
        cout << "=============================================\n";
    }

    // Ordering Multiple Items in Bulk
    void orderItems() {
        string itemName;
        int quantity;
        char continueOrder;

        do {
            cout << "Enter item name: ";
            cin >> itemName;
            cout << "Enter quantity: ";
            cin >> quantity;

            cout << "Item: " << itemName << " | Quantity: " << quantity << " added to the order.\n";

            cout << "Do you want to order another item? (y/n): ";
            cin >> continueOrder;

        } while (continueOrder == 'y' || continueOrder == 'Y');

        cout << "Bulk order complete.\n";
    }

    void displayMenu() override {
        int choice;
        do {
            cout << "\nAdmin Menu:\n1. Add Employee\n2. Delete Employee\n3. View Employees\n4. Order Items in Bulk\n5. Exit\n";
            cin >> choice;
            try {
                switch (choice) {
                case 1:
                    addEmployee();
                    break;
                case 2:
                    deleteEmployee();
                    break;
                case 3:
                    viewEmployees();
                    break;
                case 4:
                    orderItems();
                    break;
                case 5:
                    cout << "Exiting Admin Menu.\n";
                    break;
                default:
                    cout << "Invalid option!\n";
                }
            } catch (exception &e) {
                cout << "Error: " << e.what() << endl;
            }
        } while (choice != 5);
    }
};

// Derived class Employee
class Employee : public Person {
private:
    struct Order {
        string itemName;
        int quantity;
        int orderNumber;
    };

    vector<Order> foodItems;
    int orderCounter;

public:
    Employee(string n, int i, string pass) : Person(n, i, pass), orderCounter(1000) {}

    void orderFood() {
        Order newOrder;
        char continueOrder;

        do {
            cout << "Enter food item: ";
            cin >> newOrder.itemName;
            cout << "Enter quantity: ";
            cin >> newOrder.quantity;

            newOrder.orderNumber = ++orderCounter;
            foodItems.push_back(newOrder);

            cout << "Order placed successfully! Order Number: " << newOrder.orderNumber << endl;

            cout << "Do you want to order another item? (y/n): ";
            cin >> continueOrder;

        } while (continueOrder == 'y' || continueOrder == 'Y');
    }

    void searchOrder(int num) {
        for (const auto &order : foodItems) {
            if (order.orderNumber == num) {
                cout << "Order found: Item: " << order.itemName
                     << ", Quantity: " << order.quantity
                     << ", Order Number: " << order.orderNumber << endl;
                return;
            }
        }
        throw invalid_argument("Order number not found");
    }

    void generateBill() {
        cout << "Generating bill for recent orders:\n";
        for (const auto &order : foodItems) {
            cout << "Item: " << order.itemName
                 << " | Quantity: " << order.quantity
                 << " | Price: $" << (rand() % 50 + 10) * order.quantity << endl;
        }
    }

    void displayMenu() override {
        int choice;
        do {
            cout << "\nEmployee Menu:\n1. Order Food\n2. Search Order\n3. Generate Bill\n4. Exit\n";
            cin >> choice;
            try {
                switch (choice) {
                case 1:
                    orderFood();
                    break;
                case 2:
                    {
                        int searchOrderNum;
                        cout << "Enter order number to search: ";
                        cin >> searchOrderNum;
                        searchOrder(searchOrderNum);
                    }
                    break;
                case 3:
                    generateBill();
                    break;
                case 4:
                    cout << "Exiting Employee Menu.\n";
                    break;
                default:
                    cout << "Invalid option!\n";
                }
            } catch (exception &e) {
                cout << "Error: " << e.what() << endl;
            }
        } while (choice != 4);
    }
};

// Function to simulate an animated login page
void showLoginAnimation() {
    cout << "********** Welcome to Canteen Management System **********\n";
    cout << "Loading...\n";
    for (int i = 0; i < 3; i++) {
        cout << ".";
        this_thread::sleep_for(chrono::milliseconds(500));
    }
    cout << "\nLogin Page Loaded!\n";
}

// Function to authenticate admin
bool authenticateAdmin(string name, string password, Person *&user) {
    string adminName = "admin"; // Admin's name (hardcoded for this example)
    string storedPassword = "admin123"; // Admin's password (hardcoded)

    // Check if the name is "admin" and the password matches
    if (name == adminName && password == storedPassword) {
        user = new Admin(name, password);
        return true;  // Successful admin login
    }
    cout << "Authentication failed. Please try again.\n";
    return false;  // Failed admin login
}

// Function to authenticate employee
bool authenticateEmployee(string username, int id, string password, Person *&user) {
    string storedPassword = "password"; // Employee's password (hardcoded)
    // Check if the name is "admin" (to prevent admin login as employee)
    if (username == "admin") {
        cout << "You cannot log in as an employee with admin name.\n";
        return false; // Prevent login with admin name
    }

    // Check if the provided password matches
    if (password == storedPassword) {
        user = new Employee(username, id, password);
        return true;  // Successful employee login
    }
    cout << "Authentication failed. Please try again.\n";
    return false;  // Failed employee login
}

int main() {
    int choice;
    Person *user = nullptr;

    showLoginAnimation();

    do {
        cout << "\nMain Menu:\n1. Admin Login\n2. Employee Login\n3. Exit\n";
        cin >> choice;

        try {
            string name;
            int id;
            string password;

            switch (choice) {
            case 1:
                cout << "Enter Admin Name: ";
                cin >> name;
                cout << "Enter Admin Password: ";
                cin >> password;
                if (authenticateAdmin(name, password, user)) {
                    user->displayMenu();
                }
                break;
            case 2:
                cout << "Enter Employee Name: ";
                cin >> name;

                // Ensure employee does not log in with admin name
                while (name == "admin") {
                    cout << "You cannot log in as an employee with admin name. Please enter a valid employee name: ";
                    cin >> name;
                }

                cout << "Enter Employee ID: ";
                cin >> id;
                cout << "Enter Employee Password: ";
                cin >> password;
                // Authenticate and check login
                if (authenticateEmployee(name, id, password, user)) {
                    user->displayMenu();
                } else {
                    cout << "Login failed. Exiting...\n";
                    break;  // Break the loop on failed login
                }
                break;
            case 3:
                cout << "Exiting System. Thank you!\n";
                break;
            default:
                cout << "Invalid option!\n";
            }

            if (user != nullptr) {
                delete user;  // Free memory after login
                user = nullptr;
            }

        } catch (exception &e) {
            cout << "Error: " << e.what() << endl;
        }

    } while (choice != 3);  // Exit when the user selects the "Exit" option
    return 0;
}
