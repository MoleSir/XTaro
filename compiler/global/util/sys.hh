#pragma once

#include <string>

namespace xtaro::util
{
    int execute(const std::string& command) noexcept;
    int execute(const char* command) noexcept;

    char getCharNoEcho();
}