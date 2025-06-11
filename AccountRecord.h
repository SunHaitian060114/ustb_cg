#ifndef ACCOUNT_RECORD_H
#define ACCOUNT_RECORD_H

#include "account.h"

class AccountRecord {
private:
    Date date;            // 交易日期  ==  下面的Date date
    const Account* account;  // 账户
    double amount;        // 交易金额（存款或取款）==  下面的double amount
    double balance=0;       // 交易后的余额 == double balance
    std::string desc;     // 交易描述 == string aim

public:
    AccountRecord(const Date& date, const Account* account, double amount, double balance, const std::string& desc);
    void show() const;  // 显示账目信息
    // int getAccountID();
    // double getBalance();
    // double getAmount();
    // string getDesc();

    const Date& getDate() const { return date; }
    const string& getAccountID() const { return account->getID(); }
    double getAmount() const { return amount; }
    double getBalance() const { return balance; }
    const std::string& getDesc() const { return desc; }

};

#endif
 
