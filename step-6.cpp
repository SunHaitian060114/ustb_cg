#include <iomanip>
#include "AccountRecord.h"
#include "command_history.h"
#include "UserManager.h"
#include "User.h"
#include <sstream>

using namespace std;

void handleUserMenu();

/*commands函数*/
std::string extractIdFromCommand(const std::string& command) {
    std::string id = command.substr(4, 8);
    return id;
}

double extractRateFromCommand(const std::string& command) {
    std::size_t pos = command.find_last_of(" ");
    return std::stod(command.substr(pos + 1));
}

int extractIndexFromCommand(const std::string& command) {
    std::size_t pos = command.find(" ");
    return std::stoi(command.substr(pos + 1, 1));
}

double extractAmountFromCommand(const std::string& command) {
    std::size_t pos1 = command.find(" ", 2);
    std::size_t pos2 = command.find(" ", pos1 + 1);
    return std::stod(command.substr(pos1 + 1, pos2 - pos1 - 1));
}

std::string extractDescriptionFromCommand(const std::string& command) {
    std::size_t pos = command.find_last_of(" ");
    return command.substr(pos + 1);
}

void executeCommand(const std::string& command, Date& date, std::vector<Account*>& accounts) {
    std::istringstream commandStream(command);
    std::string action;
    commandStream >> action;
    
    if (action == "a") {
        std::string type, id;
        double rate;
        commandStream >> type >> id >> rate;
        Account* account = nullptr;

        if (type == "s") {
            account = new SavingsAccount(date, id, rate);
        } else if (type == "c") {
            double credit, fee;
            commandStream >> credit >> fee;
            account = new CreditAccount(date, id, credit, rate, fee);
        }

        if (account != nullptr) {
            accounts.push_back(account);
        }
    }
    if (action == "d") {
        int index;
        double amount;
        std::string desc;
        commandStream >> index >> amount;
        std::getline(commandStream, desc);

        if (index >= 0 && index < accounts.size() && accounts[index] != nullptr) {
            accounts[index]->deposit(date, amount, desc);
        } else {
            std::cout << "Invalid account index or null pointer!" << std::endl;
        }
    }
    else if (action == "w") {
        int index;
        double amount;
        std::string desc;
        commandStream >> index >> amount;
        std::getline(commandStream, desc);

        if (index >= 0 && index < accounts.size() && accounts[index] != nullptr) {
            accounts[index]->withdraw(date, amount, desc);
        } else {
            std::cout << "Invalid account index or null pointer!" << std::endl;
        }
    }
    else if (action == "s") {
        for (size_t i = 0; i < accounts.size(); i++) {
            std::cout << "[" << i << "] ";
            accounts[i]->show();
            std::cout << std::endl;
        }
    }
    else if (action == "c") {
        int day;
        commandStream >> day;
        if (day < date.Get_day()) {
            std::cout << "You cannot specify a previous day" << std::endl;
        } else if (day > date.getMaxDay()) {
            std::cout << "Invalid day" << std::endl;
        } else {
            date = Date(date.Get_year(), date.Get_month(), day);
        }
    }
    else if (action == "n") {
        if (date.Get_month() == 12)
            date = Date(date.Get_year() + 1, 1, 1);
        else
            date = Date(date.Get_year(), date.Get_month() + 1, 1);

        for (auto& account : accounts) {
            account->settle(date);
        }
    }
    else if (action == "q") {
        Date date1 = Date::read();
        Date date2 = Date::read();
        Account::query(date1, date2);
    }
    else if (action == "e") {
        std::cout << "Exiting program..." << std::endl;
    }
}

struct deleter {
    template <class T> void operator () (T* p) { delete p; }
};

/*Menu函数*/
void showMainMenu() {
    cout << "\n========== BANK MENU ==========" << endl;
    cout << "1. Start" << endl;
    cout << "2. Exit System" << endl;
    cout << "Please Start: ";
}

void showUserMenu() {
    cout << "\n========== BANK MENU ==========" << endl;
    cout << "\n1. Create Account" << endl;
    cout << "2. Deposit" << endl;
    cout << "3. Withdraw" << endl;
    cout << "4. View Accounts" << endl;
    cout << "5. Change Date" << endl;
    cout << "6. Next Month" << endl;
    cout << "7. View Transaction History" << endl;
    cout << "8. View Monthly Statistics" << endl;
    cout << "9. Exit" << endl;
    cout << "Please choose: ";
}

void handleUserMenu() {
    Date date(2008, 11, 1);
    vector<Account*> accounts;
    
    vector<string> commands = CommandHistoryManager::loadCommands();
    for (const auto& cmd : commands) {
        if (CommandHistoryManager::isValidCommand(cmd)) {
            executeCommand(cmd, date, accounts);
        }
    }
    
    int choice;
do {
    showUserMenu();
    cin >> choice;
    cin.ignore();
    
    switch(choice) {
        case 1: {
            cout << "Enter account type (s for Savings/c for Credit): ";
            string type, id;
            cin >> type;
            cout << "Enter account ID: ";
            cin >> id;
            
            if (type == "s") {
                cout << "Enter annual interest rate: ";
                double rate;
                cin >> rate;
                accounts.push_back(new SavingsAccount(date, id, rate));
            } else if (type == "c") {
                cout << "Enter credit limit: ";
                double credit;
                cin >> credit;
                cout << "Enter annual interest rate: ";
                double rate;
                cin >> rate;
                cout << "Enter annual fee: ";
                double fee;
                cin >> fee;
                accounts.push_back(new CreditAccount(date, id, credit, rate, fee));
            }
            break;
        }
        case 2: {
            cout << "Enter account index: ";
            int index;
            cin >> index;
            cout << "Enter deposit amount: ";
            double amount;
            cin >> amount;
            cin.ignore();
            cout << "Enter description: ";
            string desc;
            getline(cin, desc);
            
            if (index >= 0 && index < accounts.size()) {
                accounts[index]->deposit(date, amount, desc);
            }
            break;
        }
        case 3: {
            cout << "Enter account index: ";
            int index;
            cin >> index;
            cout << "Enter withdrawal amount: ";
            double amount;
            cin >> amount;
            cin.ignore();
            cout << "Enter description: ";
            string desc;
            getline(cin, desc);
            
            if (index >= 0 && index < accounts.size()) {
                accounts[index]->withdraw(date, amount, desc);
            }
            break;
        }
        case 4: {
            for (size_t i = 0; i < accounts.size(); i++) {
                cout << "[" << i << "] ";
                accounts[i]->show();
                cout << endl;
            }
            break;
        }
        case 5: {
            cout << "Enter new date (yyyy/mm/dd): ";
            int y, m, d;
            char sep1, sep2;
            cin >> y >> sep1 >> m >> sep2 >> d;
            date = Date(y, m, d);
            break;
        }
        case 6: {
            if (date.Get_month() == 12)
                date = Date(date.Get_year() + 1, 1, 1);
            else
                date = Date(date.Get_year(), date.Get_month() + 1, 1);

            for (auto& account : accounts) {
                account->settle(date);
            }
            break;
        }
        case 7: {
            cout << "1. Sort by date\n2. Sort by amount\nChoose: ";
            int sortChoice;
            cin >> sortChoice;
            cin.ignore();
            
            cout << "Enter month (yyyy/mm): ";
            int y, m;
            char sep;
            cin >> y >> sep >> m;
            
            Date monthStart(y, m, 1);
            vector<AccountRecord> records;
            
            if (sortChoice == 1) {
                records = Account::queryByDate(monthStart);
            } else {
                records = Account::queryByAmount(monthStart);
            }
            
            for (const auto& record : records) {
                record.show();
            }
            break;
        }
        case 8: {
            for (size_t i = 0; i < accounts.size(); i++) {
                auto stats = accounts[i]->getMonthlyStats(date);
                cout << "Account " << accounts[i]->getID() << " monthly stats:\n";
                cout << "Total income: " << stats.totalDeposit << endl;
                cout << "Total expense: " << stats.totalWithdraw << endl;
            }
            break;
        }
        case 9: {
            for_each(accounts.begin(), accounts.end(), deleter());
            return;
        }
        default:
            cout << "Invalid choice" << endl;
    }
} while (choice != 9);
}

int main() {
    int choice;
    do {
        showMainMenu();
        cin >> choice;
        cin.ignore();
        
        switch(choice) {
            case 1:
                handleUserMenu();
                break;
            case 2:
                cout << "Exit System" << endl;
                break;
            default:
                cout << "Invalid Choice" << endl;
        }
    } while (choice != 2);
    
    return 0;
}