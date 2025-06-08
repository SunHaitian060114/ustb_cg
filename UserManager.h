#pragma once
#include <vector>
#include <string>
#include "User.h"

class UserManager {
public:
    bool login(const std::string& username);
    bool registerUser(const std::string& username);
    void logout();
    User* getCurrentUser();
    void saveToFile();
    void loadFromFile();
    
private:
    std::vector<User> users;
    User* currentUser = nullptr;
};