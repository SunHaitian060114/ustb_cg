#include "UserManager.h"
#include <fstream>

bool UserManager::login(const std::string& username) {
    for (auto& user : users) {
        if (user.getUsername() == username) {
            currentUser = &user;
            return true;
        }
    }
    return false;
}

bool UserManager::registerUser(const std::string& username) {
    for (auto& user : users) {
        if (user.getUsername() == username) {
            return false;
        }
    }
    users.emplace_back(username);
    return true;
}

void UserManager::logout() {
    currentUser = nullptr;
}

User* UserManager::getCurrentUser() {
    return currentUser;
}

void UserManager::saveToFile() {
    std::ofstream fout("users.txt");
    for (auto& user : users) {
        fout << user.getUsername() << '\n';
    }
}

void UserManager::loadFromFile() {
    std::ifstream fin("users.txt");
    std::string username;
    while (std::getline(fin, username)) {
        if (!username.empty()) {
            users.emplace_back(username);
        }
    }
}