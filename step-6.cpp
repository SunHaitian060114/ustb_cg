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

    // 保存命令到文件
    CommandHistoryManager::saveCommand(command);
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
    bool shouldExit = false;
do {
    showUserMenu();

    if(!(cin >> choice)) { // 检查输入是否成功
        cin.clear(); // 重置错误状态
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // 清空缓冲区
        cout << "Please enter a NUMBER (1 or 2)!\n";
        continue;
    }
    
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

                // ✅ 保存命令
                ostringstream oss;
                oss << "a " << type << " " << id << " " << rate;
                CommandHistoryManager::saveCommand(oss.str());

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

                // ✅ 保存命令
                ostringstream oss;
                oss << "a " << type << " " << id << " " << rate << " " << credit << " " << fee;
                CommandHistoryManager::saveCommand(oss.str());
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

                // ✅ 保存命令
                ostringstream oss;
                oss << "d " << index << " " << amount << " " << desc;
                CommandHistoryManager::saveCommand(oss.str());
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

                // ✅ 保存命令
                ostringstream oss;
                oss << "w " << index << " " << amount << " " << desc;
                CommandHistoryManager::saveCommand(oss.str());
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

            // ✅ 保存命令（格式为c d）
            ostringstream oss;
            oss << "c " << d;
            CommandHistoryManager::saveCommand(oss.str());
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

            // ✅ 保存命令
            CommandHistoryManager::saveCommand("n");
            break;
        }

case 7: {  // 查看交易历史
    cout << "1. Sort by date\n2. Sort by amount\nChoose: ";
    int sortChoice;
    cin >> sortChoice;
    cin.ignore();  // 清除输入缓冲区

    cout << "Enter month (yyyy/mm): ";
    int year, month;
    char sep;
    cin >> year >> sep >> month;  // 获取年月
    cin.ignore();  // 清除输入缓冲区

    auto records = Account::queryByMonth(year, month);  // 获取该月的所有交易记录

    if (records.empty()) {
        cout << "No transactions found for " << year << "/" << month << endl;
    } else {
        for (const auto& record : records) {
            record.show();  // 显示每条记录
        }
    }

    cout << "Press Enter to continue...";
    cin.ignore();
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
            cout << "Invalid choice (1-9 only)\n" << endl;
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