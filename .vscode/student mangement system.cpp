#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <iomanip>
#include <limits>

using namespace std;
 

// Student Data Structure
struct Student {
    int rollNo;
    string name;
    string course;
    float marks;
};

// Global File Name
const string FILE_NAME = "students.txt";

// Function Prototypes
void addStudent();
void displayAll();
void updateStudent();
void deleteStudent();
void displayHeader();
bool isRollNoExists(int checkRoll);

int main() {
    int choice;

    while (true) {
        cout << "\n====================================\n";
        cout << "      STUDENT MANAGEMENT SYSTEM       \n";
        cout << "====================================\n";
        cout << "1. Add New Student\n";
        cout << "2. Display All Students\n";
        cout << "3. Update Student Record\n";
        cout << "4. Delete Student Record\n";
        cout << "5. Exit\n";
        cout << "Enter your choice (1-5): ";
        
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayAll(); break;
            case 3: updateStudent(); break;
            case 4: deleteStudent(); break;
            case 5: 
                cout << "Exiting the system. Goodbye!\n";
                return 0;
            default:
                cout << "Invalid choice! Please try again.\n";
        }
    }
}

// Helper function to check for duplicate roll numbers
bool isRollNoExists(int checkRoll) {
    ifstream file(FILE_NAME);
    if (!file) {
        return false; // If file doesn't exist yet, no duplicates exist
    }

    string line;
    auto trim = [](string &s) {
        s.erase(s.begin(), find_if(s.begin(), s.end(), [](unsigned char ch){ return !isspace(ch); }));
        s.erase(find_if(s.rbegin(), s.rend(), [](unsigned char ch){ return !isspace(ch); }).base(), s.end());
    };

    while (getline(file, line)) {
        stringstream ss(line);
        string rollStr;

        // We only need to check the first value (Roll Number)
        getline(ss, rollStr, ',');
        trim(rollStr);

        if (rollStr.empty()) continue;
        try {
            if (stoi(rollStr) == checkRoll) {
                file.close();
                return true; // Duplicate found
            }
        } catch (...) {
            continue; // malformed line, skip
        }
    }
    
    file.close();
    return false; // No duplicate found
}

// Function to add a new student to the file
void addStudent() {
    Student s;
    cout << "\n--- Add New Student ---\n";
    cout << "Enter Roll Number: ";
    cin >> s.rollNo;

    // Validation: Check if roll number already exists
    if (isRollNoExists(s.rollNo)) {
        cout << "Error: A student with Roll Number " << s.rollNo << " already exists!\n";
        cout << "Please try again with a unique Roll Number.\n";
        return; // Exit back to the main menu
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer before reading strings

    cout << "Enter Full Name: ";
    getline(cin, s.name);

    cout << "Enter Course: ";
    getline(cin, s.course);

    cout << "Enter Marks: ";
    cin >> s.marks;

    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid marks input. Aborting add.\n";
        return;
    }

    // Open file in append mode
    ofstream file(FILE_NAME, ios::app);
    if (!file) {
        cout << "Error: Could not open file for writing.\n";
        return;
    }

    // Save as comma-separated values
    file << s.rollNo << "," << s.name << "," << s.course << "," << s.marks << "\n";
    file.close();

    cout << "Student record added successfully!\n";
}

// Helper function to print table header
void displayHeader() {
    cout << "\n--------------------------------------------------------------\n";
    cout << left << setw(10) << "Roll No" 
         << setw(20) << "Name" 
         << setw(20) << "Course" 
         << setw(10) << "Marks" << endl;
    cout << "--------------------------------------------------------------\n";
}

// Function to display all students
void displayAll() {
    ifstream file(FILE_NAME);
    if (!file) {
        cout << "\nNo records found. The database is empty.\n";
        return;
    }

    string line;
    bool hasRecords = false;
    
    displayHeader();

    // Read file line by line
    auto trim = [](string &s) {
        s.erase(s.begin(), find_if(s.begin(), s.end(), [](unsigned char ch){ return !isspace(ch); }));
        s.erase(find_if(s.rbegin(), s.rend(), [](unsigned char ch){ return !isspace(ch); }).base(), s.end());
    };

    while (getline(file, line)) {
        stringstream ss(line);
        string rollStr, name, course, marksStr;

        // Parse comma-separated values
        getline(ss, rollStr, ',');
        getline(ss, name, ',');
        getline(ss, course, ',');
        getline(ss, marksStr, ',');

        trim(rollStr);
        trim(name);
        trim(course);
        trim(marksStr);

        if (!rollStr.empty()) {
            hasRecords = true;
            cout << left << setw(10) << rollStr 
                 << setw(20) << name 
                 << setw(20) << course 
                 << setw(10) << marksStr << endl;
        }
    }
    file.close();

    if (!hasRecords) {
        cout << "No records available to display.\n";
    }
}

// Function to update an existing student
void updateStudent() {
    int targetRoll;
    cout << "\n--- Update Student ---\n";
    cout << "Enter Roll Number to update: ";
    cin >> targetRoll;

    ifstream inFile(FILE_NAME);
    if (!inFile) {
        cout << "No records found to update.\n";
        return;
    }

    ofstream tempFile("temp.txt");
    string line;
    bool found = false;
    if (!tempFile) {
        cout << "Error: Could not open temporary file for writing.\n";
        inFile.close();
        return;
    }

    auto trim = [](string &s) {
        s.erase(s.begin(), find_if(s.begin(), s.end(), [](unsigned char ch){ return !isspace(ch); }));
        s.erase(find_if(s.rbegin(), s.rend(), [](unsigned char ch){ return !isspace(ch); }).base(), s.end());
    };

    while (getline(inFile, line)) {
        stringstream ss(line);
        string rollStr, name, course, marksStr;

        getline(ss, rollStr, ',');
        getline(ss, name, ',');
        getline(ss, course, ',');
        getline(ss, marksStr, ',');

        trim(rollStr);

        if (rollStr.empty()) {
            tempFile << line << "\n";
            continue;
        }

        int parsedRoll = -1;
        try {
            parsedRoll = stoi(rollStr);
        } catch (...) {
            // malformed roll, keep original line
            tempFile << line << "\n";
            continue;
        }

        if (parsedRoll == targetRoll) {
            found = true;
            Student s;
            s.rollNo = targetRoll;
            
            cout << "Record found! Enter new details:\n";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            cout << "Enter New Name: ";
            getline(cin, s.name);
            
            cout << "Enter New Course: ";
            getline(cin, s.course);
            
            cout << "Enter New Marks: ";
            cin >> s.marks;

            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid marks input. Skipping update.\n";
                // write original
                tempFile << line << "\n";
            } else {
                // Write updated record to temp file
                tempFile << s.rollNo << "," << s.name << "," << s.course << "," << s.marks << "\n";
                cout << "Record updated successfully!\n";
            }
        } else {
            // Write original record to temp file
            tempFile << line << "\n";
        }
    }

    inFile.close();
    tempFile.close();

    // Replace original file with temp using C API
    remove(FILE_NAME.c_str());
    if (rename("temp.txt", FILE_NAME.c_str()) != 0) {
        cout << "Warning: Could not replace original file.\n";
    }

    if (!found) {
        cout << "Student with Roll Number " << targetRoll << " not found.\n";
    }
}

// Function to delete a student record
void deleteStudent() {
    int targetRoll;
    cout << "\n--- Delete Student ---\n";
    cout << "Enter Roll Number to delete: ";
    cin >> targetRoll;

    ifstream inFile(FILE_NAME);
    if (!inFile) {
        cout << "No records found to delete.\n";
        return;
    }

    ofstream tempFile("temp.txt");
    string line;
    bool found = false;

    auto trim = [](string &s) {
        s.erase(s.begin(), find_if(s.begin(), s.end(), [](unsigned char ch){ return !isspace(ch); }));
        s.erase(find_if(s.rbegin(), s.rend(), [](unsigned char ch){ return !isspace(ch); }).base(), s.end());
    };

    while (getline(inFile, line)) {
        stringstream ss(line);
        string rollStr;

        getline(ss, rollStr, ',');
        trim(rollStr);

        if (rollStr.empty()) {
            tempFile << line << "\n";
            continue;
        }

        int parsedRoll = -1;
        try {
            parsedRoll = stoi(rollStr);
        } catch (...) {
            tempFile << line << "\n";
            continue;
        }

        if (parsedRoll == targetRoll) {
            found = true;
            // Skip writing this line to effectively delete it
            continue; 
        } else {
            // Keep all other records
            tempFile << line << "\n";
        }
    }

    inFile.close();
    tempFile.close();

    // Replace original file with updated temp file
    remove(FILE_NAME.c_str());
    rename("temp.txt", FILE_NAME.c_str());

    if (found) {
        cout << "Record deleted successfully!\n";
    } else {
        cout << "Student with Roll Number " << targetRoll << " not found.\n";
    }
}