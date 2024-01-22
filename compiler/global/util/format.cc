#include "format.hh"

#include <string>
#include <cstdarg>
#include <cstdlib>
#include <exception>

namespace xtaro::util
{
    
    std::string format(const char* fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        std::string result {vformat(fmt, args)};
        va_end(args);
        return result;
    }

    static char buffer[1024] {'\0'};

    std::string vformat(const char* fmt, va_list args)
    {
        int len {std::vsnprintf(buffer, sizeof(buffer), fmt, args)};
        if (len >= sizeof(buffer) || len <= 0) 
            return std::string{};
        buffer[len] = '\0';
        return std::string(buffer);
    }
}