#include "AccountException.h"

AccountException::AccountException(const std::string& msg, const Account* acc): runtime_error(msg), account(acc) 
{

}

const Account* AccountException::getAccount() const 
{ 
    return account;
}