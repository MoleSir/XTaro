#pragma once

#include <string>
#include <cstdarg>

namespace xtaro::util
{
    std::string format(const char* fmt, ...);
    std::string vformat(const char* fmt, va_list args);
}