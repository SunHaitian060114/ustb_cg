#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>
using namespace std;

class CommandHistoryManager
{
    public:
    //保存原始命令
    static void saveCommand(const std::string& rawCommand);
    //加载所有历史命令，按顺序执行
    static std::vector<std::string>loadCommands();
    //清空历史记录
    static void clearHistory();
    //命令有效性检测
    static bool isValidCommand(const std::string& cmd);

}; 