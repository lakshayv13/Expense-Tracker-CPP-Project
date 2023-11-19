#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdlib>

using namespace std;

struct User {
    int userId;
    string username;
    string password;
};

struct Transaction {
    string username;
    int amount;
    string type;  // "sent" or "received"
};

class ExpenseTracker {
private:
    vector<User> users;
    string dataFilePath = "/path/to/data.csv";

    int getUserId() {
        // Generate a unique user ID based on the current number of users
        return users.size() + 1;
    }

    vector<string> splitString(string s) {
        stringstream ss(s);
        string word;
        vector<string> words;
        while (ss >> word) {
            words.push_back(word);
        }
        return words;
    }

    void readUserData() {
        ifstream file(dataFilePath);
        if (!file.is_open()) {
            cerr << "Error opening file: " << dataFilePath << endl;
            exit(EXIT_FAILURE);
        }

        string line;
        while (getline(file, line)) {
            vector<string> userData = splitString(line);
            User user{
                stoi(userData[0]),  // User ID
                userData[1],        // Username
                userData[2]         // Password
            };
            users.push_back(user);
        }

        file.close();
    }

    void writeUserData() {
        ofstream file(dataFilePath, ios::trunc);
        if (!file.is_open()) {
            cerr << "Error opening file: " << dataFilePath << endl;
            exit(EXIT_FAILURE);
        }

        for (const auto &user : users) {
            file << user.userId << " " << user.username << " " << user.password << "\n";
        }

        file.close();
    }

    void createUser() {
        User newUser;
        newUser.userId = getUserId();

        cout << "Enter username: ";
        cin >> newUser.username;

        cout << "Enter password: ";
        cin >> newUser.password;

        users.push_back(newUser);
        cout << "User successfully registered!\n";
    }

    bool loginUser(string username, string password) {
        for (const auto &user : users) {
            if (user.username == username && user.password == password) {
                cout << "Successfully logged in. Welcome, " << user.username << "!\n";
                return true;
            }
        }

        cout << "Invalid username or password. Please try again.\n";
        return false;
    }

    void addTransaction(const string &username) {
        Transaction transaction;
        transaction.username = username;

        cout << "Enter the transaction amount: ";
        cin >> transaction.amount;

        cout << "Is the money sent or received? (s/r): ";
        cin >> transaction.type;

        ofstream transactionFile("/path/to/transactions/" + username + "_transactions.csv", ios::app);
        if (!transactionFile.is_open()) {
            cerr << "Error opening transaction file.\n";
            exit(EXIT_FAILURE);
        }

        transactionFile << transaction.username << " " << transaction.amount << " " << transaction.type << "\n";
        transactionFile.close();

        cout << "Transaction recorded successfully!\n";
    }

    void viewTransactions(const string &username) {
        ifstream transactionFile("/path/to/transactions/" + username + "_transactions.csv");
        if (!transactionFile.is_open()) {
            cerr << "Error opening transaction file.\n";
            exit(EXIT_FAILURE);
        }

        string line;
        cout << "Transaction History:\n";
        while (getline(transactionFile, line)) {
            cout << line << endl;
        }

        transactionFile.close();
    }

public:
    ExpenseTracker() {
        readUserData();
    }

    ~ExpenseTracker() {
        writeUserData();
    }

    void run() {
        string username, password;
        int choice;

        cout << "|----------- EXPENSE TRACKER -----------|\n";
        cout << "| 1. Register\n";
        cout << "| 2. Login\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            createUser();
        } else if (choice == 2) {
            cout << "Enter username: ";
            cin >> username;

            cout << "Enter password: ";
            cin >> password;

            if (loginUser(username, password)) {
                string transactionFilePath = "/path/to/transactions/" + username + "_transactions.csv";
                ofstream transactionFile(transactionFilePath, ios::app);
                if (!transactionFile.is_open()) {
                    cerr << "Error opening transaction file.\n";
                    exit(EXIT_FAILURE);
                }
                transactionFile.close();

                do {
                    cout << "\n|------------ EXPENSE TRACKER ------------|\n";
                    cout << "| 1. Add Transaction\n";
                    cout << "| 2. View Transactions\n";
                    cout << "| 3. Logout\n";
                    cout << "Enter choice: ";
                    cin >> choice;

                    switch (choice) {
                    case 1:
                        addTransaction(username);
                        break;
                    case 2:
                        viewTransactions(username);
                        break;
                    case 3:
                        cout << "Logging out...\n";
                        break;
                    default:
                        cout << "Invalid choice. Please try again.\n";
                    }
                } while (choice != 3);
            }
        } else {
            cout << "Invalid choice. Exiting...\n";
        }
    }
};

int main() {
    ExpenseTracker expenseTracker;
    expenseTracker.run();

    return 0;
}
