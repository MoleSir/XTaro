#include "sys.hh"
#include <cstdlib>
#include <iostream>

#include <termios.h>
#include <unistd.h>

namespace xtaro::util
{
    int execute(const std::string& command) noexcept
    {
        return std::system(command.c_str());
    }

    int execute(const char* command) noexcept
    {
       return std::system(command);
    }

    char getCharNoEcho()
    {
        struct termios oldt, newt;
        char ch;

        // 获取当前终端设置
        tcgetattr(STDIN_FILENO, &oldt);

        // 复制当前设置，以便在退出时恢复
        newt = oldt;

        // 禁用回显和标准输入模式
        newt.c_lflag &= ~(ICANON | ECHO);

        // 设置新的终端设置
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);

        // 从标准输入中读取一个字符
        std::cin.get(ch);

        // 恢复旧的终端设置
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

        return ch;
    }

}