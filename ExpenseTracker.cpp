#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <iomanip>

using namespace std;

struct Transaction
{
    string date;
    string description;
    double amount;
};   

struct User
{
    string username;
    string password;
    double limit;
};

bool doesUserExist(const string &username, const vector<User> &users)
{
    for (const User &user : users)
    {
        if (user.username == username)
        {
            return true;
        }
    }
    return false;
}

void registerUser(vector<User> &users)
{
    User newUser;
    cout << "Enter a username: ";
    cin >> newUser.username;

    if (doesUserExist(newUser.username, users))
    {
        cout << "Username already exists. Please choose a different username.\n";
        return;
    }

    cout << "Enter a password: ";
    cin >> newUser.password;

    cout << "Enter expense limit: ";
    cin >> newUser.limit;

    ofstream userFile("users.txt", ios::app);
    userFile << newUser.username << " " << newUser.password << " " << newUser.limit << endl;
    userFile.close();

    users.push_back(newUser);

    cout << "Registration successful.\n";
}

bool loginUser(const string &username, const string &password, const vector<User> &users, User &currentUser)
{
    for (const User &user : users)
    {
        if (user.username == username && user.password == password)
        {
            currentUser = user;
            return true;
        }
    }
    return false;
}

void addBill(const User &currentUser, vector<Transaction> &transactions)
{
    Transaction newTransaction;
    cout << "Enter date (YYYY-MM-DD): ";
    cin >> newTransaction.date;

    cout << "Enter description: ";
    cin.ignore(); // Clear buffer before getline
    getline(cin, newTransaction.description);

    cout << "Enter amount: ";
    cin >> newTransaction.amount;

    // Check if the user has reached the expense limit
    if (newTransaction.amount > currentUser.limit)
    {
        cout << "Warning: You have exceeded your expense limit!\n";
    }

    ofstream transactionFile(currentUser.username + "_transactions.txt", ios::app);
    transactionFile << newTransaction.date << " " << newTransaction.description << " " << newTransaction.amount << endl;
    transactionFile.close();

    transactions.push_back(newTransaction);

    cout << "Bill added successfully.\n";
}

// Function to display all transactions
void displayAllTransactions(const vector<Transaction> &transactions)
{
    if (transactions.empty())
    {
        cout << "No transactions available.\n";
    }
    else
    {
        cout << "All Transactions:\n";
        for (const Transaction &transaction : transactions)
        {
            cout << "Date: " << transaction.date << " Description: " << transaction.description
                 << " Amount: " << transaction.amount << endl;
        }
    }
}

void displayTransactionsByCount(const vector<Transaction> &transactions)
{
    int count;
    cout << "Enter the count: ";
    cin >> count;

    if (count <= 0 || count > transactions.size())
    {
        cout << "Invalid count.\n";
    }
    else
    {
        cout << "Transactions for the last " << count << " entries:\n";
        for (int i = transactions.size() - count; i < transactions.size(); ++i)
        {
            const Transaction &transaction = transactions[i];
            cout << "Date: " << transaction.date << " Description: " << transaction.description
                 << " Amount: " << transaction.amount << endl;
        }
    }
}

void searchTransactionsByDate(const vector<Transaction> &transactions)
{
    string date;
    cout << "Enter date to search (YYYY-MM-DD): ";
    cin >> date;

    cout << "Transactions on " << date << ":\n";
    for (const Transaction &transaction : transactions)
    {
        if (transaction.date == date)
        {
            cout << "Description: " << transaction.description << " Amount: " << transaction.amount << endl;
        }
    }
}

int main()
{
    vector<User> users;
    vector<Transaction> transactions;
    User currentUser;

    ifstream userFile("users.txt");
    string username, password;
    double limit;

    while (userFile >> username >> password >> limit)
    {
        User user = {username, password, limit};
        users.push_back(user);
    }

    userFile.close();

    int menu1Choice, menu2Choice;

    do
    {
        cout << "\n|------- User Menu -------|\n";
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> menu1Choice;
        switch (menu1Choice)
        {
        case 1:
            registerUser(users);
            break;

        case 2:
            cout << "Enter your username: ";
            cin >> username;
            cout << "Enter your password: ";
            cin >> password;

            if (loginUser(username, password, users, currentUser))
            {
                cout << "Login successful.\n";
                do
                {
                    cout << "\n|------- Main Menu -------|\n";
                    cout << "1. Add Bill\n";
                    cout << "2. Display All Transactions\n";
                    cout << "3. Display Transactions by Count\n";
                    cout << "4. Search Transactions by Date\n";
                    cout << "5. Exit\n";
                    cout << "Enter your choice: ";
                    cin >> menu2Choice;

                    switch (menu2Choice)
                    {
                    case 1:
                        addBill(currentUser, transactions);
                        break;

                    case 2:
                        displayAllTransactions(transactions);
                        break;

                    case 3:
                        displayTransactionsByCount(transactions);
                        break;

                    case 4:
                        searchTransactionsByDate(transactions);
                        break;

                    case 5:
                        cout << "Exiting...\n";
                        break;

                    default:
                        cout << "Invalid choice. Please try again.\n";
                        break;
                    }

                } while (menu2Choice != 5);
            }
            else
            {
                cout << "Login failed. Invalid username or password.\n";
            }
            break;

        default:
            cout << "Invalid choice. Please try again.\n";
            break;
        }
    } while (menu1Choice != 3);

    return 0;
}
