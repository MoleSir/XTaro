#include "util.hh"
#include <cstdarg>
#include <cstdlib>

namespace xtaro::util
{
    static char buffer[512];
    const char* format(const char* fmt, ...)
    {
        va_list args;
        va_start(args, fmt);

        int len = vsnprintf(buffer, 512, fmt, args);
        buffer[len] = '\0';
        
        va_end(args);

        return buffer;
    }

    int execute(const std::string& command) noexcept
    {
        return system(command.c_str());
    }

    int execute(const char* command) noexcept
    {
       return system(command);
    }
}