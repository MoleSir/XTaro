#pragma once

#include <string>

namespace xtaro::util
{
    const char* format(const char* fmt, ...);

    int execute(const std::string& command) noexcept;
    int execute(const char* command) noexcept;
}