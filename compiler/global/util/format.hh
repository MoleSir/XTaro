#pragma once

#include <string>

namespace xtaro::util
{
    template<class... T>
    std::string format(const char* fmt, const T&...t)
    {
        const int len = std::snprintf(nullptr, 0, fmt, t...);
        if (len < 0) return {};
        
        std::string r;
        r.resize(static_cast<size_t>(len) + 1);
        std::snprintf(r.data(), len + 1, fmt, t...);
        r.resize(static_cast<size_t>(len));
        return r;
    }
}