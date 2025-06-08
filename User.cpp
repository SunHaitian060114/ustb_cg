#include "User.h"

User::User(const std::string& username) : username(username) {}

const std::string& User::getUsername() const {
    return username;
}

const std::vector<int>& User::getAccountIndices() const {
    return accountIndices;
}

void User::addAccount(int index) {
    accountIndices.push_back(index);
}