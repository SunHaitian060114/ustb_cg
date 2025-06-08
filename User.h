#pragma once
#include <string>
#include <vector>

class User {
public:
    User(const std::string& username);
    
    const std::string& getUsername() const;
    const std::vector<int>& getAccountIndices() const;
    void addAccount(int index);
    
private:
    std::string username;
    std::vector<int> accountIndices;
};