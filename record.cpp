#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
using namespace std;

struct Record {
    int id;
    string name, dep, password;
    char gender;
};

const int MAX_RECORDS = 100;
Record records[MAX_RECORDS];
int recordCount = 0;

// Function declarations
void loadRecords();
void saveRecords();
void adminMenu();
void studentMenu(const string&, const string&);

void addRecord();
void displayRecords();
void searchRecord();                // Admin only
void viewMyRecord(const string&);   // Student only
void editMyRecord(const string&);   // Student only
void editRecord(const string&, const string&);
void deleteRecord(const string&, const string&);

int getNextId();

/* ====================== MAIN ====================== */

int main() {
    loadRecords();

    cout << "==============================================\n";
    cout << "      WELCOME TO SCHOOL MANAGEMENT SYSTEM      \n";
    cout << "==============================================\n\n";

    string username, password;
    cout << "Enter Username: ";
    cin >> username;
    cout << "Enter Password: ";
    cin >> password;

    if (username == "admin" && password == "admin123") {
        adminMenu();
    } else {
        studentMenu(username, password);
    }

    saveRecords();
    cout << "\nThank you for using the system!\n";
    return 0;
}

/* ================= FILE HANDLING ================= */

void loadRecords() {
    recordCount = 0;
    ifstream file("dataFile.txt");
    string line;

    while (getline(file, line) && recordCount < MAX_RECORDS) {
        stringstream ss(line);
        Record& r = records[recordCount];
        string temp;

        getline(ss, temp, ',');
        r.id = 0;
        for (int i = 0; i < temp.length(); i++)
            r.id = r.id * 10 + (temp[i] - '0');

        getline(ss, r.name, ',');
        getline(ss, r.dep, ',');
        getline(ss, r.password, ',');
        ss >> r.gender;

        recordCount++;
    }
    file.close();
}

void saveRecords() {
    ofstream file("dataFile.txt");
    for (int i = 0; i < recordCount; i++) {
        file << records[i].id << ","
             << records[i].name << ","
             << records[i].dep << ","
             << records[i].password << ","
             << records[i].gender << endl;
    }
    file.close();
}

/* ================= ADMIN MENU ================= */

void adminMenu() {
    int choice;
    do {
        cout << "\n----------------------------------------------\n";
        cout << "                 ADMIN PANEL                  \n";
        cout << "----------------------------------------------\n";
        cout << "1. Add Student Record\n";
        cout << "2. Display All Records\n";
        cout << "3. Search Student Record\n";
        cout << "4. Edit Student Record\n";
        cout << "5. Delete Student Record\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: addRecord(); break;
            case 2: displayRecords(); break;
            case 3: searchRecord(); break;
            case 4: editRecord("admin", "admin"); break;
            case 5: deleteRecord("admin", "admin"); break;
            case 6: cout << "Exiting Admin Panel...\n"; break;
            default: cout << "Invalid choice! Try again.\n";
        }
    } while (choice != 6);
}

/* ================= STUDENT MENU ================= */

void studentMenu(const string& username, const string& password) {
    bool valid = false;
    for (int i = 0; i < recordCount; i++) {
        if (records[i].name == username && records[i].password == password) {
            valid = true;
            break;
        }
    }

    if (!valid) {
        cout << "\nLogin failed! Invalid credentials.\n";
        return;
    }

    int choice;
    do {
        cout << "\n----------------------------------------------\n";
        cout << "                STUDENT PANEL                 \n";
        cout << "----------------------------------------------\n";
        cout << "Welcome, " << username << "!\n\n";
        cout << "1. View My Record\n";
        cout << "2. Edit My Record\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: viewMyRecord(username); break;
            case 2: editMyRecord(username); break;
            case 3: cout << "Logging out...\n"; break;
            default: cout << "Invalid choice! Try again.\n";
        }
    } while (choice != 3);
}

/* ================= STUDENT FUNCTIONS ================= */

void viewMyRecord(const string& username) {
    cout << "\n------------ MY RECORD ------------\n";
    for (int i = 0; i < recordCount; i++) {
        if (records[i].name == username) {
            cout << "ID         : " << records[i].id << endl;
            cout << "Name       : " << records[i].name << endl;
            cout << "Department : " << records[i].dep << endl;
            cout << "Gender     : " << records[i].gender << endl;
            return;
        }
    }
    cout << "Record not found!\n";
}

void editMyRecord(const string& username) {
    for (int i = 0; i < recordCount; i++) {
        if (records[i].name == username) {
            cout << "\n--------- EDIT MY RECORD ----------\n";
            cout << "Current Department: " << records[i].dep << endl;
            cout << "Enter new Department: ";
            cin >> records[i].dep;

            cout << "Current Gender: " << records[i].gender << endl;
            cout << "Enter new Gender (M/F): ";
            cin >> records[i].gender;

            cout << "Record updated successfully!\n";
            return;
        }
    }
}

/* ================= ADMIN FUNCTIONS ================= */

void addRecord() {
    if (recordCount >= MAX_RECORDS) {
        cout << "Record limit reached!\n";
        return;
    }

    Record& r = records[recordCount];
    r.id = getNextId();

    cout << "\n--------- ADD NEW STUDENT ----------\n";
    cout << "Enter Name: ";
    cin >> r.name;
    cout << "Enter Department: ";
    cin >> r.dep;
    cout << "Set Password: ";
    cin >> r.password;
    cout << "Enter Gender (M/F): ";
    cin >> r.gender;

    recordCount++;
    cout << "Student record added successfully!\n";
}

void displayRecords() {
    cout << "\n---------------- ALL RECORDS ----------------\n";
    cout << left << setw(6) << "ID"
         << setw(15) << "Name"
         << setw(15) << "Department"
         << setw(10) << "Gender" << endl;
    cout << "--------------------------------------------\n";

    for (int i = 0; i < recordCount; i++) {
        cout << left << setw(6) << records[i].id
             << setw(15) << records[i].name
             << setw(15) << records[i].dep
             << setw(10) << records[i].gender << endl;
    }
}

void searchRecord() {
    int id;
    cout << "Enter Student ID to search: ";
    cin >> id;

    for (int i = 0; i < recordCount; i++) {
        if (records[i].id == id) {
            cout << "\nRecord Found!\n";
            cout << "Name: " << records[i].name
                 << ", Department: " << records[i].dep
                 << ", Gender: " << records[i].gender << endl;
            return;
        }
    }
    cout << "Record not found!\n";
}

void editRecord(const string& user, const string& role) {
    int id;
    cout << "Enter Student ID to edit: ";
    cin >> id;

    for (int i = 0; i < recordCount; i++) {
        if (records[i].id == id) {
            cout << "Enter new Name: ";
            cin >> records[i].name;
            cout << "Enter new Department: ";
            cin >> records[i].dep;
            cout << "Enter new Gender: ";
            cin >> records[i].gender;
            cout << "Record updated successfully!\n";
            return;
        }
    }
    cout << "Record not found!\n";
}

void deleteRecord(const string&, const string&) {
    int id;
    cout << "Enter Student ID to delete: ";
    cin >> id;

    for (int i = 0; i < recordCount; i++) {
        if (records[i].id == id) {
            for (int j = i; j < recordCount - 1; j++)
                records[j] = records[j + 1];
            recordCount--;
            cout << "Record deleted successfully!\n";
            return;
        }
    }
    cout << "Record not found!\n";
}


int getNextId() {
    int maxId = 0;
    for (int i = 0; i < recordCount; i++)
        if (records[i].id > maxId)
            maxId = records[i].id;
    return maxId + 1;
}

