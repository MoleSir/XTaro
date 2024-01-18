#pragma once

#include <string>
#include <fstream>

namespace xtaro::util
{
    template<class... T>
    std::string format(const char* fmt, const T&...t)
    {
        const int len = std::snprintf(nullptr, 0, fmt, t...);
        std::string r;
        r.resize(static_cast<size_t>(len) + 1);
        std::snprintf(r.data(), len + 1, fmt, t...);
        r.resize(static_cast<size_t>(len));
        return r;
    }

    std::size_t filesize(std::ifstream& file);
    std::string readFile(std::ifstream& file);

    int power(int base, unsigned int exponent);

    int execute(const std::string& command) noexcept;
    int execute(const char* command) noexcept;
}