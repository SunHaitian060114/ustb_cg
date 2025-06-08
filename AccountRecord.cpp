#include "AccountRecord.h"
#include <iostream>

AccountRecord::AccountRecord(const Date& date, const Account* account, double amount, double balance, const string& desc)
    : date(date), account(account), amount(amount), balance(balance), desc(desc) {}

void AccountRecord::show() const {
    std::cout << date << "\t#" << account->getID() << "\t" << amount << "\t" << balance << "\t" << desc << endl;
}

// int AccountRecord::getAccountID()
// {
//     return 
// }