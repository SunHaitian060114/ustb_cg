#include "command_history.h"
#include <regex>
#include<iostream>

const std::string file = "commands.txt";

void CommandHistoryManager::saveCommand(const std::string& rawCommand) {
    if (rawCommand.empty() || rawCommand.find("(a)add") == 0 || rawCommand.find("Total:") != std::string::npos) {
        return;  // 防止空命令和无关的命令被保存
    }

    // 打开文件并追加命令
    std::ofstream fout(file, std::ios::app);
    if (!fout) {
        std::cerr << "Error: Unable to open command history file!" << std::endl;
        throw std::runtime_error("无法写入命令历史文件");
    }

    fout << rawCommand << '\n';  // 保存完整命令
    fout.close();
}



std::vector<std::string> CommandHistoryManager::loadCommands()
{
    std::vector<string>commands;
    std::ifstream fin(file);

    if(!fin) return commands;

    std::string line;
    while(std::getline(fin,line))
    {
        if(!line.empty())
        {
            commands.push_back(line);
        }
    }
    return commands;
}


// bool CommandHistoryManager::isValidCommand(const std::string& cmd) {
//     // 匹配图片中出现的所有命令类型
//     static const std::regex pattern(
//         R"(^(a\s[sc]\s\w+\s[\d.]+)|)"      // 添加账户：a s S123456 0.020
//         R"((d\s\d+\s\d+(\s.+)?)|)"        // 存款：d 0 5000 salary
//         R"((w\s\d+\s\d+(\s.+)?)|)"        // 取款：w 1 2000
//         R"((c\s\d+)|(n)|(s)|(q)|(e))"     // 其他单字母命令
//     );
//     return std::regex_match(cmd, pattern);
// }

// void CommandHistoryManager::clearHistory() {
//     std::ofstream fout(file, std::ios::trunc);
//     fout.close();
// }

bool CommandHistoryManager::isValidCommand(const std::string& cmd) {
    // 更宽松的匹配模式
    static const std::regex pattern(
        R"(^(a\s[sc]\s\w+\s[\d.]+(\s[\d.]+)*)|)"  // 添加账户
        R"(([dw]\s\d+\s[\d.-]+(\s.+)*)|)"        // 存款/取款
        R"(([csnqe]\s*.*))"                     // 其他命令
    );
    return std::regex_match(cmd, pattern);
}