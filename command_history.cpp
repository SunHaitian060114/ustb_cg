#include "command_history.h"
#include <regex>
#include<iostream>

const std::string file = "commands.txt";

void CommandHistoryManager::clearHistory() {
    std::ofstream fout("commands.txt", std::ios::trunc);  // 清空内容
    fout.close();
}

void CommandHistoryManager::saveCommand(const std::string& rawCommand) {
    // 读取现有命令
    std::ifstream fin(file);
    std::string line;
    std::vector<std::string> existingCommands;
    
    while (std::getline(fin, line)) {
        existingCommands.push_back(line);
    }

    // 检查命令是否已存在
    for (const auto& existingCommand : existingCommands) {
        if (existingCommand == rawCommand) {
            // 如果命令已存在，不再写入文件
            // std::cout << "Command already exists, skipping save." << std::endl;
            return;
        }
    }

    // 如果命令不重复，写入命令到文件
    std::ofstream fout(file, std::ios::app);  // 使用追加模式
    if (!fout) {
        std::cerr << "Error: Unable to open command history file!" << std::endl;
        throw std::runtime_error("无法写入命令历史文件");
    }

    fout << rawCommand << '\n';  // 保存新命令
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

bool CommandHistoryManager::isValidCommand(const std::string& cmd) {
    // 更宽松的匹配模式
    static const std::regex pattern(
        R"(^(a\s[sc]\s\w+\s[\d.]+(\s[\d.]+)*)|)"  // 添加账户
        R"(([dw]\s\d+\s[\d.-]+(\s.+)*)|)"        // 存款/取款
        R"(([csnqe]\s*.*))"                     // 其他命令
    );
    return std::regex_match(cmd, pattern);
}