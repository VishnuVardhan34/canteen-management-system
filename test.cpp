#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <thread>
#include <chrono>
#include <algorithm> // For case-insensitive string comparison

using namespace std;

// Base class Person
class Person {
protected:
    string name;
    int id;
    string password;

public:
    Person(string n, int i, string pass) : name(n), id(i), password(pass) {}
    Person(string n, string pass) : name(n), password(pass) {}  // Corrected constructor for Admin
    virtual void displayMenu() = 0; // Pure virtual function
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

    struct InventoryItem {
        string itemName;
        int quantity;
        double price;
    };

    vector<EmployeeData> employeeData;
    vector<InventoryItem> inventory;

    // Helper for case-insensitive string comparison
    bool caseInsensitiveMatch(const string &a, const string &b) {
        return equal(a.begin(), a.end(), b.begin(), b.end(),
                     [](char a, char b) { return tolower(a) == tolower(b); });
    }

public:
    Admin(string n, string pass) : Person(n, pass) {}

    void addEmployee() {
        EmployeeData newEmp;
        cout << "Enter Employee Name: ";
        cin >> newEmp.name;

        // Check if the name already exists
        for (const auto &emp : employeeData) {
            if (caseInsensitiveMatch(emp.name, newEmp.name)) {
                cout << "Employee with this name already exists. Please enter a different name.\n";
                return;
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
        for (const auto &emp : employeeData) {
            if (emp.empID == newEmp.empID) {
                cout << "Employee with this ID already exists. Please enter a different ID.\n";
                return;
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
                if (caseInsensitiveMatch(it->name, empName)) {
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

    void editEmployee() {
        int empID;
        cout << "Enter Employee ID to edit: ";
        cin >> empID;

        for (auto &emp : employeeData) {
            if (emp.empID == empID) {
                cout << "Editing Employee: " << emp.name << "\n";
                cout << "Enter new name: ";
                cin >> emp.name;
                cout << "Enter new age: ";
                cin >> emp.age;
                cout << "Enter new salary: ";
                cin >> emp.salary;
                cout << "Employee details updated successfully!\n";
                return;
            }
        }
        cout << "Employee not found.\n";
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

    void addItemToInventory() {
        InventoryItem newItem;
        cout << "Enter item name: ";
        cin >> newItem.itemName;
        cout << "Enter quantity: ";
        cin >> newItem.quantity;
        cout << "Enter price: ";
        cin >> newItem.price;

        inventory.push_back(newItem);
        cout << "Item added to inventory successfully!\n";
    }

    void viewInventory() {
        cout << "\n=============================================\n";
        cout << setw(10) << left << "Item Name" << setw(10) << "Quantity" << setw(10) << "Price" << endl;
        cout << "=============================================\n";
        for (const auto &item : inventory) {
            cout << setw(10) << left << item.itemName
                 << setw(10) << item.quantity
                 << "$" << setw(9) << item.price << endl;
        }
        cout << "=============================================\n";
    }

    vector<InventoryItem>& getInventory() { return inventory; }

    void displayMenu() override {
        int choice;
        do {
            cout << "\nAdmin Menu:\n1. Add Employee\n2. Delete Employee\n3. Edit Employee\n4. View Employees\n"
                 << "5. Add Inventory Item\n6. View Inventory\n7. Exit\n";
            cin >> choice;
            switch (choice) {
                case 1: addEmployee(); break;
                case 2: deleteEmployee(); break;
                case 3: editEmployee(); break;
                case 4: viewEmployees(); break;
                case 5: addItemToInventory(); break;
                case 6: viewInventory(); break;
                case 7: cout << "Exiting Admin Menu.\n"; break;
                default: cout << "Invalid option!\n";
            }
        } while (choice != 7);
    }
};

// Derived class Employee
class Employee : public Person {
private:
    struct Order {
        string itemName;  // Name of the food item
        int quantity;     // Quantity of the food item
        int orderNumber;  // Unique order number
    };

    vector<Order> foodItems; // Vector to store food orders
    int orderCounter;        // Counter for generating unique order numbers

public:
    Employee(string n, int i, string pass) : Person(n, i, pass), orderCounter(1000) {}

    // Function for ordering food
    void orderFood() {
        Order newOrder;
        char continueOrder;

        do {
            cout << "Enter food item: ";
            cin >> newOrder.itemName; // Input food item name
            cout << "Enter quantity: ";
            cin >> newOrder.quantity;  // Input quantity

            newOrder.orderNumber = ++orderCounter; // Generate unique order number
            foodItems.push_back(newOrder); // Add order to the vector

            cout << "Order placed successfully! Order Number: " << newOrder.orderNumber << endl;

            cout << "Do you want to order another item? (y/n): ";
            cin >> continueOrder; // Ask if the user wants to order more items

        } while (continueOrder == 'y' || continueOrder == 'Y');
    }

    // Function to search for an order by order number
    void searchOrder(int num) {
        for (const auto &order : foodItems) {
            if (order.orderNumber == num) {
                cout << "Order found: Item: " << order.itemName
                     << ", Quantity: " << order.quantity
                     << ", Order Number: " << order.orderNumber << endl;
                return; // Exit the function if the order is found
            }
        }
        throw invalid_argument("Order number not found"); // Throw error if not found
    }

    // Function to generate a bill for recent orders
    void generateBill() {
        cout << "Generating bill for recent orders:\n";
        for (const auto &order : foodItems) {
            cout << "Item: " << order.itemName
                 << " | Quantity: " << order.quantity
                 << " | Price: $" << (rand() % 50 + 10) * order.quantity // Example price generation
                 << " | Order Number: " << order.orderNumber << endl;
        }
        cout << "Bill generated successfully!\n";
    }

    void displayMenu() override {
        int choice;
        do {
            cout << "\nEmployee Menu:\n1. Order Food\n2. Search Order\n3. Generate Bill\n4. Exit\n";
            cin >> choice;
            switch (choice) {
                case 1: orderFood(); break;
                case 2: {
                    int orderNumber;
                    cout << "Enter order number to search: ";
                    cin >> orderNumber;
                    try {
                        searchOrder(orderNumber);
                    } catch (const invalid_argument &e) {
                        cout << e.what() << endl; // Display error if order not found
                    }
                    break;
                }
                case 3: generateBill(); break;
                case 4: cout << "Exiting Employee Menu.\n"; break;
                default: cout << "Invalid option!\n";
            }
        } while (choice != 4);
    }
};

// Function to authenticate Admin
bool authenticateAdmin(string username, string password, Person *&user) {
    string storedPassword = "admin123"; // Example admin password
    if (username == "admin" && password == storedPassword) {
        user = new Admin(username, password); // Create Admin object
        return true; // Successful admin login
    }
    cout << "Authentication failed. Please try again.\n";
    return false; // Failed admin login
}

// Function to authenticate Employee
bool authenticateEmployee(string username, int id, string password, Person *&user, Admin *adminRef) {
    string storedPassword = "emp123"; // Example employee password
    if (password == storedPassword) {
        user = new Employee(username, id, password); // Corrected constructor usage
        return true; // Successful employee login
    }
    cout << "Authentication failed. Please try again.\n";
    return false; // Failed employee login
}

// Main function
int main() {
    Person *user = nullptr; // Pointer to hold authenticated user
    int choice;

    do {
        cout << "\nWelcome to Canteen Management System\n";
        cout << "1. Admin Login\n2. Employee Login\n3. Exit\n";
        cin >> choice;

        switch (choice) {
            case 1: {
                string username, password;
                cout << "Enter Admin Username: ";
                cin >> username;
                cout << "Enter Admin Password: ";
                cin >> password;

                if (authenticateAdmin(username, password, user)) {
                    Admin *admin = dynamic_cast<Admin *>(user); // Cast to Admin
                    admin->displayMenu(); // Show Admin menu
                }
                break;
            }
            case 2: {
                string username, password;
                int id;
                cout << "Enter Employee Name: ";
                cin >> username;
                cout << "Enter Employee ID: ";
                cin >> id;
                cout << "Enter Employee Password: ";
                cin >> password;

                Admin *adminRef = nullptr; // Reference for admin, if needed
                if (authenticateEmployee(username, id, password, user, adminRef)) {
                    Employee *employee = dynamic_cast<Employee *>(user); // Cast to Employee
                    employee->displayMenu(); // Show Employee menu
                }
                break;
            }
            case 3: {
                cout << "Exiting the system. Thank you!\n";
                break;
            }
            default: {
                cout << "Invalid option! Please choose again.\n";
            }
        }
    } while (choice != 3);

    // Cleanup
    delete user; // Delete dynamically allocated user
    return 0;
}
