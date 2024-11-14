#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
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

    // Helper for case-insensitive string comparison
    bool caseInsensitiveMatch(const string &a, const string &b) {
        return equal(a.begin(), a.end(), b.begin(), b.end(),
                     [](char a, char b) { return tolower(a) == tolower(b); });
    }

    // Helper function to write inventory to file
    void writeToFile(const InventoryItem &item) {
        ofstream outFile("inv.csv", ios::app); // Open in append mode
        if (outFile.is_open()) {
            outFile << item.itemName << "," << item.quantity << "," << item.price << endl;
            outFile.close();
        } else {
            cout << "Unable to open inventory file for writing.\n";
        }
    }

    // Helper function to read inventory from file
    vector<InventoryItem> readFromFile() {
        vector<InventoryItem> inventory;
        ifstream inFile("inv.csv");
        if (inFile.is_open()) {
            string line;
            while (getline(inFile, line)) {
                size_t pos1 = line.find(",");
                size_t pos2 = line.find(",", pos1 + 1);

                if (pos1 == string::npos || pos2 == string::npos) {
                    continue; // Skip malformed lines
                }

                string itemName = line.substr(0, pos1);
                int quantity = stoi(line.substr(pos1 + 1, pos2 - pos1 - 1));
                double price = stod(line.substr(pos2 + 1));

                inventory.push_back({itemName, quantity, price});
            }

            inFile.close();
        } else {
            cout << "Unable to open inventory file for reading.\n";
        }

        return inventory;
    }

    // Write employee data to file
    void writeEmployeeToFile(const EmployeeData &emp) {
        ofstream outFile("employee_details.csv", ios::app);
        if (outFile.is_open()) {
            outFile << emp.name << "," << emp.age << "," << emp.empID << "," << emp.salary << endl;
            outFile.close();
        } else {
            cout << "Unable to open employee details file for writing.\n";
        }
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

        // Write to employee_details.csv
        writeEmployeeToFile(newEmp);

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
        if (employeeData.empty()) {
            cout << "No employees to display.\n";
            return;
        }

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

        // Write the new item to the CSV file
        writeToFile(newItem);

        cout << "Item added to inventory successfully!\n";
    }

    void viewInventory() {
        vector<InventoryItem> inventory = readFromFile();

        if (!inventory.empty()) {
            cout << "\n=============================================\n";
            cout << setw(15) << left << "Item Name" << setw(10) << "Quantity" << setw(10) << "Price" << endl;
            cout << "=============================================\n";
            for (const auto &item : inventory) {
                cout << setw(15) << left << item.itemName
                     << setw(10) << item.quantity
                     << "$" << setw(9) << item.price << endl;
            }
            cout << "=============================================\n";
        } else {
            cout << "Inventory is empty.\n";
        }
    }

    void displayMenu() override {
        int choice;
        do {
            cout << "\nAdmin Menu:\n1. Add Employee\n2. Delete Employee\n3. Edit Employee\n4. View Employees\n"
                 << "5. Add Inventory Item\n6. View Inventory\n7. Logout\n";
            cin >> choice;
            switch (choice) {
                case 1: addEmployee(); break;
                case 2: deleteEmployee(); break;
                case 3: editEmployee(); break;
                case 4: viewEmployees(); break;
                case 5: addItemToInventory(); break;
                case 6: viewInventory(); break;
                case 7: cout << "Logging out of Admin Menu.\n"; break;
                default: cout << "Invalid option!\n";
            }
        } while (choice != 7);
    }
};

// Derived class Employee
class Employee : public Person {
private:
    struct InventoryItem {
        string itemName;
        int quantity;
        double price;
    };

    void writeOrderToFile(const string &orderDetails) {
        ofstream outFile("orders.csv", ios::app);
        if (outFile.is_open()) {
            outFile << orderDetails << endl;
            outFile.close();
        } else {
            cout << "Unable to open orders file for writing.\n";
        }
    }

    // Read inventory from file
    vector<InventoryItem> readFromFile() {
        vector<InventoryItem> inventory;
        ifstream inFile("inv.csv");
        if (inFile.is_open()) {
            string line;
            while (getline(inFile, line)) {
                size_t pos1 = line.find(",");
                size_t pos2 = line.find(",", pos1 + 1);

                if (pos1 == string::npos || pos2 == string::npos) {
                    continue; // Skip malformed lines
                }

                string itemName = line.substr(0, pos1);
                int quantity = stoi(line.substr(pos1 + 1, pos2 - pos1 - 1));
                double price = stod(line.substr(pos2 + 1));

                inventory.push_back({itemName, quantity, price});
            }

            inFile.close();
        } else {
            cout << "Unable to open inventory file for reading.\n";
        }

        return inventory;
    }

public:
    Employee(string n, int i, string pass) : Person(n, i, pass) {}

    void orderItems() {
        vector<InventoryItem> inventory = readFromFile();

        if (inventory.empty()) {
            cout << "Inventory is empty.\n";
            return;
        }

        string orderedItems;
        double totalAmount = 0.0;
        int itemNumber, quantity;

        do {
            cout << "\nAvailable Inventory:\n";
            for (size_t i = 0; i < inventory.size(); ++i) {
                cout << i + 1 << ". " << inventory[i].itemName
                     << " (Price: $" << inventory[i].price
                     << ", Available Quantity: " << inventory[i].quantity << ")\n";
            }

            cout << "Enter item number to order (0 to finish): ";
            cin >> itemNumber;
            if (itemNumber == 0) {
                break;
            }

            if (itemNumber > 0 && itemNumber <= static_cast<int>(inventory.size())) {
                cout << "Enter quantity: ";
                cin >> quantity;

                InventoryItem &selectedItem = inventory[itemNumber - 1];

                if (quantity <= selectedItem.quantity) {
                    selectedItem.quantity -= quantity;
                    double itemTotal = quantity * selectedItem.price;
                    totalAmount += itemTotal;
                    orderedItems += selectedItem.itemName + " (x" + to_string(quantity) + "), ";
                } else {
                    cout << "Insufficient stock. Please try again.\n";
                }
            } else {
                cout << "Invalid item number. Please try again.\n";
            }
        } while (itemNumber != 0);

        if (!orderedItems.empty()) {
            orderedItems.pop_back(); // Remove trailing comma
            orderedItems.pop_back();
            string orderDetails = "Employee ID: " + to_string(id) + ", Items Ordered: " + orderedItems + ", Total Amount: $" + to_string(totalAmount);
            writeOrderToFile(orderDetails);

            cout << "Order placed successfully!\n";
            cout << "Items Ordered: " << orderedItems << endl;
            cout << "Total Amount: $" << totalAmount << endl;
        } else {
            cout << "No items were ordered.\n";
        }
    }

    void displayMenu() override {
        int choice;
        do {
            cout << "\nEmployee Menu:\n1. Order Items\n2. Logout\n";
            cin >> choice;
            switch (choice) {
                case 1: orderItems(); break;
                case 2: cout << "Logging out of Employee Menu.\n"; break;
                default: cout << "Invalid option!\n";
            }
        } while (choice != 2);
    }
};

int main() {
    int userType;
    cout << "Welcome to Canteen Management System\n";
    cout << "1. Admin\n2. Employee\nChoose user type: ";
    cin >> userType;

    if (userType == 1) {
        string adminName, adminPassword;
        cout << "Enter Admin Name: ";
        cin >> adminName;
        cout << "Enter Password: ";
        cin >> adminPassword;

        Admin admin(adminName, adminPassword);
        admin.displayMenu();

    } else if (userType == 2) {
        string empName, empPassword;
        int empID;
        cout << "Enter Employee ID: ";
        cin >> empID;
        cout << "Enter Employee Name: ";
        cin >> empName;
        cout << "Enter Password: ";
        cin >> empPassword;

        Employee emp(empName, empID, empPassword);
        emp.displayMenu();
    } else {
        cout << "Invalid user type!\n";
    }

    return 0;
}

