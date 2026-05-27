#include <iostream>
#include <fstream>
#include <iomanip>
#include <limits>
#include <cctype>

using namespace std;

static const double MIN_SAVINGS_BALANCE = 500.0;
static const double MIN_CURRENT_BALANCE = 1000.0;

// ==========================================
// CLASS DEFINITION (Encapsulation)
// ==========================================
class BankAccount {
private:
    int accountNumber;
    char accountHolderName[50];
    char accountType;
    double balance;

public:
    void createAccount() {
        cout << "\nEnter The Account Number: ";
        cin >> accountNumber;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Enter The Name of The Account Holder: ";
        cin.getline(accountHolderName, 50);

        do {
            cout << "Enter Account Type (S = Savings, C = Current): ";
            cin >> accountType;
            accountType = static_cast<char>(toupper(accountType));
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        } while (accountType != 'S' && accountType != 'C');

        double minBalance = (accountType == 'S') ? MIN_SAVINGS_BALANCE : MIN_CURRENT_BALANCE;
        do {
            cout << "Enter Initial Balance (>= " << minBalance << "): ";
            cin >> balance;
            if (cin.fail() || balance < minBalance) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid balance. Please enter a number at least " << minBalance << ".\n";
            }
        } while (balance < minBalance);

        cout << "\nAccount Created Successfully!\n";
    }

    void showAccount() const {
        cout << "\nAccount Number: " << accountNumber;
        cout << "\nAccount Holder Name: " << accountHolderName;
        cout << "\nAccount Type: " << ((accountType == 'S') ? "Savings" : "Current");
        cout << "\nBalance: $" << fixed << setprecision(2) << balance << "\n";
    }

    void deposit(double amount) {
        if (amount > 0.0) {
            balance += amount;
        }
    }

    bool withdraw(double amount) {
        if (amount <= 0.0) {
            cout << "\nWithdrawal amount must be positive.\n";
            return false;
        }

        double minBalance = (accountType == 'S') ? MIN_SAVINGS_BALANCE : MIN_CURRENT_BALANCE;
        if (amount > balance) {
            cout << "\nInsufficient Funds!\n";
            return false;
        }
        if ((balance - amount) < minBalance) {
            cout << "\nWithdrawal denied. Account must maintain a minimum balance of " << minBalance << ".\n";
            return false;
        }

        balance -= amount;
        return true;
    }

    int getAccountNumber() const {
        return accountNumber;
    }

    double getBalance() const {
        return balance;
    }
};

// ==========================================
// FUNCTION PROTOTYPES (File Handling)
// ==========================================
void writeAccountToFile();
bool accountExists(int);
void displayAccountFromFile(int);
void modifyAccountInFile(int, int); // 1 for deposit, 2 for withdraw

// ==========================================
// MAIN PROGRAM ENTRANCE
// ==========================================
int main() {
    char option;
    int accNum;

    do {
        cout << "\n\n====================================";
        cout << "\n      BANK MANAGEMENT SYSTEM";
        cout << "\n====================================";
        cout << "\n1. Create New Account";
        cout << "\n2. Deposit Amount";
        cout << "\n3. Withdraw Amount";
        cout << "\n4. Balance Enquiry";
        cout << "\n5. Exit";
        cout << "\n\nSelect Your Option (1-5): ";
        cin >> option;

        switch (option) {
            case '1':
                writeAccountToFile();
                break;
            case '2':
                cout << "\n\nEnter The Account Number: ";
                cin >> accNum;
                modifyAccountInFile(accNum, 1);
                break;
            case '3':
                cout << "\n\nEnter The Account Number: ";
                cin >> accNum;
                modifyAccountInFile(accNum, 2);
                break;
            case '4':
                cout << "\n\nEnter The Account Number: ";
                cin >> accNum;
                displayAccountFromFile(accNum);
                break;
            case '5':
                cout << "\nThank you for using our banking system.\n";
                break;
            default:
                cout << "\nInvalid Option! Please try again.";
        }
    } while (option != '5');

    return 0;
}

// ==========================================
// FILE HANDLING IMPLEMENTATIONS
// ==========================================

bool accountExists(int accountNumber) {
    BankAccount ac;
    ifstream inFile("bank_records.dat", ios::binary);
    if (!inFile) {
        return false;
    }

    while (inFile.read(reinterpret_cast<char*>(&ac), sizeof(BankAccount))) {
        if (ac.getAccountNumber() == accountNumber) {
            return true;
        }
    }
    return false;
}

void writeAccountToFile() {
    BankAccount ac;
    ac.createAccount();

    if (accountExists(ac.getAccountNumber())) {
        cout << "\nAn account with this number already exists.\n";
        return;
    }

    ofstream outFile("bank_records.dat", ios::binary | ios::app);
    if (!outFile) {
        cout << "\nUnable to open file for writing.\n";
        return;
    }

    outFile.write(reinterpret_cast<char*>(&ac), sizeof(BankAccount));
    outFile.close();
    cout << "Account saved to file successfully.\n";
}

// Function to read a specific record based on account number
void displayAccountFromFile(int n) {
    BankAccount ac;
    bool found = false;
    ifstream inFile("bank_records.dat", ios::binary);
    if (!inFile) {
        cout << "\nFile could not be opened!\n";
        return;
    }

    cout << "\nBALANCE DETAILS\n";
    while (inFile.read(reinterpret_cast<char*>(&ac), sizeof(BankAccount))) {
        if (ac.getAccountNumber() == n) {
            ac.showAccount();
            found = true;
            break;
        }
    }
    inFile.close();

    if (!found) {
        cout << "\nAccount number does not exist.\n";
    }
}

// Function to modify an existing record (Deposit/Withdraw)
void modifyAccountInFile(int n, int option) {
    bool found = false;
    BankAccount ac;
    fstream file("bank_records.dat", ios::binary | ios::in | ios::out);
    if (!file) {
        cout << "\nFile could not be opened!\n";
        return;
    }

    while (file.read(reinterpret_cast<char*>(&ac), sizeof(BankAccount)) && !found) {
        if (ac.getAccountNumber() == n) {
            ac.showAccount();

            double amount;
            if (option == 1) {
                cout << "\n\nTO DEPOSIT AMOUNT";
                cout << "\nEnter The Amount to be Deposited: ";
                cin >> amount;
                if (cin.fail() || amount <= 0.0) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "\nInvalid deposit amount.\n";
                    break;
                }
                ac.deposit(amount);
            } else if (option == 2) {
                cout << "\n\nTO WITHDRAW AMOUNT";
                cout << "\nEnter The Amount to be Withdrawn: ";
                cin >> amount;
                if (cin.fail() || amount <= 0.0) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "\nInvalid withdrawal amount.\n";
                    break;
                }
                if (!ac.withdraw(amount)) {
                    break;
                }
            }

            streampos position = file.tellg();
            file.seekp(position - static_cast<streamoff>(sizeof(BankAccount)));
            file.write(reinterpret_cast<char*>(&ac), sizeof(BankAccount));
            cout << "\n\nRecord Updated Successfully!\n";
            found = true;
        }
    }
    file.close();
    
    if (!found) {
        cout << "\nRecord Not Found.\n";
    }
}