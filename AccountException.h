// AccountException.h
#pragma once
#include <stdexcept>
#include <string>
using namespace std;
 
class Account;

class AccountException : public runtime_error
{
private:
    const Account* account;
public:
    AccountException(const string& msg, const Account* acc);
    
    const Account* getAccount() const;
};